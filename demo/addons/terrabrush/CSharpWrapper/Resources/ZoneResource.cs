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

    public ImageTexture HeightMapTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_heightMapTexture").As<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_heightMapTexture", value);
        }
    }

    public ImageTexture[] SplatmapsTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_splatmapsTexture").AsGodotObjectArray<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_splatmapsTexture", value);
        }
    }

    public ImageTexture[] FoliagesTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_foliagesTexture").AsGodotObjectArray<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_foliagesTexture", value);
        }
    }

    public ImageTexture[] ObjectsTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_objectsTexture").AsGodotObjectArray<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectsTexture", value);
        }
    }

    public ImageTexture WaterTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterTexture").As<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterTexture", value);
        }
    }

    public ImageTexture SnowTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowTexture").As<ImageTexture>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowTexture", value);
        }
    }

    public ImageTexture MetaInfoTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoTexture").As<ImageTexture>();
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
