using System;
using Godot;

namespace TerraBrush;

public partial class WaterResource : Resource {
    private ShaderMaterial _customShader;

    public float WaterFactor { get;set; } = 1.0f;
    public float WaterInnerOffset { get;set; } = 0.2f;
    public Color WaterColor { get;set; } = NamedColors.White;
    public Color WaterFresnelColor { get;set; } = NamedColors.White;
    public float WaterMetallic { get;set; }
    public float WaterRoughness { get;set; }
    public Texture2D WaterNormalMap { get;set; }
    public Texture2D WaterNormalMap2 { get;set; }
    public float WaterTimeScale { get;set; } = 0.1f;
    public float WaterStrength { get;set; } = 0.4f;
    public Texture2D WaterWave { get;set; }
    public float WaterNoiseScale { get;set; } = 10.0f;
    public float WaterHeightScale { get;set; } = 0.15f;
    public Color WaterColorDeep { get;set; } = NamedColors.White;
    public Color WaterColorShallow { get;set; } = NamedColors.White;
    public float WaterBeersLaw { get;set; } = 2.0f;
    public float WaterDepthOffset { get;set; } = -0.75f;
    public float WaterEdgeScale { get;set; } = 0.1f;
    public float WaterNear { get;set; } = 0.5f;
    public float WaterFar { get;set; } = 100f;
    public Color WaterEdgeColor { get;set; } = NamedColors.White;
    public int VisualInstanceLayers { get;set; } = 1;

    public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = (Shader) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
