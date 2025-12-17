using System.Linq;
using Godot;

namespace TerraBrush;

public partial class TextureSetResource : Resource {
    private Variant _godotHandle;

    public string Name {
        get {
            return _godotHandle.AsGodotObject().Call("get_name").AsString();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_name", value);
        }
    }

    public Texture2D AlbedoTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_albedoTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_albedoTexture", value);
        }
    }

    public Texture2D NormalTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_normalTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_normalTexture", value);
        }
    }

    public Texture2D RoughnessTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_roughnessTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_roughnessTexture", value);
        }
    }

    public Texture2D HeightTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_heightTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_heightTexture", value);
        }
    }

    public int TextureDetail {
        get {
            return _godotHandle.AsGodotObject().Call("get_textureDetail").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_textureDetail", value);
        }
    }

    public bool Triplanar {
        get {
            return _godotHandle.AsGodotObject().Call("get_triplanar").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_triplanar", value);
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

    public float Specular {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_specular").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_specular", value);
        }
    }

    public static implicit operator TextureSetResource(Variant handle) => new(handle);

    public TextureSetResource(Variant handle) {
        _godotHandle = handle;
    }

    public TextureSetResource() {
        _godotHandle = ClassDB.Instantiate("TextureSetResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
