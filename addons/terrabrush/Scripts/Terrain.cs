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
    private CancellationTokenSource _collisionCancellationSource = null;

    [NodePath] private Clipmap _clipmap;
    [NodePath] private StaticBody3D _terrainCollider;

    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public bool UseAntiTile { get;set; } = true;
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
    public Clipmap Clipmap => _clipmap;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }

    public void BuildTerrain(bool collisionOnly = false) {
        if (_clipmap == null) {
            return;
        }

        _clipmap.ClipmapMesh.Layers = (uint) VisualInstanceLayers;

        _terrainCollider.CollisionLayer = (uint) CollisionLayers;
        _terrainCollider.CollisionMask = (uint) CollisionMask;

        if (collisionOnly) {
            UpdateCollisionShape();
            _clipmap.ClipmapMesh.Visible = false;
        } else {
            _clipmap.ZonesSize = ZonesSize;
            _clipmap.TerrainZones = TerrainZones;
            _clipmap.Levels = LODLevels;
            _clipmap.RowsPerLevel = LODRowsPerLevel;
            _clipmap.InitialCellWidth = LODInitialCellWidth;

            _clipmap.CreateMesh();

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
        Clipmap.Shader.SetShaderParameter("Splatmaps", TerrainZones.SplatmapsTextures);
    }

    public void TerrainWaterUpdated() {
    	Clipmap.Shader.SetShaderParameter("WaterTextures", TerrainZones.WaterTextures);
    	Clipmap.Shader.SetShaderParameter("WaterFactor", WaterFactor);
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

        var updateAction = () => {
            var imagesCache = new Dictionary<ZoneResource, CollisionZoneImages>();

            for (var i = 0; i < TerrainZones.Zones.Length; i++) {
                var zone = TerrainZones.Zones[i];
                var leftNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X - 1 && x.ZonePosition.Y == zone.ZonePosition.Y);
                var topNeighbourZone = TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == zone.ZonePosition.X && x.ZonePosition.Y == zone.ZonePosition.Y - 1);

                var heightMapImage = zone.HeightMapTexture.GetImage();
                var waterImage = zone.WaterTexture?.GetImage();

                if (token.IsCancellationRequested) {
                    return;
                }

                var terrainData = new List<float>();
                for (var y = 0; y < heightMapImage.GetHeight(); y++) {
                    for (var x = 0; x < heightMapImage.GetWidth(); x++) {
                        if (token.IsCancellationRequested) {
                            return;
                        }

                        var currentZone = zone;
                        var lookupX = x;
                        var loopupY = y;
                        if (x == 0 && leftNeighbourZone != null) {
                            currentZone = leftNeighbourZone;
                            lookupX = heightMapImage.GetWidth() - 1;
                        } else if (y == 0 && topNeighbourZone != null) {
                            currentZone = topNeighbourZone;
                            loopupY = heightMapImage.GetHeight() - 1;
                        }

                        var pixelHeight = GetHeightForZone(currentZone, lookupX, loopupY, imagesCache);
                        terrainData.Add(pixelHeight);
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
        heightMapShape3D.MapWidth = ZonesSize;
        heightMapShape3D.MapDepth = ZonesSize;

        return heightMapShape3D;
    }

	private void UpdateTextures() {
		Clipmap.Shader.SetShaderParameter("TextureDetail", this.TextureDetail);

        if (this.TextureSets?.TextureSets?.Count() > 0) {
            var textureArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.AlbedoTexture));
            Clipmap.Shader.SetShaderParameter("Textures", textureArray);
            Clipmap.Shader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            var normalArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.NormalTexture));
            Clipmap.Shader.SetShaderParameter("Normals", normalArray);

            var roughnessArray = Utils.TexturesToTextureArray(this.TextureSets.TextureSets.Select(x => x.RoughnessTexture));
            Clipmap.Shader.SetShaderParameter("RoughnessTexutres", roughnessArray);

            Clipmap.Shader.SetShaderParameter("UseAntitile", UseAntiTile);
        } else if (DefaultTexture != null) {
            var textureArray = Utils.TexturesToTextureArray(new Texture2D[] {DefaultTexture});
            Clipmap.Shader.SetShaderParameter("Textures", textureArray);
            Clipmap.Shader.SetShaderParameter("NumberOfTextures", textureArray.GetLayers());

            Clipmap.Shader.SetShaderParameter("UseAntitile", false);
        }
	}

    private float GetHeightForZone(ZoneResource zone, int x, int y, Dictionary<ZoneResource, CollisionZoneImages> imagesCache) {
        CollisionZoneImages zoneImages = null;
        if (imagesCache.ContainsKey(zone)) {
            zoneImages = imagesCache[zone];
        } else {
            zoneImages = new CollisionZoneImages() {
                HeightmapImage = zone.HeightMapTexture.GetImage(),
                WaterImage = zone.WaterTexture?.GetImage()
            };
            imagesCache.Add(zone, zoneImages);
        }

        var pixelHeight = zoneImages.HeightmapImage.GetPixel(x, y).R * HeightMapFactor;
        var waterHeight = zoneImages.WaterImage?.GetPixel(x, y).R ?? 0;
        pixelHeight -= waterHeight * WaterFactor;

        return pixelHeight;
    }

    private class CollisionZoneImages {
        public Image HeightmapImage { get;set; }
        public Image WaterImage { get;set; }
    }
}
