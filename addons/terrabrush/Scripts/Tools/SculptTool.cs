using System;
using System.Collections.Generic;
using System.Linq;
using Godot;
using TerraBrush;

namespace TerraBrush;

public class SculptTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {

        // GD.Print("Image Position ", imagePosition, " XZone ", zoneXPosition, " YZone", zoneYPosition, " Brush X ", zoneBrushXPosition, " Brush y ", zoneBrushYPosition);


        // var heightMapImage = terraBrush.HeightMap.GetImage();

        if (toolType == TerrainToolType.TerrainSmooth) {
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else if (toolType == TerrainToolType.TerrainFlattern) {
            Flattern(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else {
            Sculpt(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
            // Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        }

        terraBrush.TerrainZones.UpdateHeightmaps();
        // terraBrush.HeightMap.Update(heightMapImage);
        // terraBrush.UpdateObjectsHeight();
    }

    private void Sculpt(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, xPosition, yPosition, imagesCache) => {
            var currentPixel = image.GetPixel(xPosition, yPosition);
            var newValue = Colors.Red * (pixelBrushStrength * brushStrength);
            if (toolType == TerrainToolType.TerrainAdd) {
                newValue = currentPixel + newValue;
            } else if (toolType == TerrainToolType.TerrainRemove) {
                newValue = currentPixel - newValue;
            }

            image.SetPixel(xPosition, yPosition, newValue);
        });
    }

    private void Flattern(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, xPosition, yPosition, imagesCache) => {
            var currentPixel = image.GetPixel(xPosition, yPosition);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, xPosition, yPosition, imagesCache) => {
            var currentPixel = image.GetPixel(xPosition, yPosition);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, smoothValue.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, smoothValue.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, smoothValue.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, smoothValue.A, pixelBrushStrength * brushStrength)
            );

            image.SetPixel(xPosition, yPosition, newValue);
        });
    }

    private void Smooth(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, xPosition, yPosition, imagesCache) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition - 1, yPosition, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition + 1, yPosition, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition, yPosition - 1, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition, yPosition + 1, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            var currentPixel = image.GetPixel(xPosition, yPosition).R;
            directions.Add(currentPixel);

            float average = directions.Average();
            float resultValue = Mathf.Lerp(currentPixel, average, pixelBrushStrength * brushStrength);

            image.SetPixel(xPosition, yPosition, new Color(resultValue, 0, 0, 1.0f));
        });
    }
}
