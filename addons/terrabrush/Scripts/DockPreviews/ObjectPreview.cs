using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class ObjectPreview : TextureButton, IDockPreview {
    private Texture2D _textureNormal;

    [NodePath] private TextureRect _objectTextureRect;
    [NodePath] private Label _label;

    public Action OnSelect { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect("pressed", new Callable(this, nameof(OnItemSelect)));

        _textureNormal = TextureNormal;
    }

    public void SetTextureImage(Texture2D textureImage) {
        _objectTextureRect.Texture = textureImage;
    }

    public void OnItemSelect() {
        OnSelect?.Invoke();
    }

    public void LoadPreview(PackedScene scene) {
        EditorInterface.Singleton.GetResourcePreviewer().QueueEditedResourcePreview(scene, this, nameof(OnPreviewThumbnailReady), System.IO.Path.GetFileName(scene.ResourcePath));
    }

    private void OnPreviewThumbnailReady(string path, Texture2D preview, Texture2D thumbnail_preview, string resourceName) {
        if (preview == null) {
            _label.Text = resourceName;
        } else {
            _label.Text = string.Empty;
            SetTextureImage(preview);
        }
    }

    public void ShowMouseOver() {
        TextureNormal = TextureHover;
    }

    public void ClearMouseOver() {
        TextureNormal = _textureNormal;
    }
}
