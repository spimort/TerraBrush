using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Terrain : Node3D {
	private ShaderMaterial _heightMapShader = null;
    private HeightMapShape3D _heightMapCollisionShape = null;

    [NodePath] private MeshInstance3D _terrainMesh;
    [NodePath] private MeshInstance3D _resultMesh;
    [NodePath] private Camera3D _resultMeshCamera;
    [NodePath] private CollisionShape3D _terrainCollision;
    [NodePath] private StaticBody3D _terrainCollider;
    [NodePath] private SubViewport _resultViewport;

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

    public MeshInstance3D TerrainMesh => _terrainMesh;
    public StaticBody3D TerrainCollider => _terrainCollider;
    public SubViewport ResultViewport => _resultViewport;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

		this._heightMapShader = (ShaderMaterial) this._terrainMesh.GetSurfaceOverrideMaterial(0);
        this._resultMesh.SetSurfaceOverrideMaterial(0, _heightMapShader);
    }

    public void BuildTerrain(bool collisionOnly = false) {
        _terrainMesh.Layers = (uint) VisualInstanceLayers;

        var heightMapShape3D = new HeightMapShape3D();
        _terrainCollision.Shape = heightMapShape3D;
        _heightMapCollisionShape = heightMapShape3D;
        heightMapShape3D.MapWidth = TerrainSize;
        heightMapShape3D.MapDepth = TerrainSize;

        _terrainCollider.CollisionLayer = (uint) CollisionLayers;
        _terrainCollider.CollisionMask = (uint) CollisionMask;

        if (collisionOnly) {
            UpdateCollisionShape();
            _terrainMesh.Visible = false;
        } else {
            ((PlaneMesh) _resultMesh.Mesh).Size = new Vector2I(TerrainSize, TerrainSize);
            _resultMeshCamera.Size = TerrainSize;

            var planeMesh = (PlaneMesh) _terrainMesh.Mesh;

            planeMesh.Size = new Vector2I(TerrainSize, TerrainSize);
            planeMesh.SubdivideWidth = TerrainSubDivision;
            planeMesh.SubdivideDepth = TerrainSubDivision;

            TerrainUpdated(true);
            TerrainTextureUpdated();
            TerrainWaterUpdated();
        }
    }

    public void TerrainUpdated(bool updateCollision = false) {
        UpdateShaderParams();

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
    		this._heightMapShader.SetShaderParameter("Splatmaps", this.TexturesToTextureArray(this.Splatmaps));
        }
    }

    public void TerrainWaterUpdated() {
    	this._heightMapShader.SetShaderParameter("WaterTexture", WaterTexture);
    	this._heightMapShader.SetShaderParameter("WaterFactor", WaterFactor);
    }

	private void UpdateCollisionShape() {
        var heightMapImage = HeightMap.GetImage();
        var waterImage = WaterTexture?.GetImage();

        var terrainData = new Godot.Collections.Array<float>();
        for (var y = 0; y < heightMapImage.GetHeight(); y++) {
            for (var x = 0; x < heightMapImage.GetWidth(); x++) {
                var pixelHeight = heightMapImage.GetPixel(x, y).R * this.HeightMapFactor;
                var waterHeight = waterImage?.GetPixel(x, y).R ?? 0;

                pixelHeight -= waterHeight * WaterFactor;

                terrainData.Add(pixelHeight);
            }
        }

        _heightMapCollisionShape.MapData = terrainData.ToArray();
	}

	private void UpdateShaderParams() {
		this._heightMapShader.SetShaderParameter("HeightmapTexture", this.HeightMap);
		this._heightMapShader.SetShaderParameter("HeightmapFactor", this.HeightMapFactor);
	}

	private void UpdateTextures() {
		this._heightMapShader.SetShaderParameter("TextureDetail", this.TextureDetail);

        if (this.TextureSets?.TextureSets?.Count() > 0) {
            var textureArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.AlbedoTexture));
            this._heightMapShader.SetShaderParameter("Textures", textureArray);
            this._heightMapShader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            var normalArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.NormalTexture));
            this._heightMapShader.SetShaderParameter("Normals", normalArray);

            var roughnessArray = this.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.RoughnessTexture));
            this._heightMapShader.SetShaderParameter("RoughnessTexutres", roughnessArray);

            this._heightMapShader.SetShaderParameter("UseAntitile", true);
        } else if (DefaultTexture != null) {
            var textureArray = this.TexturesToTextureArray(new Texture2D[] {DefaultTexture});
            this._heightMapShader.SetShaderParameter("Textures", textureArray);
            this._heightMapShader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            this._heightMapShader.SetShaderParameter("UseAntitile", false);
        }
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
}
