using Godot;
using Godot.Collections;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class TextureSetsResource : Resource {
    [BindProperty] public GodotArray<TextureSetResource> TextureSets { get;set; }
}
