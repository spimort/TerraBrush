using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODMeshesDefinitionResource : Resource {
    public ObjectOctreeLODMeshDefinitionResource[] Meshes { get;set; }
    public Shape3D CollisionShape { get;set; }
}
