using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ZonesResource : Resource {
    private Variant _godotHandle;

    public ZoneResource[] Zones {
        get {
            return _godotHandle.AsGodotObject().Call("get_zones").AsGodotArray<Variant>()?.Select(x => new ZoneResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_zones", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public static implicit operator ZonesResource(Variant handle) => new(handle);

    public ZonesResource(Variant handle) {
        _godotHandle = handle;
    }

    public ZonesResource() {
        _godotHandle = ClassDB.Instantiate("ZonesResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
