using Godot;

namespace TerraBrush;

public partial class MetaInfoTool : ToolBase {
    public MetaInfoTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.MetaInfoTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.MetaInfoIndex == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            if (pixelBrushStrength > 0.0) {
                var layerValue = toolType == TerrainToolType.MetaInfoAdd ? _terraBrush.MetaInfoIndex.Value : -1;

                var newValue = layerValue;
                imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, new Color(layerValue, 0, 0, 0));
            }
        }, true);

        _terraBrush.TerrainZones.UpdateMetaInfoTextures();
    }
}
