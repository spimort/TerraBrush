using Godot;
using static Godot.GeometryInstance3D;

namespace TerraBrush;

public partial class ObjectOctreeLODMeshDefinitionResource : Resource {
    private Variant _godotHandle;

    public Mesh Mesh {
        get {
            return _godotHandle.AsGodotObject().Call("get_mesh").As<Mesh>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_mesh", value);
        }
    }

    public Material MaterialOverride {
        get {
            return _godotHandle.AsGodotObject().Call("get_materialOverride").As<Material>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_materialOverride", value);
        }
    }

    public Vector3 Scale {
        get {
            return _godotHandle.AsGodotObject().Call("get_scale").AsVector3();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_scale", value);
        }
    }

    public ShadowCastingSetting CastShadow {
        get {
            return (ShadowCastingSetting) _godotHandle.AsGodotObject().Call("get_castShadow").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_castShadow", (int) value);
        }
    }

    public static implicit operator ObjectOctreeLODMeshDefinitionResource(Variant handle) => new(handle);

    public ObjectOctreeLODMeshDefinitionResource(Variant handle) {
        _godotHandle = handle;
    }

    public ObjectOctreeLODMeshDefinitionResource() {
        _godotHandle = ClassDB.Instantiate("ObjectOctreeLODMeshDefinitionResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
