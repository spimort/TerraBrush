using System;
using Godot;

namespace TerraBrush;

public partial class WaterResource : Resource {
    private Variant _godotHandle;

    public float WaterFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterFactor", value);
        }
    }

    public float WaterInnerOffset {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterInnerOffset").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterInnerOffset", value);
        }
    }

    public Color WaterColor {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterColor").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterColor", value);
        }
    }

    public Color WaterFresnelColor {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterFresnelColor").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterFresnelColor", value);
        }
    }

    public float WaterMetallic {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterMetallic").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterMetallic", value);
        }
    }

    public float WaterRoughness {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterRoughness").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterRoughness", value);
        }
    }

    public Texture2D WaterNormalMap {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterNormalMap").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterNormalMap", value);
        }
    }

    public Texture2D WaterNormalMap2 {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterNormalMap2").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterNormalMap2", value);
        }
    }

    public float WaterTimeScale {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterTimeScale").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterTimeScale", value);
        }
    }

    public float WaterStrength {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterStrength").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterStrength", value);
        }
    }

    public Texture2D WaterWave {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterWave").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterWave", value);
        }
    }

    public float WaterNoiseScale {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterNoiseScale").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterNoiseScale", value);
        }
    }

    public float WaterHeightScale {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterHeightScale").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterHeightScale", value);
        }
    }

    public Color WaterColorDeep {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterColorDeep").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterColorDeep", value);
        }
    }

    public Color WaterColorShallow {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterColorShallow").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterColorShallow", value);
        }
    }

    public float WaterBeersLaw {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterBeersLaw").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterBeersLaw", value);
        }
    }

    public float WaterDepthOffset {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterDepthOffset").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterDepthOffset", value);
        }
    }

    public float WaterEdgeScale {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterEdgeScale").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterEdgeScale", value);
        }
    }

    public float WaterNear {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterNear").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterNear", value);
        }
    }

    public float WaterFar {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterFar").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterFar", value);
        }
    }

    public Color WaterEdgeColor {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterEdgeColor").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterEdgeColor", value);
        }
    }

    public int VisualInstanceLayers {
        get {
            return _godotHandle.AsGodotObject().Call("get_visualInstanceLayers").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_visualInstanceLayers", value);
        }
    }

    public ShaderMaterial CustomShader {
        get {
            return _godotHandle.AsGodotObject().Call("get_customShader").As<ShaderMaterial>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_customShader", value);
        }
    }

    public float RippleResetSpeed {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_rippleResetSpeed").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_rippleResetSpeed", value);
        }
    }

    public static implicit operator WaterResource(Variant handle) => new(handle);

    public WaterResource(Variant handle) {
        _godotHandle = handle;
    }

    public WaterResource() {
        _godotHandle = ClassDB.Instantiate("WaterResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
