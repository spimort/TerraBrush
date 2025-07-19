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

    [BindProperty] public Texture2D ButtonImage { get;set; }
    [BindProperty] public TextureRect TextureRect { get;set; }
    [BindProperty] public MarginContainer MarginContainer { get;set; }
    [BindProperty] public Label Label { get;set; }

    [BindProperty] public IconType IconType { get;set; } = IconType.Square;
    [BindProperty] public int Margin { get;set; }
    [BindProperty] public string Text { get;set; }

    [BindProperty] public Texture2D TextureSquare { get;set; }
    [BindProperty] public Texture2D TextureCircle { get;set; }
    [BindProperty] public Texture2D TextureCircleWithSign { get;set; }

    protected override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect((StringName)"pressed", new Callable(this, (StringName) nameof(OnItemSelect)));

        if (ButtonImage != null) {
            SetTextureImage(ButtonImage);
        }

        CalculateTextures();

        var iconsColor = (Color) ProjectSettings.Singleton.GetSetting(SettingContants.IconsColor);

        _normalColor = iconsColor;
        _hoverColor = _normalColor.Lightened(0.4f);
        _pressedColor = _normalColor.Lightened(0.5f);

        if (MarginContainer != null && Margin > 0) {
            MarginContainer.Set((StringName)"theme_override_constants/margin_left", Margin);
            MarginContainer.Set((StringName)"theme_override_constants/margin_right", Margin);
            MarginContainer.Set((StringName)"theme_override_constants/margin_top", Margin);
            MarginContainer.Set((StringName)"theme_override_constants/margin_bottom", Margin);
        }

        if (Label != null && Text != "") {
            Label.Text = Text;
        }
    }

    protected override void _Notification(int what) {
        base._Notification(what);

        if (what == NotificationDraw) {
            SetTextureColor(GetDrawMode());
        }
    }

    public void SetTextureImage(Texture2D toolImage) {
        TextureRect.Texture = toolImage;
    }

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
                TextureNormal = TextureSquare;
                break;
            case IconType.Circle:
                TextureNormal = TextureCircle;
                break;
            case IconType.CircleWithSign:
                TextureNormal = TextureCircleWithSign;
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

        var tween = CreateTween();
        tween.TweenProperty(this, (NodePath)"self_modulate", color, 0.1);
    }

    public void LoadResourcePreview(Resource resoruce) {
        EditorInterface.Singleton.GetResourcePreviewer().QueueEditedResourcePreview(resoruce, this, (StringName)nameof(OnPreviewThumbnailReady), System.IO.Path.GetFileName(resoruce.ResourcePath));
    }

    private void OnPreviewThumbnailReady(string path, Texture2D preview, Texture2D thumbnail_preview, string resourceName) {
        if (preview == null) {
            Label.Text = resourceName;
        } else {
            Label.Text = string.Empty;
            SetTextureImage(preview);
        }
    }
}
