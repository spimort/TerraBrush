using System.Collections.Generic;
using System.Linq;
using Godot;
using Godot.Collections;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ZoneResource : Resource {
    public ImageTexture LockTexture { get;set; }
    [BindProperty] public Vector2I ZonePosition { get;set; }
    [BindProperty] public ImageTexture HeightMapTexture { get;set; }
    [BindProperty] public GodotArray<ImageTexture> SplatmapsTexture { get;set; }
    [BindProperty] public GodotArray<ImageTexture> FoliagesTexture { get;set; }
    [BindProperty] public GodotArray<ImageTexture> ObjectsTexture { get;set; }
    [BindProperty] public ImageTexture WaterTexture { get;set; }
    [BindProperty] public ImageTexture SnowTexture { get;set; }
    [BindProperty] public ImageTexture MetaInfoTexture { get;set; }

    public void InitializeImagesForTerrain(TerraBrush terraBrush) {
        HeightMapTexture = ZoneUtils.CreateHeightmapImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);

        var numberOfSplatmaps = Mathf.CeilToInt((terraBrush.TextureSets?.TextureSets?.Count ?? 0) / 4.0f);
        var splatmaps = new GodotArray<ImageTexture>();
        for (var i = 0; i < numberOfSplatmaps; i++) {
            splatmaps.Add(ZoneUtils.CreateSplatmapImage(terraBrush.ZonesSize, ZonePosition, i, terraBrush.DataPath));
        }
        SplatmapsTexture = splatmaps;

        if (terraBrush.Foliages != null) {
            FoliagesTexture = new GodotArray<ImageTexture>([..terraBrush.Foliages.Select((foliage, index) => {
                return ZoneUtils.CreateFoliageImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
            })]);
        }

        if (terraBrush.Objects != null) {
            ObjectsTexture = new GodotArray<ImageTexture>([..terraBrush.Objects.Select((objectItem, index) => {
                return ZoneUtils.CreateObjectImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
            })]);
        }

        if (terraBrush.WaterDefinition != null) {
            WaterTexture = ZoneUtils.CreateWaterImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
        }

        if (terraBrush.SnowDefinition != null) {
            SnowTexture = ZoneUtils.CreateSnowImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
        }

        if (terraBrush.MetaInfoLayers?.Count > 0) {
            MetaInfoTexture = ZoneUtils.CreateMetaInfoImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
        }
    }
}
