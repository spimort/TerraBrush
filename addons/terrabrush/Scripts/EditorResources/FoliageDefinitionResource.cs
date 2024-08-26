using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageDefinitionResource : Resource {
    [Export] public FoliageStrategy Strategy { get;set; } = FoliageStrategy.MultiMesh;
    [Export] public Mesh Mesh { get;set; }
    // Multimesh settings
    [Export] public int LODLevels { get;set; } = 3;
    [Export] public int LODRowsPerLevel { get;set; } = 50;
    [Export] public float LODInitialCellWidth { get;set; } = 1;
    [Export] public Color Albedo { get;set; } = Colors.White;
    [Export] public Texture2D[] AlbedoTextures { get;set; }
    [Export] public bool UseGroundColor { get;set; } = true;
    [Export] public bool CastShadow { get;set; } = false;
    [Export] public bool UseBrushScale { get;set; } = true;
    [Export] public Texture2D ScaleNoiseTexture { get;set; }
    [Export] public float RandomPlacementRange { get;set; } = 3.0f;
    // Particles settings
    [Export] public Vector3 MeshScale { get;set; } = new Vector3(1, 1, 1);
    [Export] public Material MeshMaterial { get;set; }
    [Export] public int MaximumRenderDistance { get;set; } = 50;
    [Export] public int EditorMaximumRenderDistance { get;set; } = 50;
    // General settings
    [Export] public float WindStrength { get;set; } = 0.1f;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
}
