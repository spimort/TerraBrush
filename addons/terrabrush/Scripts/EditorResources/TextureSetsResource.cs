using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class TextureSetsResource : Resource {
    [Export] public TextureSetResource[] TextureSets { get;set; }
}
