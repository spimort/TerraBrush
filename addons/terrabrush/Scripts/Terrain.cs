using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Terrain : Node3D {
	private ShaderMaterial _terrainColorShader = null;
    private HeightMapShape3D _heightMapCollisionShape = null;
    private CancellationTokenSource _collisionCancellationSource = null;

    [NodePath] private Clipmap _clipmap;
    [NodePath] private CollisionShape3D _terrainCollision;
    [NodePath] private StaticBody3D _terrainCollider;
    [NodePath] private SubViewport _resultViewport;
    [NodePath] private ColorRect _terrainColorRect;

    [Export] public int TerrainSize { get;set; }
    [Export] public int TerrainSubDivision { get;set; }
    [Export] public Texture2D HeightMap { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public Texture2D[] Splatmaps { get;set; } = new Texture2D[]{};
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public ImageTexture WaterTexture { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export] public Texture2D DefaultTexture { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
    [Export(PropertyHint.Layers3DPhysics)] public int CollisionLayers { get;set; } = 1;
    [Export(PropertyHint.Layers3DPhysics)] public int CollisionMask { get;set; } = 1;
    [Export] public int LODLevels { get;set; } = 8;
    [Export] public int LODRowsPerLevel { get;set; } = 21;
    [Export] public float LODInitialCellWidth { get;set; } = 1;
    [Export] public bool CreateCollisionInThread { get;set; } = true;

    public StaticBody3D TerrainCollider => _terrainCollider;
    public SubViewport ResultViewport => _resultViewport;
    public Clipmap Clipmap => _clipmap;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        _terrainColorShader = (ShaderMaterial) _terrainColorRect.Material;
    }

    public void BuildTerrain(bool collisionOnly = false) {
        if (_clipmap == null) {
            return;
        }

        _clipmap.ClipmapMesh.Layers = (uint) VisualInstanceLayers;

        var heightMapShape3D = new HeightMapShape3D();
        _terrainCollision.Shape = heightMapShape3D;
        _heightMapCollisionShape = heightMapShape3D;
        heightMapShape3D.MapWidth = TerrainSize;
        heightMapShape3D.MapDepth = TerrainSize;

        _terrainCollider.CollisionLayer = (uint) CollisionLayers;
        _terrainCollider.CollisionMask = (uint) CollisionMask;

        if (collisionOnly) {
            UpdateCollisionShape();
            _clipmap.ClipmapMesh.Visible = false;
        } else {
            _clipmap.Heightmap = HeightMap;
            _clipmap.HeightmapFactor = HeightMapFactor;
            _clipmap.Levels = LODLevels;
            _clipmap.RowsPerLevel = LODRowsPerLevel;
            _clipmap.InitialCellWidth = LODInitialCellWidth;

            _clipmap.CreateMesh();

            TerrainUpdated(true);
            TerrainTextureUpdated();
            TerrainWaterUpdated();
        }
    }

    public void TerrainUpdated(bool updateCollision = false) {
        if (updateCollision) {
            UpdateCollisionShape();
        }
    }

    public void TerrainTextureUpdated() {
        UpdateTextures();
        TerrainSplatmapsUpdated();
    }

    public void TerrainSplatmapsUpdated() {
        if (this.Splatmaps?.Count() > 0) {
            var splatmaps = TexturesToTextureArray(Splatmaps);
    		Clipmap.Shader.SetShaderParameter("Splatmaps", splatmaps);
            _terrainColorShader.SetShaderParameter("Splatmaps", splatmaps);
            _resultViewport.RenderTargetUpdateMode = SubViewport.UpdateMode.Once;
        }
    }

    public void TerrainSplatmapsUpdated(IEnumerable<Image> images) {
        if (images.Count() > 0) {
            var splatmaps = ImagesToTextureArray(images);
    		Clipmap.Shader.SetShaderParameter("Splatmaps", splatmaps);
            _terrainColorShader.SetShaderParameter("Splatmaps", splatmaps);
            _resultViewport.RenderTargetUpdateMode = SubViewport.UpdateMode.Once;
        }
    }

    public void TerrainWaterUpdated() {
    	Clipmap.Shader.SetShaderParameter("WaterTexture", WaterTexture);
    	Clipmap.Shader.SetShaderParameter("WaterFactor", WaterFactor);
    }

	private void UpdateCollisionShape() {
        if (CreateCollisionInThread) {
            _collisionCancellationSource?.Cancel();
            _collisionCancellationSource = new CancellationTokenSource();
        }

        var token = CreateCollisionInThread ? _collisionCancellationSource.Token : CancellationToken.None;

        var updateAction = () => {
            var heightMapImage = HeightMap.GetImage();
            var waterImage = WaterTexture?.GetImage();

            if (token.IsCancellationRequested) {
                return;
            }

            var terrainData = new List<float>();
            for (var y = 0; y < heightMapImage.GetHeight(); y++) {
                for (var x = 0; x < heightMapImage.GetWidth(); x++) {
                    if (token.IsCancellationRequested) {
                        return;
                    }

                    var pixelHeight = heightMapImage.GetPixel(x, y).R * this.HeightMapFactor;
                    var waterHeight = waterImage?.GetPixel(x, y).R ?? 0;

                    pixelHeight -= waterHeight * WaterFactor;

                    terrainData.Add(pixelHeight);
                }
            }

            if (token.IsCancellationRequested) {
                return;
            }

            CallDeferred(nameof(AssignCollisionData), terrainData.ToArray());
        };

        if (CreateCollisionInThread) {
            Task.Factory.StartNew(() => {
                updateAction();
            }, token);
        } else {
            updateAction();
        }
	}

    private void AssignCollisionData(float[] data) {
        _heightMapCollisionShape.MapData = data;
    }

	private void UpdateTextures() {
		Clipmap.Shader.SetShaderParameter("TextureDetail", this.TextureDetail);

        if (this.TextureSets?.TextureSets?.Count() > 0) {
            var textureArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.AlbedoTexture));
            Clipmap.Shader.SetShaderParameter("Textures", textureArray);
            Clipmap.Shader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());
            _terrainColorShader.SetShaderParameter("Textures", textureArray);
            _terrainColorShader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            var normalArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.NormalTexture));
            Clipmap.Shader.SetShaderParameter("Normals", normalArray);

            var roughnessArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.RoughnessTexture));
            Clipmap.Shader.SetShaderParameter("RoughnessTexutres", roughnessArray);

            Clipmap.Shader.SetShaderParameter("UseAntitile", true);
        } else if (DefaultTexture != null) {
            var textureArray = this.TexturesToTextureArray(new Texture2D[] {DefaultTexture});
            Clipmap.Shader.SetShaderParameter("Textures", textureArray);
            Clipmap.Shader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());
            _terrainColorShader.SetShaderParameter("Textures", textureArray);
            _terrainColorShader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            Clipmap.Shader.SetShaderParameter("UseAntitile", false);
        }

        _resultViewport.RenderTargetUpdateMode = SubViewport.UpdateMode.Once;
	}

	private Texture2DArray TexturesToTextureArray(IEnumerable<Texture2D> textures) {
		var textureArray = new Texture2DArray();
		var textureImageArray = new Godot.Collections.Array<Image>();

		int width = 0;
		int height = 0;

		if (textures != null) {
			textures.ToList().ForEach(texture => {
				if (texture != null) {
					var textureImage = texture.GetImage();

					if (width == 0) {
						width = textureImage.GetWidth();
						height = textureImage.GetHeight();
					} else if (textureImage.GetWidth() != width || textureImage.GetHeight() != height) {
						textureImage.Resize(width, height);
					}

					textureImageArray.Add(textureImage);
				}
			});
		}

		textureArray._Images = textureImageArray;

		return textureArray;
	}

	private Texture2DArray ImagesToTextureArray(IEnumerable<Image> images) {
		return new Texture2DArray() {
            _Images = new Godot.Collections.Array<Image>(images)
        };
	}
}
