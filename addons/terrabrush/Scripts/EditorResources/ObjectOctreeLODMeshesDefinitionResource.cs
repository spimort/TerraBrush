using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ObjectOctreeLODMeshesDefinitionResource : Resource {
    [Export] public ObjectOctreeLODMeshDefinitionResource[] Meshes { get;set; }
    [Export] public Shape3D CollisionShape { get;set; }
}
