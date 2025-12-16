using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ZoneResource : Resource {
    private Variant _godotHandle;

    public Vector2I ZonePosition {
        get {
            return _godotHandle.AsGodotObject().Call("get_zonePosition").AsVector2I();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_zonePosition", value);
        }
    }

    public Image HeightMapImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_heightMapImage").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_heightMapImage", value);
        }
    }

    public Image ColorImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_colorImage").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_colorImage", value);
        }
    }

    public Image[] SplatmapsImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_splatmapsImage").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_splatmapsImage", value);
        }
    }

    public Image[] FoliagesImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_foliagesImage").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_foliagesImage", value);
        }
    }

    public Image[] ObjectsImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_objectsImage").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectsImage", value);
        }
    }

    public Image WaterImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterImage").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterImage", value);
        }
    }

    public Image SnowImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowImage").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowImage", value);
        }
    }

    public Image MetaInfoImage {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoImage").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metaInfoImage", value);
        }
    }

    public static implicit operator ZoneResource(Variant handle) => new(handle);

    public ZoneResource(Variant handle) {
        _godotHandle = handle;
    }

    public ZoneResource() {
        _godotHandle = ClassDB.Instantiate("ZoneResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
