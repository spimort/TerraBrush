using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ZonesResource : Resource {
    private Texture2DArray _heightmapTextures = new Texture2DArray();
    private Texture2DArray _splatmapsTextures = new Texture2DArray();
    private Texture2DArray _foliagesTextures = new Texture2DArray();
    private Texture2DArray _objectsTextures = new Texture2DArray();
    private Texture2DArray _waterTextures = new Texture2DArray();
    private Texture2DArray _snowTextures = new Texture2DArray();

    public Texture2DArray HeightmapTextures => _heightmapTextures;
    public Texture2DArray SplatmapsTextures => _splatmapsTextures;
    public Texture2DArray FoliagesTextures => _foliagesTextures;
    public Texture2DArray ObjectsTextures => _objectsTextures;
    public Texture2DArray WaterTextures => _waterTextures;
    public Texture2DArray SnowTextures => _snowTextures;

    [Export] public ZoneResource[] Zones { get;set; }

    public void UpdateHeightmaps() {
        _heightmapTextures.CreateFromImages(new Godot.Collections.Array<Image>(Zones.Select(zone => zone.HeightMapTexture.GetImage())));
    }

    public void UpdateSplatmapsTextures() {
        _splatmapsTextures.CreateFromImages(new Godot.Collections.Array<Image>(
            Zones.Aggregate(new List<Image>(), (source, zone) => {
                if (zone.SplatmapsTexture != null) {
                    source.AddRange(zone.SplatmapsTexture.Select(texture => texture.GetImage()));
                }

                return source;
            })
        ));
    }

    public void UpdateFoliagesTextures() {
        _foliagesTextures.CreateFromImages(new Godot.Collections.Array<Image>(
            Zones.Aggregate(new List<Image>(), (source, zone) => {
                if (zone.FoliagesTexture != null) {
                    source.AddRange(zone.FoliagesTexture.Select(texture => texture.GetImage()));
                }

                return source;
            })
        ));
    }

    public void UpdateObjectsTextures() {
        _objectsTextures.CreateFromImages(new Godot.Collections.Array<Image>(
            Zones.Aggregate(new List<Image>(), (source, zone) => {
                if (zone.ObjectsTexture != null) {
                    source.AddRange(zone.ObjectsTexture.Select(texture => texture.GetImage()));
                }

                return source;
            })
        ));
    }

    public void UpdateWaterTextures() {
        _waterTextures.CreateFromImages(new Godot.Collections.Array<Image>(Zones.Select(zone => zone.WaterTexture.GetImage())));
    }

    public void UpdateSnowTextures() {
        _snowTextures.CreateFromImages(new Godot.Collections.Array<Image>(Zones.Select(zone => zone.SnowTexture.GetImage())));
    }

    public void SaveResources() {
        if (Zones == null) {
            return;
        }

        foreach (var zone in Zones) {
            if (zone.HeightMapTexture != null) {
                SaveImageResource(zone.HeightMapTexture);
            }

            if (zone.WaterTexture != null) {
                SaveImageResource(zone.WaterTexture);
            }

            if (zone.SnowTexture != null) {
                SaveImageResource(zone.SnowTexture);
            }

            if (zone.SplatmapsTexture != null) {
                foreach (var splatMap in zone.SplatmapsTexture) {
                    SaveImageResource(splatMap);
                }
            }

            if (zone.FoliagesTexture != null) {
                foreach (var foliageTexture in zone.FoliagesTexture) {
                    SaveImageResource(foliageTexture);
                }
            }

            if (zone.ObjectsTexture != null) {
                foreach (var objectItemTexture in zone.ObjectsTexture) {
                    SaveImageResource(objectItemTexture);
                }
            }
        }
    }

    private void SaveImageResource(ImageTexture image) {
        if (!string.IsNullOrWhiteSpace(image.ResourcePath) && FileAccess.FileExists(image.ResourcePath)) {
            ResourceSaver.Save(image, image.ResourcePath);
        }
    }
}
