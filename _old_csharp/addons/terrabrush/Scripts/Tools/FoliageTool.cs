#if TOOLS
using Godot;

namespace TerraBrush;

public class FoliageTool : ToolBase {
    public FoliageTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.FoliagesTexture[_terraBrush.FoliageIndex.Value];
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.FoliageIndex == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newColor = toolType == TerrainToolType.FoliageAdd ? Colors.Red : new Color(0, 0, 0, 0);

            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
            );
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
        });

        _terraBrush.TerrainZones.UpdateFoliagesTextures(_terraBrush.FoliageIndex.Value);
    }
}
#endif
