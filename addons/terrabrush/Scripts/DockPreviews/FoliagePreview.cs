using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class FoliagePreview : TextureButton, IDockPreview {
    private Texture2D _textureNormal;

    [NodePath] private TextureRect _foliageTextureRect;
    [NodePath] private Label _label;

    public Action OnSelect { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        Connect("pressed", new Callable(this, nameof(OnItemSelect)));

        _textureNormal = TextureNormal;
    }

    public void SetTextureImage(Texture2D textureImage) {
        _foliageTextureRect.Texture = textureImage;
    }

    public void OnItemSelect() {
        OnSelect?.Invoke();
    }

    public void LoadPreview(Mesh mesh, Material material) {
        if (material == null) {
            EditorInterface.Singleton.GetResourcePreviewer().QueueEditedResourcePreview(mesh, this, nameof(OnPreviewThumbnailReady), System.IO.Path.GetFileName(mesh.ResourcePath));
        } else {
            EditorInterface.Singleton.GetResourcePreviewer().QueueEditedResourcePreview(material, this, nameof(OnPreviewThumbnailReady), System.IO.Path.GetFileName(material.ResourcePath));
        }
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
