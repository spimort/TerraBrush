using System;
using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class WaterResource : Resource {
    private ShaderMaterial _customShader;

    [BindProperty] public float WaterFactor { get;set; } = 1.0f;
    [BindProperty] public float WaterInnerOffset { get;set; } = 0.2f;
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color WaterColor { get;set; } = NamedColors.White;
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color WaterFresnelColor { get;set; } = NamedColors.White;
    [BindProperty(Hint = PropertyHint.Range, HintString = "0,1,0.001")] public float WaterMetallic { get;set; }
    [BindProperty(Hint = PropertyHint.Range, HintString = "0,1,0.001")] public float WaterRoughness { get;set; }
    [BindProperty] public Texture2D WaterNormalMap { get;set; }
    [BindProperty] public Texture2D WaterNormalMap2 { get;set; }
    [BindProperty] public float WaterTimeScale { get;set; } = 0.1f;
    [BindProperty] public float WaterStrength { get;set; } = 0.4f;
    [BindProperty] public Texture2D WaterWave { get;set; }
    [BindProperty] public float WaterNoiseScale { get;set; } = 10.0f;
    [BindProperty] public float WaterHeightScale { get;set; } = 0.15f;
    [BindProperty] public Color WaterColorDeep { get;set; } = NamedColors.White;
    [BindProperty] public Color WaterColorShallow { get;set; } = NamedColors.White;
    [BindProperty] public float WaterBeersLaw { get;set; } = 2.0f;
    [BindProperty] public float WaterDepthOffset { get;set; } = -0.75f;
    [BindProperty] public float WaterEdgeScale { get;set; } = 0.1f;
    [BindProperty] public float WaterNear { get;set; } = 0.5f;
    [BindProperty] public float WaterFar { get;set; } = 100f;
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color WaterEdgeColor { get;set; } = NamedColors.White;
    [BindProperty(Hint = PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    [BindProperty] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoaderHelper.Load<Shader>("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
