using Godot;

namespace TerraBrush;

public class WaterTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.WaterTexture == null) {
            return;
        }

        var waterImage = terraBrush.WaterTexture.GetImage();

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = waterImage.GetPixel(xPosition, yPosition);

            var newColor = toolType == TerrainToolType.WaterAdd ? new Color(1, currentPixel.G, currentPixel.B, 1) : new Color(0, 0.5f, 0.5f, 1);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
            );
            waterImage.SetPixel(xPosition, yPosition, newValue);
        });

        terraBrush.WaterTexture.Update(waterImage);
        terraBrush.UpdateObjectsHeight();
    }
}
