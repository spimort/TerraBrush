using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class SnowResource : Resource {
    private ShaderMaterial _customShader;

    [Export] public float SnowFactor { get;set; } = 1;
    [Export] public float SnowInnerOffset { get;set; } = 0.3f;
    [Export] public Texture2D SnowColorTexture { get;set; }
    [Export] public Texture2D SnowColorNormal { get;set; }
    [Export] public Texture2D SnowColorRoughness { get;set; }
    [Export] public float SnowColorDetail { get;set; } = 20f;
    [Export] public Texture2D Noise { get;set; }
    [Export] public float NoiseFactor { get;set; }
    [Export(PropertyHint.Range, "0,1,0.001")] public float Metallic { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    [Export] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
