using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class BrushNumericSelector : Control {
    private const int BackgroundMargin = 10;

    [NodePath] private TextureRect _background;
    [NodePath] private TextureRect _brushPreview;
    [NodePath] private Label _valueLabel;

    [Export] public int BrushSizeFactor { get;set; } = 2;
    [Export] public Color WidgetColor { get;set; } = Colors.White;
    [Export] public int MinValue { get;set; } = -1;
    [Export] public int MaxValue { get;set; } = -1;

    public Action<int> OnValueSelected { get;set; }
    public Action OnCancel { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        _brushPreview.Modulate = WidgetColor;

        var iconsColor = (Color) ProjectSettings.GetSetting(SettingContants.IconsColor);
        _valueLabel.Set("theme_override_colors/font_outline_color", iconsColor);
        var normalStyle = (StyleBoxFlat) _valueLabel.Get("theme_override_styles/normal");
        normalStyle.BgColor = iconsColor;
    }

    public override void _Process(double delta) {
        base._Process(delta);

        UpdateValue(GetMouseDistance());
    }

    public override void _GuiInput(InputEvent @event) {
        base._GuiInput(@event);

        if (@event is InputEventMouseButton inputButton) {
            if (inputButton.ButtonIndex == MouseButton.Left) {
                var distance = GetMouseDistance();
                OnValueSelected?.Invoke(distance);
            } else {
                OnCancel?.Invoke();
            }
        }
    }

    public void SetInitialValue(float value) {
        Position -= new Vector2(value * BrushSizeFactor / 2, 0);
        UpdateValue(value);
    }

    public void UpdateValue(float value) {
        var size = value * BrushSizeFactor;
        _background.Size = new Vector2(size + BackgroundMargin, size + BackgroundMargin);
        _background.Position = new Vector2(-((size / 2) + BackgroundMargin / 2), -((size / 2) + BackgroundMargin / 2));

        _brushPreview.Size = new Vector2(size, size);
        _brushPreview.Position = new Vector2(-(size / 2), -(size / 2));

        _valueLabel.Text = value.ToString();
    }

    private int GetMouseDistance() {
        var distance = Position.DistanceTo(GetGlobalMousePosition());

        if (MinValue >= 0) {
            distance = Math.Max(distance, MinValue);
        }

        if (MaxValue >= 0) {
            distance = Math.Min(distance, MaxValue);
        }

        return (int) distance;
    }

    public void RequestSelectValue() {
        var distance = GetMouseDistance();
        OnValueSelected?.Invoke(distance);
    }
}
