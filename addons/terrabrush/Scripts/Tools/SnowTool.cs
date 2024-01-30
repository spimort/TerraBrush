using Godot;

namespace TerraBrush;

public class SnowTool : ToolBase {
    protected override ImageTexture GetToolCurrentImageTexture(TerraBrush terraBrush, ZoneResource zone) {
        return zone.SnowTexture;
    }

    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.SnowDefinition == null) {
            return;
        }

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newColor = toolType == TerrainToolType.SnowAdd ? Colors.Red : new Color(0, 0, 0, 0);

            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
            );
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
        });

        terraBrush.TerrainZones.UpdateSnowTextures();
    }
}
