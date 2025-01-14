using System;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class WaterResource : Resource {
    private ShaderMaterial _customShader;

    [Export] public float WaterFactor { get;set; } = 1.0f;
    [Export] public float WaterInnerOffset { get;set; } = 0.2f;
    [Export(PropertyHint.ColorNoAlpha)] public Color WaterColor { get;set; } = Colors.White;
    [Export(PropertyHint.ColorNoAlpha)] public Color WaterFresnelColor { get;set; } = Colors.White;
    [Export(PropertyHint.Range, "0,1,0.001")] public float WaterMetallic { get;set; }
    [Export(PropertyHint.Range, "0,1,0.001")] public float WaterRoughness { get;set; }
    [Export] public Texture2D WaterNormalMap { get;set; }
    [Export] public Texture2D WaterNormalMap2 { get;set; }
    [Export] public float WaterTimeScale { get;set; } = 0.1f;
    [Export] public float WaterStrength { get;set; } = 0.4f;
    [Export] public Texture2D WaterWave { get;set; }
    [Export] public float WaterNoiseScale { get;set; } = 10.0f;
    [Export] public float WaterHeightScale { get;set; } = 0.15f;
    [Export] public Color WaterColorDeep { get;set; } = Colors.White;
    [Export] public Color WaterColorShallow { get;set; } = Colors.White;
    [Export] public float WaterBeersLaw { get;set; } = 2.0f;
    [Export] public float WaterDepthOffset { get;set; } = -0.75f;
    [Export] public float WaterEdgeScale { get;set; } = 0.1f;
    [Export] public float WaterNear { get;set; } = 0.5f;
    [Export] public float WaterFar { get;set; } = 100f;
    [Export(PropertyHint.ColorNoAlpha)] public Color WaterEdgeColor { get;set; } = Colors.White;
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    [Export] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }
}
