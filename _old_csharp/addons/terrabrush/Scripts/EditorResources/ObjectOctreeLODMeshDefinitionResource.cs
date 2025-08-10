using Godot;
using static Godot.GeometryInstance3D;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ObjectOctreeLODMeshDefinitionResource : Resource {
    [Export] public Mesh Mesh { get;set; }
    [Export] public Material MaterialOverride { get;set; }
    [Export(PropertyHint.Link)] public Vector3 Scale { get;set; } = Vector3.One;
    [Export] public ShadowCastingSetting CastShadow { get;set; } = ShadowCastingSetting.On;    
}
