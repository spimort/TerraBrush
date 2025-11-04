using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODDefinitionResource : Resource {
    private Variant _godotHandle;

    public float MaxDistance {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_maxDistance").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_maxDistance", value);
        }
    }

    public bool AddCollision {
        get {
            return _godotHandle.AsGodotObject().Call("get_addCollision").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_addCollision", value);
        }
    }

    public int ObjectFrequency {
        get {
            return _godotHandle.AsGodotObject().Call("get_objectFrequency").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectFrequency", value);
        }
    }

    public static implicit operator ObjectOctreeLODDefinitionResource(Variant handle) => new(handle);

    public ObjectOctreeLODDefinitionResource(Variant handle) {
        _godotHandle = handle;
    }

    public ObjectOctreeLODDefinitionResource() {
        _godotHandle = ClassDB.Instantiate("ObjectOctreeLODDefinitionResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
