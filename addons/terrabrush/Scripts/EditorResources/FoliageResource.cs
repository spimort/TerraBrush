using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageResource : Resource {
    [Export] public ImageTexture Texture { get;set; }
    [Export] public FoliageDefinitionResource Definition { get;set; }
}
