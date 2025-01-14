using Godot;

namespace TerraBrush;

public partial class SnowResource : Resource {
    private ShaderMaterial _customShader;

    public float SnowFactor { get;set; } = 1;
    public float SnowInnerOffset { get;set; } = 0.3f;
    public Texture2D SnowColorTexture { get;set; }
    public Texture2D SnowColorNormal { get;set; }
    public Texture2D SnowColorRoughness { get;set; }
    public float SnowColorDetail { get;set; } = 20f;
    public Texture2D Noise { get;set; }
    public float NoiseFactor { get;set; }
    [Export(PropertyHint.Range, "0,1,0.001")] public float Metallic { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = (Shader) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
