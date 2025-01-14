using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageDefinitionResource : Resource {
    private static readonly List<string> _multimeshProperties = new List<string> {
        nameof(LODLevels),
        nameof(LODRowsPerLevel),
        nameof(LODInitialCellWidth),
        nameof(Albedo),
        nameof(AlbedoTextures),
        nameof(UseGroundColor),
        nameof(CastShadow),
        nameof(UseBrushScale),
        nameof(ScaleNoiseTexture),
        nameof(RandomPlacementRange),
        nameof(CustomShader),
    };

    private static readonly List<string> _gpuParticlesProperties = new List<string> {
        nameof(MeshMaterial),
        nameof(MaximumRenderDistance),
        nameof(EditorMaximumRenderDistance),
    };

    private FoliageStrategy _strategy = FoliageStrategy.MultiMesh;
    private ShaderMaterial _customShader;

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
    [Export] public bool UseGroundColor { get;set; }
    [Export] public bool CastShadow { get;set; } = false;
    [Export] public bool UseBrushScale { get;set; } = true;
    [Export] public Texture2D ScaleNoiseTexture { get;set; }
    [Export] public float RandomPlacementRange { get;set; } = 3.0f;
    [Export] public int[] ApplyOnTextureIndexes { get;set; }
    [Export] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
    // Particles settings
    [Export] public Material MeshMaterial { get;set; }
    [Export] public int MaximumRenderDistance { get;set; } = 50;
    [Export] public int EditorMaximumRenderDistance { get;set; } = 50;

    public override void _ValidateProperty(Dictionary property) {
        base._ValidateProperty(property);

        if (Strategy == FoliageStrategy.MultiMesh) {
            if (_gpuParticlesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            } else if (_multimeshProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Default;
            }
        } else if (Strategy == FoliageStrategy.GPUParticle) {
            if (_gpuParticlesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Default;
            } else if (_multimeshProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            }
        }
    }
}
