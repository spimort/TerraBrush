using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

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

    public FoliageStrategy Strategy {
        get {
            return _strategy;
        }
        set {
            _strategy = value;
            NotifyPropertyListChanged();
        }
    }
    public Mesh Mesh { get;set; }
    public Vector3 MeshScale { get;set; } = new Vector3(1, 1, 1);
    public float WindStrength { get;set; } = 0.1f;
    public Texture2D NoiseTexture { get;set; }
    public int VisualInstanceLayers { get;set; } = 1;
    // Multimesh settings
    public int LODLevels { get;set; } = 3;
    public int LODRowsPerLevel { get;set; } = 50;
    public float LODInitialCellWidth { get;set; } = 1;
    public Color Albedo { get;set; } = NamedColors.White;
    public Texture2D[] AlbedoTextures { get;set; }
    public bool UseGroundColor { get;set; }
    public bool CastShadow { get;set; } = false;
    public bool UseBrushScale { get;set; } = true;
    public Texture2D ScaleNoiseTexture { get;set; }
    public float RandomPlacementRange { get;set; } = 3.0f;
    public int[] ApplyOnTextureIndexes { get;set; }
    public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = (Shader) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
    // Particles settings
    public Material MeshMaterial { get;set; }
    public int MaximumRenderDistance { get;set; } = 50;
    public int EditorMaximumRenderDistance { get;set; } = 50;

    protected override void _ValidateProperty(Dictionary property) {
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
