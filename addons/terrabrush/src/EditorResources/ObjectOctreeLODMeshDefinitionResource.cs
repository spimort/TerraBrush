using Godot;
using static Godot.GeometryInstance3D;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ObjectOctreeLODMeshDefinitionResource : Resource {
    [BindProperty] public Mesh Mesh { get;set; }
    [BindProperty] public Material MaterialOverride { get;set; }
    [BindProperty(Hint = PropertyHint.Link)] public Vector3 Scale { get;set; } = Vector3.One;
    [BindProperty] public ShadowCastingSetting CastShadow { get;set; } = ShadowCastingSetting.On;
}
