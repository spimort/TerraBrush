using Godot;
using Godot.Collections;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ObjectOctreeLODMeshesDefinitionResource : Resource {
    [BindProperty] public GodotArray<ObjectOctreeLODMeshDefinitionResource> Meshes { get;set; }
    [BindProperty] public Shape3D CollisionShape { get;set; }
}
