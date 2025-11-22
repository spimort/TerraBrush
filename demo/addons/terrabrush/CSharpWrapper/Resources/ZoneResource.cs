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

    public Image HeightMapTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_heightMapTexture").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_heightMapTexture", value);
        }
    }

    public Image[] SplatmapsTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_splatmapsTexture").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_splatmapsTexture", value);
        }
    }

    public Image[] FoliagesTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_foliagesTexture").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_foliagesTexture", value);
        }
    }

    public Image[] ObjectsTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_objectsTexture").AsGodotObjectArray<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectsTexture", value);
        }
    }

    public Image WaterTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterTexture").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterTexture", value);
        }
    }

    public Image SnowTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowTexture").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowTexture", value);
        }
    }

    public Image MetaInfoTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoTexture").As<Image>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metaInfoTexture", value);
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
