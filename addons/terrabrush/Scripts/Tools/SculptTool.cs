#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class SculptTool : ToolBase {
    private int _sculptingMultiplier = 1;
    private HashSet<ZoneResource> _sculptedZones;

    protected override bool ApplyResolution => true;

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
        switch (toolType) {
            case TerrainToolType.TerrainSmooth:
                Smooth(brushImage, brushSize, brushStrength, imagePosition);
                break;
            case TerrainToolType.TerrainFlatten:
                Flatten(brushImage, brushSize, brushStrength, imagePosition);
                break;
            default:
                Sculpt(toolType, brushImage, brushSize, brushStrength, imagePosition);
                Smooth(brushImage, brushSize, brushStrength, imagePosition);
                break;
        }

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }

    private void Sculpt(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = pixelBrushStrength * brushStrength * _sculptingMultiplier;
            if (toolType == TerrainToolType.TerrainAdd) {
                newValue = currentPixel.R + newValue;
            } else if (toolType == TerrainToolType.TerrainRemove) {
                newValue = currentPixel.R - newValue;
            }

            var newPixel = new Color(newValue, currentPixel.G, currentPixel.B, currentPixel.A);
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newPixel);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }

    private void Flatten(Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        Color smoothValue = Colors.Transparent;
        var numberOfSamples = 0;

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            smoothValue += currentPixel;
            numberOfSamples += 1;
        });

        smoothValue = smoothValue / numberOfSamples;

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, smoothValue.R, pixelBrushStrength * brushStrength),
                currentPixel.G,
                currentPixel.B,
                currentPixel.A
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }

    private void Smooth(Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, -1, 0, true);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 1, 0, true);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 0, -1, true);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 0, 1, true);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            directions.Add(currentPixel.R);

            float average = directions.Average();
            float resultValue = Mathf.Lerp(currentPixel.R, average, pixelBrushStrength * brushStrength);

            var newPixel = new Color(resultValue, currentPixel.G, currentPixel.B, currentPixel.A);
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newPixel);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }
}
#endif
