using System;
using System.Collections.Generic;
using System.Linq;
using Godot;
using TerraBrush;

namespace TerraBrush;

public class SculptTool : ToolBase {
    private int _sculptingMultiplier = 1;
    private HashSet<ZoneResource> _sculptedZones;

    public SculptTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint() {
        base.BeginPaint();

        _sculptingMultiplier = (int) ProjectSettings.GetSetting(SettingContants.SculptingMultiplier);
        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _terraBrush.UpdateObjectsHeight(_sculptedZones.ToList());

        _sculptedZones = null;
    }

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.HeightMapTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (toolType == TerrainToolType.TerrainSmooth) {
            Smooth(toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else if (toolType == TerrainToolType.TerrainFlattern) {
            Flattern(toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else {
            Sculpt(toolType, brushImage, brushSize, brushStrength, imagePosition);
            Smooth(toolType, brushImage, brushSize, brushStrength, imagePosition);
        }

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }

    private void Sculpt(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
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

    private void Flattern(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
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

    private void Smooth(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength, absoluteImagePosition) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(absoluteImagePosition.X - 1, absoluteImagePosition.Y);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(absoluteImagePosition.X + 1, absoluteImagePosition.Y);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(absoluteImagePosition.X, absoluteImagePosition.Y - 1);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(absoluteImagePosition.X, absoluteImagePosition.Y + 1);
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
