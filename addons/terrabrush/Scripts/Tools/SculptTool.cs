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
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, relativeImagePosition, absoluteImagePosition, imagesCache) => {
            var currentPixel = image.GetPixel(relativeImagePosition.X, relativeImagePosition.Y);
            var newValue = Colors.Red * (pixelBrushStrength * brushStrength);
            if (toolType == TerrainToolType.TerrainAdd) {
                newValue = currentPixel + newValue;
            } else if (toolType == TerrainToolType.TerrainRemove) {
                newValue = currentPixel - newValue;
            }

            image.SetPixel(relativeImagePosition.X, relativeImagePosition.Y, newValue);
        });
    }

    private void Flattern(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, relativeImagePosition, absoluteImagePosition, imagesCache) => {
            var currentPixel = image.GetPixel(relativeImagePosition.X, relativeImagePosition.Y);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, relativeImagePosition, absoluteImagePosition, imagesCache) => {
            var currentPixel = image.GetPixel(relativeImagePosition.X, relativeImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, smoothValue.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, smoothValue.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, smoothValue.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, smoothValue.A, pixelBrushStrength * brushStrength)
            );

            image.SetPixel(relativeImagePosition.X, relativeImagePosition.Y, newValue);
        });
    }

    private void Smooth(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (image, pixelBrushStrength, relativeImagePosition, absoluteImagePosition, imagesCache) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X - 1, absoluteImagePosition.Y, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X + 1, absoluteImagePosition.Y, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X, absoluteImagePosition.Y - 1, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X, absoluteImagePosition.Y + 1, imagesCache);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            var currentPixel = image.GetPixel(relativeImagePosition.X, relativeImagePosition.Y).R;
            directions.Add(currentPixel);

            float average = directions.Average();
            float resultValue = Mathf.Lerp(currentPixel, average, pixelBrushStrength * brushStrength);

            image.SetPixel(relativeImagePosition.X, relativeImagePosition.Y, new Color(resultValue, 0, 0, 1.0f));
        });
    }
}
