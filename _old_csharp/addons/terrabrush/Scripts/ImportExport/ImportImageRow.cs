#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class ImportImageRow : PanelContainer {
    private bool _isSelectorOpen = false;
    private Texture2D _originalNormalTexture;

    [NodePath] private Label _label;
    [NodePath] private TextureButton _textureButton;

    [Export] public string ImageTypeName { get;set; }
    [Export] public Texture2D ImageTexture { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        var iconsColor = (Color) ProjectSettings.GetSetting(SettingContants.IconsColor);
        var rowStyle = (StyleBoxFlat) Get("theme_override_styles/panel");
        rowStyle.BgColor = iconsColor;

        _label.Text = ImageTypeName;

        _originalNormalTexture = _textureButton.TextureNormal;
        if (ImageTexture != null) {
            _textureButton.TextureNormal = ImageTexture;
        }

        _textureButton.MouseEntered += () => {
            var tween = CreateTween();
            tween.TweenProperty(_textureButton, "self_modulate", new Color(1, 1, 1, 0.3f), 0.1);
        };

        _textureButton.MouseExited += () => {
            var tween = CreateTween();
            tween.TweenProperty(_textureButton, "self_modulate", Colors.White, 0.1);
        };

        _textureButton.GuiInput += async e => {
            if (e is InputEventMouseButton mouseEvent && !e.IsEcho() && !_isSelectorOpen) {
                if (mouseEvent.ButtonIndex == MouseButton.Left) {
                    _isSelectorOpen = true;
                    var file = await DialogUtils.ShowFileDialog(this, filters: new [] {"*.png, *.jpg, *.jpeg, *.bmp, *.exr ; Supported Images", "*res, *.tres ; Resources"}, transient: true);
                    _isSelectorOpen = false;

                    if (file != null) {
                        var imageResource = ResourceLoader.Load<Texture2D>(file);
                        _textureButton.TextureNormal = imageResource;
                        ImageTexture = imageResource;
                    }
                } else if (mouseEvent.ButtonIndex == MouseButton.Right) {
                    ImageTexture = null;
                    _textureButton.TextureNormal = _originalNormalTexture;
                }
            }
        };
    }
}
#endif
