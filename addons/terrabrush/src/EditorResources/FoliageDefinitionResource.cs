using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class FoliageDefinitionResource : Resource {
    private static readonly List<string> _multimeshProperties = new List<string> {
        nameof(LODLevels),
        nameof(LODRowsPerLevel),
        nameof(LODInitialCellWidth),
        nameof(Albedo),
        nameof(AlbedoTextures),
        nameof(NearestTextureFilter),
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

    [BindProperty] public FoliageStrategy Strategy {
        get {
            return _strategy;
        }
        set {
            _strategy = value;
            NotifyPropertyListChanged();
        }
    }
    [BindProperty] public Mesh Mesh { get;set; }
    [BindProperty] public Vector3 MeshScale { get;set; } = new Vector3(1, 1, 1); // (PropertyHint.Link)
    [BindProperty] public float WindStrength { get;set; } = 0.1f;
    [BindProperty] public Texture2D NoiseTexture { get;set; }
    [BindProperty] public int VisualInstanceLayers { get;set; } = 1; // (PropertyHint.Layers3DRender)
    // Multimesh settings
    [BindProperty] public int LODLevels { get;set; } = 3;
    [BindProperty] public int LODRowsPerLevel { get;set; } = 50;
    [BindProperty] public float LODInitialCellWidth { get;set; } = 1;
    [BindProperty] public Color Albedo { get;set; } = NamedColors.White;
    [BindProperty] public GodotArray<Texture2D> AlbedoTextures { get;set; }
    [BindProperty] public bool NearestTextureFilter { get;set; }
    [BindProperty] public bool UseGroundColor { get;set; }
    [BindProperty] public bool CastShadow { get;set; } = false;
    [BindProperty] public bool UseBrushScale { get;set; } = true;
    [BindProperty] public Texture2D ScaleNoiseTexture { get;set; }
    [BindProperty] public float RandomPlacementRange { get;set; } = 3.0f;
    [BindProperty] public GodotArray<int> ApplyOnTextureIndexes { get;set; }
    [BindProperty] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoaderHelper.Load<Shader>("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
    // Particles settings
    [BindProperty] public Material MeshMaterial { get;set; }
    [BindProperty] public int MaximumRenderDistance { get;set; } = 50;
    [BindProperty] public int EditorMaximumRenderDistance { get;set; } = 50;

    // TODO
    // protected override void _ValidateProperty(GodotDictionary property) {
    //     base._ValidateProperty(property);

    //     if (Strategy == FoliageStrategy.MultiMesh) {
    //         if (_gpuParticlesProperties.Contains((string)property["name"])) {
    //             property["usage"] = (long)PropertyUsageFlags.NoEditor;
    //         }
    //         else if (_multimeshProperties.Contains((string)property["name"])) {
    //             property["usage"] = (long)PropertyUsageFlags.Default;
    //         }
    //     }
    //     else if (Strategy == FoliageStrategy.GPUParticle) {
    //         if (_gpuParticlesProperties.Contains((string)property["name"])) {
    //             property["usage"] = (long)PropertyUsageFlags.Default;
    //         }
    //         else if (_multimeshProperties.Contains((string)property["name"])) {
    //             property["usage"] = (long)PropertyUsageFlags.NoEditor;
    //         }
    //     }
    // }
}
