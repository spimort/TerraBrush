using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageDefinitionResource : Resource {
    private static readonly List<string> _multimeshProperties = [
        nameof(LODLevels),
        nameof(LODRowsPerLevel),
        nameof(LODInitialCellWidth),
        nameof(Albedo),
        nameof(AlbedoTextures),
        nameof(UseGroundColor),
        nameof(CastShadow),
        nameof(UseBrushScale),
        nameof(ScaleNoiseTexture),
        nameof(RandomPlacementRange)
    ];

    private static readonly List<string> _gpuParticlesProperties = [
        nameof(MeshMaterial),
        nameof(MaximumRenderDistance),
        nameof(EditorMaximumRenderDistance),
    ];

    private FoliageStrategy _strategy = FoliageStrategy.MultiMesh;

    [Export] public FoliageStrategy Strategy {
        get {
            return _strategy;
        }
        set {
            _strategy = value;
            NotifyPropertyListChanged();
        }
    }
    [Export] public Mesh Mesh { get;set; }
    [Export(PropertyHint.Link)] public Vector3 MeshScale { get;set; } = new Vector3(1, 1, 1);
    [Export] public float WindStrength { get;set; } = 0.1f;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
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
    [Export] public Material MeshMaterial { get;set; }
    [Export] public int MaximumRenderDistance { get;set; } = 50;
    [Export] public int EditorMaximumRenderDistance { get;set; } = 50;

    public FoliageDefinitionResource() {
        Strategy = 0; // Because of the legacy, we default it to 0 so we can detect that this is an old value
    }

    public override void _ValidateProperty(Dictionary property) {
        base._ValidateProperty(property);

        if (Strategy == FoliageStrategy.MultiMesh || Strategy == 0) { // 0 is considered as Multimesh
            if (_gpuParticlesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            } else if (_multimeshProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Editor;
            }
        } else if (Strategy == FoliageStrategy.GPUParticle) {
            if (_gpuParticlesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Editor;
            } else if (_multimeshProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            }
        }
    }
}
