using Godot;

namespace TerraBrush;

public partial class MetaInfoLayerResource : Resource {
    private Variant _godotHandle;

    public string Name {
        get {
            return _godotHandle.AsGodotObject().Call("get_name").AsString();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_name", value);
        }
    }

    public Color Color {
        get {
            return _godotHandle.AsGodotObject().Call("get_color").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_color", value);
        }
    }

    public static implicit operator MetaInfoLayerResource(Variant handle) => new(handle);

    public MetaInfoLayerResource(Variant handle) {
        _godotHandle = handle;
    }

    public MetaInfoLayerResource() {
        _godotHandle = ClassDB.Instantiate("MetaInfoLayerResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
