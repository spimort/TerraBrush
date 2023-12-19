using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class TexturePreview : TextureButton, IDockPreview {
    private Texture2D _textureNormal;

    [NodePath] private TextureRect _textureRect;

    public Action OnSelect { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect("pressed", new Callable(this, nameof(OnItemSelect)));

        _textureNormal = TextureNormal;
    }

    public void SetTextureImage(Texture2D textureImage) {
        _textureRect.Texture = textureImage;
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
