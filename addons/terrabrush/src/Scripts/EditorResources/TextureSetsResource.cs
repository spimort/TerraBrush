using Godot;

namespace TerraBrush;

[GlobalClass]
public partial class TextureSetsResource : Resource {
    [Export] public TextureSetResource[] TextureSets { get;set; }
}
