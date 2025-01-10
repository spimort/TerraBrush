using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ZonesResource : Resource {
    private HashSet<ImageTexture> _dirtyImageTextures = new();

    private Texture2DArray _lockTextures = new();
    private Texture2DArray _heightmapTextures = new();
    private Texture2DArray _splatmapsTextures = new();
    private Texture2DArray[] _foliagesTextures;
    private Texture2DArray _objectsTextures = new();
    private Texture2DArray _waterTextures = new();
    private Texture2DArray _snowTextures = new();
    private ImageTexture _zonesMap = new();

    public Texture2DArray LockTextures => _lockTextures;
    public Texture2DArray HeightmapTextures => _heightmapTextures;
    public Texture2DArray SplatmapsTextures => _splatmapsTextures;
    public Texture2DArray[] FoliagesTextures => _foliagesTextures;
    public Texture2DArray ObjectsTextures => _objectsTextures;
    public Texture2DArray WaterTextures => _waterTextures;
    public Texture2DArray SnowTextures => _snowTextures;
    public ImageTexture ZonesMap => _zonesMap;

    [Export] public ZoneResource[] Zones { get;set; }

    public void UpdateLockTexture(int zoneSize) {
        var images = Zones.Select(zone => zone.LockTexture?.GetImage() ?? GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rf)).ToArray();

		if (images.Length != 0) {
            _lockTextures.CreateFromImages(new Godot.Collections.Array<Image>(images));
        }
    }

    public void UpdateHeightmaps() {
		var images = Zones.Select(zone => zone.HeightMapTexture.GetImage()).ToArray();
		if (images.Length != 0) {
            _heightmapTextures.CreateFromImages(new Godot.Collections.Array<Image>(images));
        }
    }

    public void UpdateSplatmapsTextures() {
        var images = Zones.Aggregate(new List<Image>(), (source, zone) => {
            if (zone.SplatmapsTexture != null) {
                source.AddRange(zone.SplatmapsTexture.Select(texture => texture.GetImage()));
            }

            return source;
        });

        if (images.Count > 0) {
            _splatmapsTextures.CreateFromImages(new Godot.Collections.Array<Image>(images));
        }
    }

    public void InitializeFoliageTextures(TerraBrush terraBrush) {
        _foliagesTextures = terraBrush.Foliages?.Select(_ => new Texture2DArray()).ToArray();
    }

    public void UpdateFoliagesTextures() {
		if (_foliagesTextures?.Length <= 0) return;

		for (var i = 0; i < _foliagesTextures?.Length; i++) {
			UpdateFoliagesTextures(i);
		}
	}

    public void UpdateFoliagesTextures(int foliageIndex) {
        var images = Zones.Select(zone => zone.FoliagesTexture[foliageIndex].GetImage()).ToArray();

        if (images.Length > 0) {
            _foliagesTextures[foliageIndex].CreateFromImages(new Godot.Collections.Array<Image>(images));
        }
    }

    public void UpdateObjectsTextures() {
        var images = Zones.Aggregate(new List<Image>(), (source, zone) => {
            if (zone.ObjectsTexture != null) {
                source.AddRange(zone.ObjectsTexture.Select(texture => texture.GetImage()));
            }

            return source;
        });

        if (images.Count > 0) {
            _objectsTextures.CreateFromImages(new Godot.Collections.Array<Image>(images));
        }
    }

    public void UpdateWaterTextures() {
        if (Zones.Any(zone => zone.WaterTexture == null)) {
            return;
        }

        _waterTextures.CreateFromImages(new Godot.Collections.Array<Image>(Zones.Select(zone => zone.WaterTexture.GetImage())));
    }

    public void UpdateZoneWaterTexture(ZoneResource zone) {
        _waterTextures.UpdateLayer(zone.WaterTexture.GetImage(), Array.IndexOf(Zones, zone));
    }

    public void UpdateSnowTextures() {
        if (Zones.Any(zone => zone.SnowTexture == null)) {
            return;
        }

        _snowTextures.CreateFromImages(new Godot.Collections.Array<Image>(Zones.Select(zone => zone.SnowTexture.GetImage())));
    }

    public void UpdateZoneSnowTexture(ZoneResource zone) {
        _snowTextures.UpdateLayer(zone.SnowTexture.GetImage(), Array.IndexOf(Zones, zone));
    }

    public void SaveResources() {
        foreach (var dirtyImageResource in _dirtyImageTextures) {
            SaveImageResource(dirtyImageResource);
        }

        _dirtyImageTextures.Clear();
    }

    private void SaveImageResource(ImageTexture image) {
        if (!string.IsNullOrWhiteSpace(image.ResourcePath) && FileAccess.FileExists(image.ResourcePath)) {
            ResourceSaver.Save(image, image.ResourcePath);
        }
    }

    public void UpdateZonesMap() {
        var zonePositions = Zones.Select(zone => zone.ZonePosition).ToArray();
		var maxX = zonePositions.Max(x => Math.Abs(x.X));
		var maxY = zonePositions.Max(x => Math.Abs(x.Y));

		var zonesMap = GodotAgnostic.ImageCreateEmpty((maxX * 2) + 1, (maxY * 2) + 1, false, Image.Format.Rf);
		zonesMap.Fill(new Color(-1, 0, 0, 0));
		for (var i = 0; i < zonePositions.Length; i++) {
			var position = zonePositions[i];
			zonesMap.SetPixel(position.X + maxX, position.Y + maxY, new Color(i, 0, 0, 0));
		}

        _zonesMap.SetImage(zonesMap);
    }

    public ZoneResource AddNewZone(TerraBrush terraBrush, Vector2I zonePosition) {
        var zone = new ZoneResource() {
            ZonePosition = zonePosition
        };

        zone.InitializeImagesForTerrain(terraBrush);

        var newList = new List<ZoneResource>(Zones) {
            zone
        };
        Zones = newList.ToArray();

        UpdateImageTextures(terraBrush.ZonesSize);

        terraBrush.Terrain?.Clipmap?.UpdateAABB();
        terraBrush.Water?.Clipmap?.UpdateAABB();
        terraBrush.Snow?.Clipmap?.UpdateAABB();

        return zone;
    }

    public void AddDirtyImageTexture(ImageTexture imageTexture) {
        _dirtyImageTextures.Add(imageTexture);
    }

    public void UpdateImageTextures(int zoneSize) {
        if (Engine.IsEditorHint()) {
            UpdateLockTexture(zoneSize);
        }
        UpdateHeightmaps();
        UpdateSplatmapsTextures();
        UpdateFoliagesTextures();
        UpdateObjectsTextures();
        UpdateWaterTextures();
        UpdateSnowTextures();
        UpdateZonesMap();
    }

    public ZoneResource GetZoneForZoneInfo(ZoneInfo zoneInfo) {
        return Zones?.FirstOrDefault(x => x.ZonePosition.X == zoneInfo.ZonePosition.X && x.ZonePosition.Y == zoneInfo.ZonePosition.Y);
    }
}
