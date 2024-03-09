using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageDefinitionResource : Resource {
    [Export] public Mesh Mesh { get;set; }
    [Export] public Material MeshMaterial { get;set; }
    [Export] public Vector3 MeshScale { get;set; } = new Vector3(1, 1, 1);
    [Export] public int MaximumRenderDistance { get;set; } = 50;
    [Export] public int EditorMaximumRenderDistance { get;set; } = 50;
    [Export] public float WindStrength { get;set; } = 0.1f;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
}
