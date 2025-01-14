using Godot;
using Godot.Collections;

namespace TerraBrush;

public partial class ImportImageRow : PanelContainer {
    private bool _isSelectorOpen = false;
    private Texture2D _originalNormalTexture;

    [NodePath] private Label _label;
    [NodePath] private TextureButton _textureButton;

    public string ImageTypeName { get;set; }
    public Texture2D ImageTexture { get;set; }

    protected override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        var iconsColor = (Color) ProjectSettings.Singleton.GetSetting(SettingContants.IconsColor);
        var rowStyle = (StyleBoxFlat) Get((StringName)"theme_override_styles/panel");
        rowStyle.BgColor = iconsColor;

        _label.Text = ImageTypeName;

        _originalNormalTexture = _textureButton.TextureNormal;
        if (ImageTexture != null) {
            _textureButton.TextureNormal = ImageTexture;
        }

        _textureButton.MouseEntered += () => {
            var tween = CreateTween();
            tween.TweenProperty(_textureButton, (NodePath)"self_modulate", new Color(1, 1, 1, 0.3f), 0.1);
        };

        _textureButton.MouseExited += () => {
            var tween = CreateTween();
            tween.TweenProperty(_textureButton, (NodePath)"self_modulate", NamedColors.White, 0.1);
        };

        _textureButton.GuiInput += async e => {
            if (e is InputEventMouseButton mouseEvent && !e.IsEcho() && !_isSelectorOpen) {
                if (mouseEvent.ButtonIndex == MouseButton.Left) {
                    _isSelectorOpen = true;
                    var file = await DialogUtils.ShowFileDialog(this, filters: new PackedStringArray([..new [] {"*.png, *.jpg, *.jpeg, *.bmp, *.exr ; Supported Images", "*res, *.tres ; Resources"}]), transient: true);
                    _isSelectorOpen = false;

                    if (file != null) {
                        var imageResource = (Texture2D) ResourceLoader.Singleton.Load(file);
                        _textureButton.TextureNormal = (Texture2D)imageResource;
                        ImageTexture = (Texture2D)imageResource;
                    }
                } else if (mouseEvent.ButtonIndex == MouseButton.Right) {
                    ImageTexture = null;
                    _textureButton.TextureNormal = _originalNormalTexture;
                }
            }
        };
    }
}
