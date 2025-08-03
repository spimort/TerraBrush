using System.Linq;
using Godot;
using Godot.Collections;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class TextureSetResource : Resource {
    private const string NormalFilesHint = "normal";
    private const string RoughnessFilesHint = "roughness";
    private const string HeightFilesHint = "height";

    private Texture2D _albedoTexture;

    [BindProperty] public string Name { get;set; }
    [BindProperty] public Texture2D AlbedoTexture {
        get {
            return _albedoTexture;
        } set {
            if (Engine.Singleton.IsEditorHint() && value != null && !string.IsNullOrWhiteSpace(value.ResourcePath) && value.ResourcePath != _albedoTexture?.ResourcePath) {
                var directory = value.ResourcePath.Replace(System.IO.Path.GetFileName(value.ResourcePath), string.Empty);
                var directoryFiles = DirAccess.GetFilesAt(directory);

                var normalFiles = directoryFiles.Where(file => file.Contains(NormalFilesHint, System.StringComparison.InvariantCultureIgnoreCase) && !file.EndsWith(".import"));
                var normalFile = string.Empty;
                if (normalFiles.Count() == 1) {
                    normalFile = normalFiles.ElementAt(0);
                } else if (normalFiles.Count() > 1) {
                    normalFiles = normalFiles.Where(file => file.Contains("GL"));
                    if (normalFiles.Count() == 1) {
                        normalFile = normalFiles.ElementAt(0);
                    }
                }

                if (!string.IsNullOrWhiteSpace(normalFile)) {
                    NormalTexture = ResourceLoaderHelper.Load<Texture2D>(System.IO.Path.Combine(directory, normalFile));
                }

                RoughnessTexture ??= FindTexture(RoughnessFilesHint, directory, directoryFiles);
                HeightTexture ??= FindTexture(HeightFilesHint, directory, directoryFiles);
            }

            _albedoTexture = value;
        }
    }

    [BindProperty] public Texture2D NormalTexture { get;set; }
    [BindProperty] public Texture2D RoughnessTexture { get;set; }
    [BindProperty] public Texture2D HeightTexture { get;set; }
    [BindProperty] public int TextureDetail { get;set; } = -1;
    [BindProperty] public bool Triplanar { get;set; }

    private Texture2D FindTexture(string fileHint, string directory, PackedStringArray directoryFiles) {
        var files = directoryFiles.Where(file => file.Contains(fileHint, System.StringComparison.InvariantCultureIgnoreCase) && !file.EndsWith(".import"));
        if (files.Count() == 1) {
            var file = files.ElementAt(0);
            return ResourceLoaderHelper.Load<Texture2D>(System.IO.Path.Combine(directory, file));
        }
        return null;
    }
}
