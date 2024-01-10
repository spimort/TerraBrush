using System.Collections.Generic;
using System.Linq;
using Godot;
using TerraBrush;

namespace TerraBrush;

public class SculptTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        var heightMapImage = terraBrush.HeightMap.GetImage();

        if (toolType == TerrainToolType.TerrainSmooth) {
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
        } else if (toolType == TerrainToolType.TerrainFlattern) {
            Flattern(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
        } else {
            Sculpt(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition, heightMapImage);
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

    private void Flattern(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition, Image heightMapImage) {
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

    private void Smooth(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition, Image heightMapImage) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var directions = new List<float>();
            if (xPosition - 1 >= 0) {
                directions.Add(heightMapImage.GetPixel(xPosition - 1, yPosition).R);
            }

            if (xPosition + 1 < terraBrush.TerrainSize) {
                directions.Add(heightMapImage.GetPixel(xPosition + 1, yPosition).R);
            }

            if (yPosition - 1 >= 0) {
                directions.Add(heightMapImage.GetPixel(xPosition, yPosition - 1).R);
            }

            if (yPosition + 1 < terraBrush.TerrainSize) {
                directions.Add(heightMapImage.GetPixel(xPosition, yPosition + 1).R);
            }

            var currentPixel = heightMapImage.GetPixel(xPosition, yPosition).R;
            directions.Add(currentPixel);

            float average = directions.Average();
            float resultValue = Mathf.Lerp(currentPixel, average, pixelBrushStrength * brushStrength);

            heightMapImage.SetPixel(xPosition, yPosition, new Color(resultValue, 0, 0, 1.0f));
        });
    }
}
