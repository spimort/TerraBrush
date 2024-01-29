using Godot;

namespace TerraBrush;

public class FoliageTool : ToolBase {
    protected override ImageTexture GetToolCurrentImageTexture(TerraBrush terraBrush, ZoneResource zone) {
        return zone.FoliagesTexture[terraBrush.FoliageIndex.Value];
    }

    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.FoliageIndex == null) {
            return;
        }

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
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

        terraBrush.TerrainZones.UpdateFoliagesTextures();
    }
}
