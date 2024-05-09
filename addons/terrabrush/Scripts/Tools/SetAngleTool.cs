#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class SetAngleTool : ToolBase {
    private const float MaxAngle = 89.9f;

    private HashSet<ZoneResource> _sculptedZones;
    private float _setAngleValue = 0;
    private Vector3? _setAngleInitialPoint = null;
    private Node3D _initialPointMesh = null;

    public SetAngleTool(TerraBrush terraBrush) : base(terraBrush) {
        ClearInitialPointMesh();

        _setAngleValue = terraBrush.SelectedSetAngle;
        _setAngleInitialPoint = terraBrush.SelectedSetAngleInitialPoint;

        UpdateInitialPointMesh();
    }

    public override void BeforeDeselect () {
        ClearInitialPointMesh();
    }

    public override bool HandleInput(TerrainToolType toolType, InputEvent @event) {
        if (Input.IsKeyPressed(Key.Ctrl)) {
            float increment = 1;
            int roundFactor = 0;

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
                _setAngleValue += incrementValue.Value;
                UpdateSetAngleValue((float) Math.Round(_setAngleValue, roundFactor));
                return true;
            }
        }

        return base.HandleInput(toolType, @event);
    }

    public override string GetToolInfo(TerrainToolType toolType) {
        var initialValue = string.Empty;

        if (_setAngleInitialPoint == null) {
            initialValue = "Select the initial point with CTRL + click";
        }

        if (_setAngleValue == 0) {
            initialValue = $"{initialValue}\nSelect angle with CTRL + (mouse wheel or +/-)";
        }

        return $"{initialValue}\nAngle : {_setAngleValue}\n".Trim();
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

            _setAngleInitialPoint = new Vector3(imagePosition.X, currentPixel.R, imagePosition.Y);

            UpdateSetAngleValue(_setAngleValue);
            UpdateInitialPointMesh();

            return;
        }

        if (_setAngleInitialPoint == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var absolutePosition = imageZoneInfo.ZoneInfo.ImagePosition + (imageZoneInfo.ZoneInfo.ZonePosition * (_terraBrush.ZonesSize - 1));
            var distanceToStartingPoint = new Vector2(_setAngleInitialPoint.Value.X, _setAngleInitialPoint.Value.Z).DistanceTo(absolutePosition);
            var angleHeight = (float) (distanceToStartingPoint * Mathf.Tan(Mathf.DegToRad(_setAngleValue))) + _setAngleInitialPoint.Value.Y;

            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, angleHeight, pixelBrushStrength * brushStrength),
                currentPixel.G,
                currentPixel.B,
                currentPixel.A
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }

    public float GetSetAngleValue() {
        return _setAngleValue;
    }

    public void UpdateSetAngleValue(float value) {
        value = Mathf.Clamp(value, -MaxAngle, MaxAngle);

        _setAngleValue = value;
        _terraBrush.UpdateSetAngleValue(value, _setAngleInitialPoint);
    }

    private void UpdateInitialPointMesh() {
        if (_setAngleInitialPoint == null) {
            _initialPointMesh?.QueueFree();
        } else {
            if (_initialPointMesh == null) {
                _initialPointMesh = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/SetAngleInitialPoint.tscn").Instantiate<Node3D>();

                var container = _terraBrush.GetNodeOrNull(new NodePath(StringNames.SetAnglePointContainer));
                if (container == null) {
                    container = new Node3D();
                    container.Name = StringNames.SetAnglePointContainer;
                    _terraBrush.AddChild(container);
                }

                container.AddChild(_initialPointMesh);
            }

            _initialPointMesh.GlobalPosition = _setAngleInitialPoint.Value - new Vector3(_terraBrush.ZonesSize / 2.0f, 0, _terraBrush.ZonesSize / 2.0f);
        }
    }

    private void ClearInitialPointMesh() {
        var existingPointContainer = _terraBrush.GetNodeOrNull(new NodePath(StringNames.SetAnglePointContainer));
        if (existingPointContainer != null) {
            existingPointContainer.Name = new StringName($"{StringNames.SetAnglePointContainer}_temp");
            existingPointContainer.QueueFree();
        }
    }
}
#endif
