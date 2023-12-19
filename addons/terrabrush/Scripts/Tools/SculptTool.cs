using Godot;
using TerraBrush;

namespace TerraBrush;

public class SculptTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        var heightMapImage = terraBrush.HeightMap.GetImage();

        if (toolType == TerrainToolType.TerrainSmooth) {
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
        } else {
            Sculpt(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
        }

        terraBrush.HeightMap.Update(heightMapImage);
        terraBrush.UpdateObjectsHeight();
    }

    private void Sculpt(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition, Image heightMapImage) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = heightMapImage.GetPixel(xPosition, yPosition);
            var newValue = Colors.Red * (pixelBrushStrength * brushStrength);
            if (toolType == TerrainToolType.TerrainAdd) {
                newValue = currentPixel + newValue;
            } else if (toolType == TerrainToolType.TerrainRemove) {
                newValue = currentPixel - newValue;
            }

            heightMapImage.SetPixel(xPosition, yPosition, newValue);
        });
    }

    private void Smooth(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition, Image heightMapImage) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = heightMapImage.GetPixel(xPosition, yPosition);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = heightMapImage.GetPixel(xPosition, yPosition);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, smoothValue.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, smoothValue.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, smoothValue.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, smoothValue.A, pixelBrushStrength * brushStrength)
            );

            heightMapImage.SetPixel(xPosition, yPosition, newValue);
        });
    }
}
