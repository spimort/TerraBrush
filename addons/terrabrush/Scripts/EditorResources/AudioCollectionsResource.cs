using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class AudioCollectionsResource : Resource {
    [Export] public AudioCollectionResource[] AudioCollection { get; set; }
}
