using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODMeshDefinitionResource : Resource {
    public Mesh Mesh { get;set; }
    public Material MaterialOverride { get;set; }
    [Export(PropertyHint.Link)] public Vector3 Scale { get;set; } = Vector3.One;
}
