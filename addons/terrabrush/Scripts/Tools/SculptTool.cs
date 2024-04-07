#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class SculptTool : ToolBase {
    private float _setHeightValue = 0;
    private int _sculptingMultiplier = 1;
    private HashSet<ZoneResource> _sculptedZones;

    public SculptTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override string GetToolInfo(TerrainToolType toolType) {
        if (toolType != TerrainToolType.TerrainSetHeight) {
            return base.GetToolInfo(toolType);
        }

        return $"{base.GetToolInfo(toolType)}{(_setHeightValue == 0 ? "\nSelect height with CTRL + (click or mouse wheel or +/-)\nUse CTRL + ALT to set the increment to 0.1" : "")}\nHeight : {_setHeightValue}".Trim();
    }

    public override bool HandleInput(TerrainToolType toolType, InputEvent @event) {
        if (toolType == TerrainToolType.TerrainSetHeight && Input.IsKeyPressed(Key.Ctrl)) {
            float increment = 1;
            int roundFactor = 0;
            if (Input.IsKeyPressed(Key.Alt)) {
                increment = 0.1f;
                roundFactor = 1;
            }

            float? incrementValue = null;
            if (@event is InputEventMouseButton inputMouseButton) {
                if (inputMouseButton.ButtonIndex == MouseButton.WheelUp) {
                    incrementValue = increment;
                } else if (inputMouseButton.ButtonIndex == MouseButton.WheelDown) {
                    incrementValue = -increment;
                }
            }

            if (@event is InputEventKey inputEvent) {
                if (inputEvent.Keycode == Key.Equal) {
                    incrementValue = increment;
                } else if (inputEvent.Keycode == Key.Minus) {
                    incrementValue = -increment;
                }
            }

            if (incrementValue != null) {
                _setHeightValue += incrementValue.Value;
                _setHeightValue = (float) Math.Round(_setHeightValue, roundFactor);
                return true;
            }
        }

        return base.HandleInput(toolType, @event);
    }

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
        } else if (toolType == TerrainToolType.TerrainFlatten) {
            Flatten(toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else if (toolType == TerrainToolType.TerrainSetHeight) {
            SetHeight(toolType, brushImage, brushSize, brushStrength, imagePosition);
        } else {
            Sculpt(toolType, brushImage, brushSize, brushStrength, imagePosition);
            Smooth(toolType, brushImage, brushSize, brushStrength, imagePosition);
        }

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }

    private void Sculpt(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
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

    private void Flatten(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
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
                Mathf.Lerp(currentPixel.G, smoothValue.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, smoothValue.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, smoothValue.A, pixelBrushStrength * brushStrength)
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }

    private void Smooth(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var directions = new List<float>();

            var neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, -1, 0);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 1, 0);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 0, -1);
            if (neighbourImageZoneInfo != null) {
                directions.Add(neighbourImageZoneInfo.Image.GetPixel(neighbourImageZoneInfo.ZoneInfo.ImagePosition.X, neighbourImageZoneInfo.ZoneInfo.ImagePosition.Y).R);
            }

            neighbourImageZoneInfo = GetImageZoneInfoForPosition(imageZoneInfo.ZoneInfo, 0, 1);
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

    private void SetHeight(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (Input.IsKeyPressed(Key.Ctrl)) {
            var initialPoint = ZoneUtils.GetPixelToZoneInfo(imagePosition.X, imagePosition.Y, _terraBrush.ZonesSize);
            var imageZoneInfo = GetImageZoneInfoForPosition(initialPoint, 0, 0);
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            _setHeightValue = currentPixel.R;
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, _setHeightValue, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, currentPixel.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, currentPixel.B, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.A, currentPixel.A, pixelBrushStrength * brushStrength)
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });
    }
}
#endif
