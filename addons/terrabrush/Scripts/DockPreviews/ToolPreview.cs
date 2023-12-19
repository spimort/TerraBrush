using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class ToolPreview : TextureButton, IDockPreview {
    private Texture2D _textureNormal;

    [NodePath] private TextureRect _toolIconTextureRect;

    public Action OnSelect { get;set; }

    [Export] public Texture2D ToolImage { get;set; }
    [Export] public TerrainToolType ToolType { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect("pressed", new Callable(this, nameof(OnItemSelect)));

        if (ToolImage != null) {
            SetTextureImage(ToolImage);
        }

        _textureNormal = TextureNormal;
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
}
