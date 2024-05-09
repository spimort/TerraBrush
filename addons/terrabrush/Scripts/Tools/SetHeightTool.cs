#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class SetHeightTool : ToolBase {
    private HashSet<ZoneResource> _sculptedZones;
    private float _setHeightValue = 0;

    public SetHeightTool(TerraBrush terraBrush) : base(terraBrush) {
        _setHeightValue = terraBrush.SelectedSetHeight;
    }

    public override bool HandleInput(TerrainToolType toolType, InputEvent @event) {
        if (Input.IsKeyPressed(Key.Ctrl)) {
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
                UpdateSetHeightValue((float) Math.Round(_setHeightValue, roundFactor));
                return true;
            }
        }

        return base.HandleInput(toolType, @event);
    }

    public override string GetToolInfo(TerrainToolType toolType) {
        var initialValue = string.Empty;
        if (_setHeightValue == 0) {
            initialValue = "Select height with CTRL + (click or mouse wheel or +/-)\nUse CTRL + ALT to set the increment to 0.1";
        }

        return $"{initialValue}\nHeight : {_setHeightValue}".Trim();
    }

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.HeightMapTexture;
    }

    public override void BeginPaint() {
        base.BeginPaint();

        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _terraBrush.UpdateObjectsHeight(_sculptedZones.ToList());

        _sculptedZones = null;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (Input.IsKeyPressed(Key.Ctrl)) {
            var initialPoint = ZoneUtils.GetPixelToZoneInfo(imagePosition.X, imagePosition.Y, _terraBrush.ZonesSize);
            var imageZoneInfo = GetImageZoneInfoForPosition(initialPoint, 0, 0);
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            UpdateSetHeightValue(_setHeightValue = currentPixel.R);
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, _setHeightValue, pixelBrushStrength * brushStrength),
                currentPixel.G,
                currentPixel.B,
                currentPixel.A
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }

    public float GetSetHeightValue() {
        return _setHeightValue;
    }

    public void UpdateSetHeightValue(float value) {
        _setHeightValue = value;
        _terraBrush.UpdateSetHeightValue(value);
    }
}
#endif
