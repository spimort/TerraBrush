using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ObjectDefinitionResource : Resource {
    private Variant _godotHandle;

    public ObjectStrategy Strategy {
        get {
            return (ObjectStrategy) _godotHandle.AsGodotObject().Call("get_strategy").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_strategy", (int) value);
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

    public float RandomRange {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_randomRange").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomRange", value);
        }
    }

    public Texture2D NoiseTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_noiseTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_noiseTexture", value);
        }
    }

    public bool RandomYRotation {
        get {
            return _godotHandle.AsGodotObject().Call("get_randomYRotation").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomYRotation", value);
        }
    }

    public bool RandomSize {
        get {
            return _godotHandle.AsGodotObject().Call("get_randomSize").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomSize", value);
        }
    }

    public float RandomSizeFactorMin {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_randomSizeFactorMin").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomSizeFactorMin", value);
        }
    }

    public float RandomSizeFactorMax {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_randomSizeFactorMax").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomSizeFactorMax", value);
        }
    }

    public PackedScene[] ObjectScenes {
        get {
            return _godotHandle.AsGodotObject().Call("get_objectScenes").AsGodotObjectArray<PackedScene>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectScenes", value);
        }
    }

    public ObjectOctreeLODDefinitionResource[] LODList {
        get {
            return _godotHandle.AsGodotObject().Call("get_lodList").AsGodotArray<Variant>()?.Select(x => new ObjectOctreeLODDefinitionResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_lodList", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public ObjectOctreeLODMeshesDefinitionResource[] LODMeshes {
        get {
            return _godotHandle.AsGodotObject().Call("get_lodMeshes").AsGodotArray<Variant>()?.Select(x => new ObjectOctreeLODMeshesDefinitionResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_lodMeshes", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public float UpdateDistanceThreshold {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_updateDistanceThreshold").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_updateDistanceThreshold", value);
        }
    }

    public float UpdateTimeFrequency {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_updateTimeFrequency").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_updateTimeFrequency", value);
        }
    }

    public int VisualInstanceLayers {
        get {
            return _godotHandle.AsGodotObject().Call("get_visualInstanceLayers").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_visualInstanceLayers", value);
        }
    }

    public static implicit operator ObjectDefinitionResource(Variant handle) => new(handle);

    public ObjectDefinitionResource(Variant handle) {
        _godotHandle = handle;
    }

    public ObjectDefinitionResource() {
        _godotHandle = ClassDB.Instantiate("ObjectDefinitionResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
