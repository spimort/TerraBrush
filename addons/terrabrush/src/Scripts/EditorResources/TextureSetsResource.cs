using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class TextureSetsResource : Resource {
    [BindProperty] public TextureSetResource[] TextureSets { get;set; }
}
