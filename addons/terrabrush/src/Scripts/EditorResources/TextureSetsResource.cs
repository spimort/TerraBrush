using Godot;

namespace TerraBrush;

public partial class TextureSetsResource : Resource {
    [Export] public TextureSetResource[] TextureSets { get;set; }
}
