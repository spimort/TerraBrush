#if TOOLS
using Godot;

namespace TerraBrush;

public class SnowTool : ToolBase {
    public SnowTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.SnowTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.SnowDefinition == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newColor = toolType == TerrainToolType.SnowAdd ? Colors.Red : new Color(0, 0, 0, 0);

            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                1
            );
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
        });

        _terraBrush.TerrainZones.UpdateSnowTextures();
    }
}
#endif
