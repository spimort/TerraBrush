using Godot;

namespace TerraBrush;

public partial class SnowResource : Resource {
    private Variant _godotHandle;

    public float SnowFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_snowFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowFactor", value);
        }
    }

    public float SnowInnerOffset {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_snowInnerOffset").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowInnerOffset", value);
        }
    }

    public Texture2D SnowColorTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowColorTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowColorTexture", value);
        }
    }

    public Texture2D SnowColorNormal {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowColorNormal").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowColorNormal", value);
        }
    }

    public Texture2D SnowColorRoughness {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowColorRoughness").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowColorRoughness", value);
        }
    }

    public float SnowColorDetail {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_snowColorDetail").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowColorDetail", value);
        }
    }

    public Texture2D Noise {
        get {
            return _godotHandle.AsGodotObject().Call("get_noise").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_noise", value);
        }
    }

    public float NoiseFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_noiseFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_noiseFactor", value);
        }
    }

    public float Metallic {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_metallic").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metallic", value);
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

    public float DecompressSpeed {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_decompressSpeed").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_decompressSpeed", value);
        }
    }

    public float DecompressOffsetSpeed {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_decompressOffsetSpeed").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_decompressOffsetSpeed", value);
        }
    }

    public static implicit operator SnowResource(Variant handle) => new(handle);

    public SnowResource(Variant handle) {
        _godotHandle = handle;
    }

    public SnowResource() {
        _godotHandle = ClassDB.Instantiate("SnowResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
