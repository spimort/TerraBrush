using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class SnowResource : Resource {
    private ShaderMaterial _customShader;

    [BindProperty] public float SnowFactor { get;set; } = 1;
    [BindProperty] public float SnowInnerOffset { get;set; } = 0.3f;
    [BindProperty] public Texture2D SnowColorTexture { get;set; }
    [BindProperty] public Texture2D SnowColorNormal { get;set; }
    [BindProperty] public Texture2D SnowColorRoughness { get;set; }
    [BindProperty] public float SnowColorDetail { get;set; } = 20f;
    [BindProperty] public Texture2D Noise { get;set; }
    [BindProperty] public float NoiseFactor { get;set; }
    [BindProperty] public float Metallic { get;set; } // (PropertyHint.Range, "0,1,0.001")
    [BindProperty] public int VisualInstanceLayers { get;set; } = 1; // (PropertyHint.Layers3DRender)

    [BindProperty] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoaderHelper.Load<Shader>("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
