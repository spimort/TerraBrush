using System.Linq;
using Godot;

public partial class TerrainPositionInformation : RefCounted {
    private Variant _godotHandle;

    public TerrainPositionTextureInformation[] Textures {
        get {
            return _godotHandle.AsGodotObject().Call("get_textures").AsGodotArray<Variant>()?.Select(x => new TerrainPositionTextureInformation(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_textures", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public float WaterFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterFactor", value);
        }
    }

    public float WaterDeepness {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_waterDeepness").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterDeepness", value);
        }
    }

    public float SnowFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_snowFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowFactor", value);
        }
    }

    public float SnowHeight {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_snowHeight").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowHeight", value);
        }
    }

    public int MetaInfoIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metaInfoIndex", value);
        }
    }

    public string MetaInfoName {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoName").AsString();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metaInfoName", value);
        }
    }

    public static implicit operator TerrainPositionInformation(Variant handle) => new(handle);

    public TerrainPositionInformation(Variant handle) {
        _godotHandle = handle;
    }

    public TerrainPositionInformation() {
        _godotHandle = ClassDB.Instantiate("TerrainPositionInformation");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }
}

public partial class TerrainPositionTextureInformation : RefCounted {
    private Variant _godotHandle;

    public int Index {
        get {
            return _godotHandle.AsGodotObject().Call("get_index").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_index", value);
        }
    }

    public string Name {
        get {
            return _godotHandle.AsGodotObject().Call("get_name").AsString();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_name", value);
        }
    }

    public float Factor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_factor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_factor", value);
        }
    }

    public static implicit operator TerrainPositionTextureInformation(Variant handle) => new(handle);

    public TerrainPositionTextureInformation(Variant handle) {
        _godotHandle = handle;
    }

    public TerrainPositionTextureInformation() {
        _godotHandle = ClassDB.Instantiate("TerrainPositionTextureInformation");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }
}
