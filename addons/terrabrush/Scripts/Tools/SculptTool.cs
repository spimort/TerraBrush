using System;
using System.Collections.Generic;
using System.Linq;
using Godot;
using TerraBrush;

namespace TerraBrush;

public class SculptTool : ToolBase {
    private int _sculptingMultiplier = 1;
    private HashSet<ZoneResource> _sculptedZones;

    public override void BeginPaint(TerraBrush terraBrush) {
        base.BeginPaint(terraBrush);

        _sculptingMultiplier = (int) ProjectSettings.GetSetting(SettingContants.SculptingMultiplier);
        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint(TerraBrush terraBrush) {
        base.EndPaint(terraBrush);

        terraBrush.UpdateObjectsHeight(_sculptedZones.ToList());

        _sculptedZones = null;
    }

    protected override ImageTexture GetToolCurrentImageTexture(TerraBrush terraBrush, ZoneResource zone) {
        return zone.HeightMapTexture;
    }

    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (toolType == TerrainToolType.TerrainSmooth) {
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else if (toolType == TerrainToolType.TerrainFlattern) {
            Flattern(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else {
            Sculpt(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
            Smooth(terraBrush, toolType, brushImage, brushSize, brushStrength, imagePosition);
        }

        terraBrush.TerrainZones.UpdateHeightmaps();
    }

    private void Sculpt(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = Colors.Red * (pixelBrushStrength * brushStrength) * _sculptingMultiplier;
            if (toolType == TerrainToolType.TerrainAdd) {
                newValue = currentPixel + newValue;
            } else if (toolType == TerrainToolType.TerrainRemove) {
                newValue = currentPixel - newValue;
            }

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }

    private void Flattern(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, smoothValue.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, smoothValue.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, smoothValue.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, smoothValue.A, pixelBrushStrength * brushStrength)
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }

    private void Smooth(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X - 1, absoluteImagePosition.Y);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X + 1, absoluteImagePosition.Y);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X, absoluteImagePosition.Y - 1);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(terraBrush, absoluteImagePosition.X, absoluteImagePosition.Y + 1);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y).R;
            directions.Add(currentPixel);

            float average = directions.Average();
            float resultValue = Mathf.Lerp(currentPixel, average, pixelBrushStrength * brushStrength);

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, new Color(resultValue, 0, 0, 1.0f));
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }
}
