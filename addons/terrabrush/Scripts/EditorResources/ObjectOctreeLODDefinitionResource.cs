using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ObjectOctreeLODDefinitionResource : Resource {
    [Export] public float MaxDistance { get;set; }
    [Export] public bool AddCollision { get;set; }
    [Export] public int AutomaticLODTargetVertices { get;set; } = -1;
    [Export] public int ObjectFrequency { get;set; } = -1;
}
