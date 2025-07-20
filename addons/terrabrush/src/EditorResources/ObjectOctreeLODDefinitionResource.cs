using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ObjectOctreeLODDefinitionResource : Resource {
    [BindProperty] public float MaxDistance { get;set; }
    [BindProperty] public bool AddCollision { get;set; }
    [BindProperty] public bool AutomaticLOD { get;set; }
    [BindProperty] public int AutomaticLODTargetVertices { get;set; } = -1;
    [BindProperty] public int ObjectFrequency { get;set; } = -1;
}
