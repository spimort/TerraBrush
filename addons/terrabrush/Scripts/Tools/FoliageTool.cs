using Godot;

namespace TerraBrush;

public class FoliageTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.FoliageIndex == null) {
            return;
        }

        var currentFoliage = terraBrush.Foliages[terraBrush.FoliageIndex.Value];
        if (currentFoliage.Definition == null) {
            return;
        }

        var foliageImage = currentFoliage.Texture.GetImage();

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = foliageImage.GetPixel(xPosition, yPosition);
            var newColor = toolType == TerrainToolType.FoliageAdd ? Colors.Red : new Color(0, 0, 0, 0);

            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
            );
            foliageImage.SetPixel(xPosition, yPosition, newValue);
        });

        currentFoliage.Texture.Update(foliageImage);
    }
}
