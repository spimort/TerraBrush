using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ObjectDefinitionResource : Resource {
    [Export] public int ObjectFrequency { get;set; } = -1;
    [Export] public float RandomRange { get;set; } = 1;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public bool RandomYRotation { get;set; }
    [Export] public PackedScene[] ObjectScenes { get;set; }
}
