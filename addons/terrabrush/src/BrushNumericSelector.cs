using System;
using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class BrushNumericSelector : Control {
    private const int BackgroundMargin = 10;

    private TextureRect _background;
    private TextureRect _brushPreview;
    private Label _valueLabel;

    [BindProperty] public int BrushSizeFactor { get;set; } = 2;
    [BindProperty] public Color WidgetColor { get;set; } = NamedColors.White;
    [BindProperty] public int MinValue { get;set; } = -1;
    [BindProperty] public int MaxValue { get;set; } = -1;

    public Action<int> OnValueSelected { get;set; }
    public Action OnCancel { get;set; }

    protected override void _Ready() {
        base._Ready();

        _background = new TextureRect {
            ExpandMode = TextureRect.ExpandModeEnum.IgnoreSize,
            Texture = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/white_circle.png"),
            Modulate = Color.FromHtml("#3a3a3a4b")
        };
        AddChild(_background);

        _brushPreview = new TextureRect {
            ExpandMode = TextureRect.ExpandModeEnum.IgnoreSize,
            Texture = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/brush_preview.png"),
        };
        _brushPreview.SetAnchorsAndOffsetsPreset(LayoutPreset.Center);
        AddChild(_brushPreview);

        _valueLabel = new Label {
            HorizontalAlignment = HorizontalAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center,
            AnchorLeft = 0.5f,
            AnchorRight = 0.5f,
            AnchorTop = 0.5f,
            AnchorBottom = 0.5f,
            OffsetLeft = -11.5f,
            OffsetTop = -14.0f,
            OffsetRight = 11.5f,
            OffsetBottom = 14.0f,
            GrowHorizontal = GrowDirection.Both,
            GrowVertical = GrowDirection.Both,
        };
        _valueLabel.Set((StringName)"theme_override_colors/font_color", NamedColors.White);
        _valueLabel.Set((StringName)"theme_override_colors/font_outline_color", Color.FromHtml("#00151f"));
        _valueLabel.Set((StringName)"theme_override_constants/outline_size", 3);
        _valueLabel.Set((StringName)"theme_override_font_sizes/font_size", 20);
        _valueLabel.Set((StringName)"theme_override_styles/normal", new StyleBoxFlat {
            BgColor = Color.FromHtml("#00151f"),
            CornerRadiusTopLeft = 5,
            CornerRadiusTopRight = 5,
            CornerRadiusBottomLeft = 5,
            CornerRadiusBottomRight = 5,
            ExpandMarginTop = 5,
            ExpandMarginRight = 5,
            ExpandMarginBottom = 5,
            ExpandMarginLeft = 5,
        });
        AddChild(_valueLabel);

        _brushPreview.Modulate = WidgetColor;

        var iconsColor = (Color) ProjectSettings.Singleton.GetSetting(SettingContants.IconsColor);
        _valueLabel.Set((StringName)"theme_override_colors/font_outline_color", iconsColor);
        var normalStyle = (StyleBoxFlat) _valueLabel.Get((StringName)"theme_override_styles/normal");
        normalStyle.BgColor = iconsColor;
    }

    protected override void _Process(double delta) {
        base._Process(delta);

        UpdateValue(GetMouseDistance());
    }

    protected override void _GuiInput(InputEvent @event) {
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
