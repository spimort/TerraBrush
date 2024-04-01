#if TOOLS
using System;
using Godot;

namespace TerraBrush;

public enum IconType {
    Square = 1,
    Circle = 2,
    CircleWithSign = 3
}

[Tool]
public partial class ToolPreview : TextureButton, IDockPreview {
    private Texture2D _textureNormal;
    private Texture2D _textureHover;
    private Texture2D _texturePressed;

    [NodePath] private TextureRect _toolIconTextureRect;

    public Action OnSelect { get;set; }

    [Export] public Texture2D ToolImage { get;set; }
    [Export] public TerrainToolType ToolType { get;set; }
    [Export] public IconType IconType { get;set; } = IconType.Square;

    [Export] public Texture2D TextureNormalSquare { get;set; }
    [Export] public Texture2D TextureHoverSquare { get;set; }
    [Export] public Texture2D TexturePressedSquare { get;set; }

    [Export] public Texture2D TextureNormalCircle { get;set; }
    [Export] public Texture2D TextureHoverCircle { get;set; }
    [Export] public Texture2D TexturePressedCircle { get;set; }

    [Export] public Texture2D TextureNormalCircleWithSign { get;set; }
    [Export] public Texture2D TextureHoverCircleWithSign { get;set; }
    [Export] public Texture2D TexturePressedCircleWithSign { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect("pressed", new Callable(this, nameof(OnItemSelect)));

        if (ToolImage != null) {
            SetTextureImage(ToolImage);
        }

        CalculateTextures();
        TextureNormal = _textureNormal;
        TextureHover = _textureHover;
        TexturePressed = _texturePressed;
        TextureFocused = _texturePressed;
    }

    public void SetTextureImage(Texture2D toolImage) {
        _toolIconTextureRect.Texture = toolImage;
    }

    public void OnItemSelect() {
        OnSelect?.Invoke();
    }

    public void ShowMouseOver() {
        TextureNormal = TextureHover;
    }

    public void ClearMouseOver() {
        TextureNormal = _textureNormal;
    }

    private void CalculateTextures() {
        switch (IconType) {
            case IconType.Square:
                _textureNormal = TextureNormalSquare;
                _textureHover = TextureHoverSquare;
                _texturePressed = TexturePressedSquare;
                break;
            case IconType.Circle:
                _textureNormal = TextureNormalCircle;
                _textureHover = TextureHoverCircle;
                _texturePressed = TexturePressedCircle;
                break;
            case IconType.CircleWithSign:
                _textureNormal = TextureNormalCircleWithSign;
                _textureHover = TextureHoverCircleWithSign;
                _texturePressed = TexturePressedCircleWithSign;
                break;
        }
    }
}
#endif
