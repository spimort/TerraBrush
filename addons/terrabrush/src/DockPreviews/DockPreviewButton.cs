using System;
using Godot;

namespace TerraBrush;

public enum IconType {
    Square = 1,
    Circle = 2,
    CircleWithSign = 3
}

[GodotClass(Tool = true)]
public partial class DockPreviewButton : TextureButton {
    public Action OnSelect { get;set; }

    private Color _normalColor;
    private Color _hoverColor;
    private Color _pressedColor;
    private MarginContainer _marginContainer;
    private TextureRect _textureRect;
    private Label _label;
    private Texture2D _textureSquare;
    private Texture2D _textureCircle;
    private Texture2D _textureCircleWithSign;

    [BindProperty] public Texture2D ButtonImage { get;set; }

    [BindProperty] public IconType IconType { get;set; } = IconType.Square;
    [BindProperty] public int Margin { get;set; }
    [BindProperty] public string Text { get;set; }


    protected override void _Ready() {
        base._Ready();

        CustomMinimumSize = new Vector2(48, 48);
        IgnoreTextureSize = true;
        StretchMode = StretchModeEnum.Scale;
        ToggleMode = true;
        SetAnchorsAndOffsetsPreset(LayoutPreset.FullRect);

        _marginContainer = new MarginContainer();
        _marginContainer.SetAnchorsAndOffsetsPreset(LayoutPreset.FullRect);
        AddChild(_marginContainer);

        _textureRect = new TextureRect {
            ExpandMode = TextureRect.ExpandModeEnum.IgnoreSize
        };
        _marginContainer.AddChild(_textureRect);

        _label = new Label() {
            HorizontalAlignment = HorizontalAlignment.Center,
            VerticalAlignment = VerticalAlignment.Center
        };
        _label.Set((StringName)"theme_override_font_sizes/font_size", 10);
        _marginContainer.AddChild(_label);

        _textureSquare = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Buttons/square_white.png");
        _textureCircle = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Buttons/circle_white.png");
        _textureCircleWithSign = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Buttons/circle_sign_white.png");

        Connect((StringName)"pressed", new Callable(this, (StringName) nameof(OnItemSelect)));

        if (ButtonImage != null) {
            SetTextureImage(ButtonImage);
        }

        CalculateTextures();

        var iconsColor = (Color) ProjectSettings.Singleton.GetSetting(SettingContants.IconsColor);

        _normalColor = iconsColor;
        _hoverColor = _normalColor.Lightened(0.4f);
        _pressedColor = _normalColor.Lightened(0.5f);

        if (_marginContainer != null && Margin > 0) {
            _marginContainer.Set((StringName)"theme_override_constants/margin_left", Margin);
            _marginContainer.Set((StringName)"theme_override_constants/margin_right", Margin);
            _marginContainer.Set((StringName)"theme_override_constants/margin_top", Margin);
            _marginContainer.Set((StringName)"theme_override_constants/margin_bottom", Margin);
        }

        if (_label != null && Text != "") {
            _label.Text = Text;
        }
    }

    protected override void _Notification(int what) {
        base._Notification(what);

        if (what == NotificationDraw) {
            SetTextureColor(GetDrawMode());
        }
    }

    public void SetTextureImage(Texture2D toolImage) {
        _textureRect.Texture = toolImage;
    }

    [BindMethod]
    public void OnItemSelect() {
        OnSelect?.Invoke();
    }

    public void ShowMouseOver() {
        SetTextureColor(DrawMode.Hover);
    }

    public void ClearMouseOver() {
        SetTextureColor(DrawMode.Normal);
    }

    private void CalculateTextures() {
        switch (IconType) {
            case IconType.Square:
                TextureNormal = _textureSquare;
                break;
            case IconType.Circle:
                TextureNormal = _textureCircle;
                break;
            case IconType.CircleWithSign:
                TextureNormal = _textureCircleWithSign;
                break;
        }
    }

    private void SetTextureColor(DrawMode drawMode) {
        var color = drawMode switch {
            DrawMode.Normal => _normalColor,
            DrawMode.Hover => _hoverColor,
            DrawMode.Pressed => _pressedColor,
            DrawMode.HoverPressed => _pressedColor,
            _ => _normalColor
        };

        var tween = GetTree().CreateTween();
        tween.TweenProperty(this, (NodePath)"self_modulate", color, 0.1);
    }

    public void LoadResourcePreview(Resource resoruce) {
        EditorInterface.Singleton.GetResourcePreviewer().QueueEditedResourcePreview(resoruce, this, (StringName)nameof(OnPreviewThumbnailReady), System.IO.Path.GetFileName(resoruce.ResourcePath));
    }

    private void OnPreviewThumbnailReady(string path, Texture2D preview, Texture2D thumbnail_preview, string resourceName) {
        if (preview == null) {
            _label.Text = resourceName;
        } else {
            _label.Text = string.Empty;
            SetTextureImage(preview);
        }
    }
}
