using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODMeshesDefinitionResource : Resource {
    [Export] public ObjectOctreeLODMeshDefinitionResource[] Meshes { get;set; }
    [Export] public Shape3D CollisionShape { get;set; }
}
