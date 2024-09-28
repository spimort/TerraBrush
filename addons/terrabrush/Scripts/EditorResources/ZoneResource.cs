using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ZoneResource : Resource {
    public ImageTexture LockTexture { get;set; }
    [Export] public Vector2I ZonePosition { get;set; }
    [Export] public ImageTexture HeightMapTexture { get;set; }
    [Export] public ImageTexture[] SplatmapsTexture { get;set; }
    [Export] public ImageTexture[] FoliagesTexture { get;set; }
    [Export] public ImageTexture[] ObjectsTexture { get;set; }
    [Export] public ImageTexture WaterTexture { get;set; }
    [Export] public ImageTexture SnowTexture { get;set; }

    public void InitializeImagesForTerrain(TerraBrush terraBrush) {
        HeightMapTexture = ZoneUtils.CreateHeightmapImage(terraBrush.ZonesSize, ZonePosition, terraBrush.DataPath);

        var numberOfSplatmaps = Mathf.CeilToInt((terraBrush.TextureSets?.TextureSets?.Length ?? 0) / 4.0f);
        var splatmaps = new List<ImageTexture>();
        for (var i = 0; i < numberOfSplatmaps; i++) {
            splatmaps.Add(ZoneUtils.CreateSplatmapImage(terraBrush.ZonesSize, ZonePosition, i, terraBrush.DataPath));
        }
        SplatmapsTexture = splatmaps.ToArray();

        if (terraBrush.Foliages != null) {
            FoliagesTexture = terraBrush.Foliages.Select((foliage, index) => {
                return ZoneUtils.CreateFoliageImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
            }).ToArray();
        }

        if (terraBrush.Objects != null) {
            ObjectsTexture = terraBrush.Objects.Select((objectItem, index) => {
                return ZoneUtils.CreateObjectImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
            }).ToArray();
        }

        if (terraBrush.WaterDefinition != null) {
            WaterTexture = ZoneUtils.CreateWaterImage(terraBrush.ZonesSize, ZonePosition, terraBrush.DataPath);
        }

        if (terraBrush.SnowDefinition != null) {
            SnowTexture = ZoneUtils.CreateSnowImage(terraBrush.ZonesSize, ZonePosition, terraBrush.DataPath);
        }
    }
}
