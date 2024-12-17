using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public enum AlphaChannelUsage {
    None = 0,
    Roughness = 1,
    Height = 2
}

[Tool]
public partial class Terrain : Node3D {
    private const float HoleValue = float.NaN;

    private CancellationTokenSource _collisionCancellationSource = null;

    [NodePath] private Clipmap _clipmap;
    [NodePath] private StaticBody3D _terrainCollider;

    [Export] public int ZonesSize { get;set; }
    [Export] public int Resolution { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public ShaderMaterial CustomShader { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public bool UseAntiTile { get;set; } = true;
    [Export] public bool NearestTextureFilter { get;set; } = false;
    [Export] public float HeightBlendFactor { get;set; } = 10f;
    [Export] public AlphaChannelUsage AlbedoAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;
    [Export] public AlphaChannelUsage NormalAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;
    [Export] public float WaterFactor { get;set; }
    [Export] public Texture2D DefaultTexture { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
    [Export(PropertyHint.Layers3DPhysics)] public int CollisionLayers { get;set; } = 1;
    [Export(PropertyHint.Layers3DPhysics)] public int CollisionMask { get;set; } = 1;
    [Export] public int LODLevels { get;set; } = 8;
    [Export] public int LODRowsPerLevel { get;set; } = 21;
    [Export] public float LODInitialCellWidth { get;set; } = 1;
    [Export] public bool CollisionOnly { get;set; } = false;
    [Export] public bool CreateCollisionInThread { get;set; } = true;

    public StaticBody3D TerrainCollider => _terrainCollider;
    public Clipmap Clipmap => _clipmap;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        BuildTerrain();
    }

    public void BuildTerrain() {
        if (_clipmap == null) {
            return;
        }

        if (CustomShader == null) {
            _clipmap.Shader = new ShaderMaterial() {
                Shader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader.gdshader")
            };
        } else {
            _clipmap.Shader = Utils.CreateCustomShaderCopy(CustomShader);
        }

        _clipmap.ClipmapMesh.Layers = (uint) VisualInstanceLayers;

        _terrainCollider.CollisionLayer = (uint) CollisionLayers;
        _terrainCollider.CollisionMask = (uint) CollisionMask;

        if (!Engine.IsEditorHint() && (CollisionOnly || DefaultSettings.CollisionOnly)) {
            UpdateCollisionShape();
            _clipmap.ClipmapMesh.Visible = false;
        } else {
            _clipmap.ZonesSize = ZonesSize;
            _clipmap.TerrainZones = TerrainZones;
            _clipmap.Levels = LODLevels;
            _clipmap.RowsPerLevel = LODRowsPerLevel;
            _clipmap.InitialCellWidth = LODInitialCellWidth;

            _clipmap.CreateMesh();

            if (Engine.IsEditorHint()) {
                Clipmap.Shader.SetShaderParameter(StringNames.ApplyLockTextures, true);
                Clipmap.Shader.SetShaderParameter(StringNames.LockTextures, TerrainZones.LockTextures);
            }

            TerrainUpdated();
            TerrainTextureUpdated();
            TerrainWaterUpdated();
        }
    }

    public void TerrainUpdated() {
        UpdateCollisionShape();
    }

    private void TerrainTextureUpdated() {
        UpdateTextures();
        TerrainSplatmapsUpdated();
    }

    private void TerrainSplatmapsUpdated() {
        Clipmap.Shader.SetShaderParameter(StringNames.Splatmaps, TerrainZones.SplatmapsTextures);
    }

    public void TerrainWaterUpdated() {
    	Clipmap.Shader.SetShaderParameter(StringNames.WaterTextures, TerrainZones.WaterTextures);
    	Clipmap.Shader.SetShaderParameter(StringNames.WaterFactor, WaterFactor);
    }

	private void UpdateCollisionShape() {
        if (CreateCollisionInThread) {
            _collisionCancellationSource?.Cancel();
            _collisionCancellationSource = new CancellationTokenSource();
        }

        var token = CreateCollisionInThread ? _collisionCancellationSource.Token : CancellationToken.None;

        foreach (var collisionShape in _terrainCollider.GetChildren()) {
            collisionShape.QueueFree();
        }

        var shapes = new List<HeightMapShape3D>();
        foreach (var zone in TerrainZones.Zones) {
            var heightMapShape3D = AddZoneCollision(zone);

            shapes.Add(heightMapShape3D);
        }

        // The calculation for the resolution is painful but it works for now ☠️...
        // TODO : Make it a simpler process...
        var updateAction = () => {
            var imagesCache = new Dictionary<ZoneResource, CollisionZoneImages>();

            for (var i = 0; i < TerrainZones.Zones.Length; i++) {
                var zone = TerrainZones.Zones[i];
                var leftNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X - 1 && x.ZonePosition.Y == zone.ZonePosition.Y);
                var rightNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X + 1 && x.ZonePosition.Y == zone.ZonePosition.Y);
                var topLeftNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X - 1 && x.ZonePosition.Y == zone.ZonePosition.Y - 1);
                var topRightNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X + 1 && x.ZonePosition.Y == zone.ZonePosition.Y - 1);
                var topNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X && x.ZonePosition.Y == zone.ZonePosition.Y - 1);
                var bottomNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X && x.ZonePosition.Y == zone.ZonePosition.Y + 1);
                var bottomLeftNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X - 1 && x.ZonePosition.Y == zone.ZonePosition.Y + 1);
                var bottomRightNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X + 1 && x.ZonePosition.Y == zone.ZonePosition.Y + 1);

                var heightMapImage = zone.HeightMapTexture.GetImage();
                var waterImage = zone.WaterTexture?.GetImage();

                var imageWidth = heightMapImage.GetWidth();
                var imageHeight = heightMapImage.GetHeight();

                if (token.IsCancellationRequested) {
                    return;
                }

                var terrainData = new List<float>();

                // Add one extra line for non 1 resolution
                if (Resolution != 1 && zone.ZonePosition.Y % 2 != 0) {
                    if (zone.ZonePosition.X % 2 != 0) {
                        if (topLeftNeighbourZone != null) {
                            var extraPixelHeight = GetHeightForZone(topLeftNeighbourZone, imageWidth - 1, imageHeight - 1, imagesCache);
                            terrainData.Add(extraPixelHeight);
                        } else {
                            terrainData.Add(GetHeightForZone(zone, 0, 0, imagesCache));
                        }
                    }

                    for (var x = 0; x < imageWidth; x++) {
                        if (topNeighbourZone != null) {
                            if (zone.ZonePosition.X % 2 != 0) {
                                if (x < imageWidth - 1 && topNeighbourZone != null) {
                                    var averagePixelHeight = (GetHeightForZone(topNeighbourZone, x, imageWidth - 1, imagesCache) + GetHeightForZone(topNeighbourZone, x + 1, imageWidth - 1, imagesCache)) / 2.0f;
                                    terrainData.Add(averagePixelHeight);
                                } else if (topRightNeighbourZone != null) {
                                    terrainData.Add(GetHeightForZone(topRightNeighbourZone, 0, imageWidth - 1, imagesCache));
                                } else if (topNeighbourZone != null) {
                                    terrainData.Add(GetHeightForZone(topNeighbourZone, x, imageWidth - 1, imagesCache));
                                } else {
                                    terrainData.Add(GetHeightForZone(zone, x, imageWidth - 1, imagesCache));
                                }
                            } else {
                                var extraPixelHeight = GetHeightForZone(topNeighbourZone, x, imageHeight - 1, imagesCache);
                                terrainData.Add(extraPixelHeight);
                            }
                        } else {
                            terrainData.Add(GetHeightForZone(zone, x, 0, imagesCache));
                        }
                    }
                }

                for (var y = 0; y < imageHeight; y++) {
                    for (var x = 0; x < imageWidth; x++) {
                        if (token.IsCancellationRequested) {
                            return;
                        }

                        if (Resolution == 1) {
                            var currentZone = zone;
                            var lookupX = x;
                            var lookupY = y;

                            if (x == 0 && leftNeighbourZone != null) {
                                currentZone = leftNeighbourZone;
                                lookupX = imageWidth - 1;
                            } else if (y == 0 && topNeighbourZone != null) {
                                currentZone = topNeighbourZone;
                                lookupY = imageHeight - 1;
                            }

                            var pixelHeight = GetHeightForZone(currentZone, lookupX, lookupY, imagesCache);
                            terrainData.Add(pixelHeight);
                        } else if (zone.ZonePosition.X % 2 != 0 && zone.ZonePosition.Y % 2 != 0) { // Support for multizone when the resolution is not 1
                            if (x == 0) {
                                if (y < imageHeight - 1) {
                                    if (leftNeighbourZone != null) {
                                        var averagePixelHeight = (GetHeightForZone(leftNeighbourZone, imageWidth - 1, y, imagesCache) + GetHeightForZone(leftNeighbourZone, imageWidth - 1, y + 1, imagesCache)) / 2.0f;
                                        terrainData.Add(averagePixelHeight);
                                    } else {
                                        var averagePixelHeight = (GetHeightForZone(zone, x, y, imagesCache) + GetHeightForZone(zone, x, y + 1, imagesCache)) / 2.0f;
                                        terrainData.Add(averagePixelHeight);
                                    }
                                } else if (y == imageHeight - 1 && bottomLeftNeighbourZone != null) {
                                    terrainData.Add(GetHeightForZone(bottomLeftNeighbourZone, imageWidth - 1, 0, imagesCache));
                                } else if (leftNeighbourZone != null && bottomNeighbourZone != null) {
                                    var averageForPixel = (GetHeightForZone(leftNeighbourZone, imageWidth - 1, imageHeight - 1, imagesCache) + GetHeightForZone(bottomNeighbourZone, 0, 0, imagesCache)) / 2.0f;
                                    terrainData.Add(averageForPixel);
                                } else {
                                    terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                                }
                            }

                            if (x == imageWidth - 1) {
                                if (y < imageHeight - 1) {
                                    if (rightNeighbourZone != null) {
                                        var averagePixelHeight = (GetHeightForZone(rightNeighbourZone, 0, y, imagesCache) + GetHeightForZone(rightNeighbourZone, 0, y + 1, imagesCache)) / 2.0f;
                                        terrainData.Add(averagePixelHeight);
                                    } else {
                                        var averagePixelHeight = (GetHeightForZone(zone, x, y, imagesCache) + GetHeightForZone(zone, x, y + 1, imagesCache)) / 2.0f;
                                        terrainData.Add(averagePixelHeight);
                                    }
                                } else if (y == imageHeight - 1 && bottomRightNeighbourZone != null) {
                                    terrainData.Add(GetHeightForZone(bottomRightNeighbourZone, 0, 0, imagesCache));
                                } else if (rightNeighbourZone != null && bottomNeighbourZone != null) {
                                    var averageForPixel = (GetHeightForZone(rightNeighbourZone, 0, imageHeight - 1, imagesCache) + GetHeightForZone(bottomNeighbourZone, imageWidth - 1, 0, imagesCache)) / 2.0f;
                                    terrainData.Add(averageForPixel);
                                } else {
                                    terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                                }
                            } else if (y < imageHeight - 1) {
                                var averageForPixel = (
                                    GetHeightForZone(zone, x, y, imagesCache) +
                                    GetHeightForZone(zone, x + 1, y, imagesCache) +
                                    GetHeightForZone(zone, x, y + 1, imagesCache) +
                                    GetHeightForZone(zone, x + 1, y + 1, imagesCache)
                                ) / 4.0f;

                                terrainData.Add(averageForPixel);
                            } else if (y == imageHeight - 1) {
                                if (bottomNeighbourZone != null) {
                                    var averagePixelHeight = (GetHeightForZone(bottomNeighbourZone, x, 0, imagesCache) + GetHeightForZone(bottomNeighbourZone, x + 1, 0, imagesCache)) / 2.0f;
                                    terrainData.Add(averagePixelHeight);
                                } else {
                                    var averagePixelHeight = (GetHeightForZone(zone, x, y, imagesCache) + GetHeightForZone(zone, x + 1, y, imagesCache)) / 2.0f;
                                    terrainData.Add(averagePixelHeight);
                                }
                            } else {
                                terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                            }
                        } else if (zone.ZonePosition.X % 2 != 0) { // Support for multizone when the resolution is not 1
                            // Add it twice since we have one extra column when the resolution is not 1 on odd zones
                            if (x == 0) {
                                if (leftNeighbourZone != null) {
                                    var extraPixelHeight = GetHeightForZone(leftNeighbourZone, imageWidth - 1, y, imagesCache);
                                    terrainData.Add(extraPixelHeight);
                                } else {
                                    terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                                }
                            }

                            if (x < heightMapImage.GetWidth() - 1) {
                                var averagePixelHeight = (GetHeightForZone(zone, x, y, imagesCache) + GetHeightForZone(zone, x + 1, y, imagesCache)) / 2.0f;
                                terrainData.Add(averagePixelHeight);
                            } else if (rightNeighbourZone != null) {
                                terrainData.Add(GetHeightForZone(rightNeighbourZone, 0, y, imagesCache));
                            } else {
                                terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                            }
                        } else if (zone.ZonePosition.Y % 2 != 0) {  // Support for multizone when the resolution is not 1
                            if (y < imageHeight - 1) {
                                var averagePixelHeight = (GetHeightForZone(zone, x, y, imagesCache) + GetHeightForZone(zone, x, y + 1, imagesCache)) / 2.0f;
                                terrainData.Add(averagePixelHeight);
                            } else if (bottomNeighbourZone != null) {
                                terrainData.Add(GetHeightForZone(bottomNeighbourZone, x, 0, imagesCache));
                            } else {
                                terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                            }
                        } else {
                            terrainData.Add(GetHeightForZone(zone, x, y, imagesCache));
                        }
                    }
                }

                if (token.IsCancellationRequested) {
                    return;
                }

                CallDeferred(nameof(AssignCollisionData), shapes[i], terrainData.ToArray());
            }
        };

        if (CreateCollisionInThread) {
            Task.Factory.StartNew(() => {
                updateAction();
            }, token);
        } else {
            updateAction();
        }
	}

    private void AssignCollisionData(HeightMapShape3D shape, float[] data) {
        shape.MapData = data;
    }

    public HeightMapShape3D AddZoneCollision(ZoneResource zone) {
        var collisionShape = new CollisionShape3D();
        _terrainCollider.AddChild(collisionShape);

        collisionShape.Position = new Vector3((ZonesSize - 1) * zone.ZonePosition.X, 0, (ZonesSize - 1) * zone.ZonePosition.Y);

        var heightMapShape3D = new HeightMapShape3D();
        collisionShape.Shape = heightMapShape3D;

        var size = ZoneUtils.GetImageSizeForResolution(ZonesSize, Resolution);

        heightMapShape3D.MapWidth = size;
        heightMapShape3D.MapDepth = size;

        // Add one extra row/col if the resolution is not 1
        if (Resolution != 1) {
            collisionShape.Scale = new Vector3(Resolution, 1.0f, Resolution);

            var offsetPosition = Vector3.Zero;
            if (zone.ZonePosition.X % 2 != 0) {
                heightMapShape3D.MapWidth += 1;
            }

            if (zone.ZonePosition.Y % 2 != 0) {
                heightMapShape3D.MapDepth += 1;
            }

            collisionShape.Position += offsetPosition;
        }

        return heightMapShape3D;
    }

	private void UpdateTextures() {
		Clipmap.Shader.SetShaderParameter(StringNames.NearestFilter, NearestTextureFilter);

        var filterParamName = string.Empty;
        if (NearestTextureFilter) {
            filterParamName = "Nearest";
        }

        if (this.TextureSets?.TextureSets?.Length > 0) {
            var textureArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.AlbedoTexture));
            Clipmap.Shader.SetShaderParameter(StringNames.TexturesDetail, TextureSets.TextureSets.Select(x => x.TextureDetail <= 0 ? TextureDetail : x.TextureDetail).ToArray());
            Clipmap.Shader.SetShaderParameter($"Textures{filterParamName}", textureArray);
            Clipmap.Shader.SetShaderParameter(StringNames.NumberOfTextures, textureArray.GetLayers());

            if (this.TextureSets.TextureSets.Any(x => x.NormalTexture != null)) {
                var normalArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.NormalTexture));
                Clipmap.Shader.SetShaderParameter($"Normals{filterParamName}", normalArray);
                Clipmap.Shader.SetShaderParameter(StringNames.HasNormalTextures, true);
            }

            if (this.TextureSets.TextureSets.Any(x => x.RoughnessTexture != null)) {
                var roughnessArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.RoughnessTexture));
                Clipmap.Shader.SetShaderParameter($"RoughnessTextures{filterParamName}", roughnessArray);
                Clipmap.Shader.SetShaderParameter(StringNames.HasRoughnessTextures, true);
            }

            if (this.TextureSets.TextureSets.Any(x => x.HeightTexture != null)) {
                var heightArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.HeightTexture));
                Clipmap.Shader.SetShaderParameter($"HeightTextures{filterParamName}", heightArray);
                Clipmap.Shader.SetShaderParameter(StringNames.HasHeightTextures, true);
            }

            Clipmap.Shader.SetShaderParameter(StringNames.UseAntitile, UseAntiTile);
            Clipmap.Shader.SetShaderParameter(StringNames.BlendFactor, HeightBlendFactor);
            Clipmap.Shader.SetShaderParameter(StringNames.AlbedoAlphaChannelUsage, (int) AlbedoAlphaChannelUsage);
            Clipmap.Shader.SetShaderParameter(StringNames.NormalAlphaChannelUsage, (int) NormalAlphaChannelUsage);
        } else if (DefaultTexture != null) {
            var textureArray = Utils.TexturesToTextureArray(new Texture2D[] {DefaultTexture});
            Clipmap.Shader.SetShaderParameter(StringNames.TexturesDetail, new int[] {TextureDetail});
            Clipmap.Shader.SetShaderParameter($"Textures{filterParamName}", textureArray);
            Clipmap.Shader.SetShaderParameter(StringNames.NumberOfTextures, textureArray.GetLayers());
            Clipmap.Shader.SetShaderParameter(StringNames.UseAntitile, false);
        }
	}

    private float GetHeightForZone(ZoneResource zone, int x, int y, Dictionary<ZoneResource, CollisionZoneImages> imagesCache) {
        CollisionZoneImages zoneImages;
        if (imagesCache.ContainsKey(zone)) {
            zoneImages = imagesCache[zone];
        } else {
            zoneImages = new CollisionZoneImages() {
                HeightmapImage = zone.HeightMapTexture.GetImage(),
                WaterImage = zone.WaterTexture?.GetImage()
            };
            imagesCache.Add(zone, zoneImages);
        }

        var pixel = zoneImages.HeightmapImage.GetPixel(x, y);
        if (pixel.G > 0.0f) {
            return HoleValue;
        }

        var pixelHeight = pixel.R * HeightMapFactor;
        var waterHeight = zoneImages.WaterImage?.GetPixel(x, y).R ?? 0;
        pixelHeight -= waterHeight * WaterFactor;

        return pixelHeight;
    }

    private class CollisionZoneImages {
        public Image HeightmapImage { get;set; }
        public Image WaterImage { get;set; }
    }
}
