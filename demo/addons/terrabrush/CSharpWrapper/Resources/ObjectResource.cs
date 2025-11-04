using Godot;

namespace TerraBrush;

public partial class ObjectResource : Resource {
    private Variant _godotHandle;

    public ObjectDefinitionResource Definition {
        get {
            return _godotHandle.AsGodotObject().Call("get_definition");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_definition", value == null ? default : value.GodotHandle());
        }
    }

    public bool Hide {
        get {
            return _godotHandle.AsGodotObject().Call("get_hide").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_hide", value);
        }
    }

    public static implicit operator ObjectResource(Variant handle) => new(handle);

    public ObjectResource(Variant handle) {
        _godotHandle = handle;
    }

    public ObjectResource() {
        _godotHandle = ClassDB.Instantiate("ObjectResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
