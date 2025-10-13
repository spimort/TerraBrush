using Godot;

namespace TerraBrush;

public partial class FoliageResource : Resource {
    private Variant _godotHandle;

    public FoliageDefinitionResource Definition {
        get {
            return _godotHandle.AsGodotObject().Call("get_definition");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_definition", value == null ? default : value.GodotHandle());
        }
    }

    public static implicit operator FoliageResource(Variant handle) => new(handle);

    public FoliageResource(Variant handle) {
        _godotHandle = handle;
    }

    public FoliageResource() {
        _godotHandle = ClassDB.Instantiate("FoliageResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
