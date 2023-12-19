using Godot;

namespace TerraBrush;

public class SnowTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.SnowTexture == null) {
            return;
        }

        var snowImage = terraBrush.SnowTexture.GetImage();

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = snowImage.GetPixel(xPosition, yPosition);
            var newColor = toolType == TerrainToolType.SnowAdd ? Colors.Red : new Color(0, 0, 0, 0);

            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
            );
            snowImage.SetPixel(xPosition, yPosition, newValue);
        });

        terraBrush.SnowTexture.Update(snowImage);
    }
}
