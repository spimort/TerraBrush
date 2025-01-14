using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODDefinitionResource : Resource {
    public float MaxDistance { get;set; }
    public bool AddCollision { get;set; }
    public int AutomaticLODTargetVertices { get;set; } = -1;
}
