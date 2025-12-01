using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ObjectOctreeLODMeshesDefinitionResource : Resource {
    private Variant _godotHandle;

    public ObjectOctreeLODMeshDefinitionResource[] Meshes {
        get {
            return _godotHandle.AsGodotObject().Call("get_meshes").AsGodotArray<Variant>()?.Select(x => new ObjectOctreeLODMeshDefinitionResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_meshes", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public Shape3D CollisionShape {
        get {
            return _godotHandle.AsGodotObject().Call("get_collisionShape").As<Shape3D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_collisionShape", value);
        }
    }

    public Vector3 CollisionOffset {
        get {
            return _godotHandle.AsGodotObject().Call("get_collisionOffset").As<Vector3>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_collisionOffset", value);
        }
    }

    public static implicit operator ObjectOctreeLODMeshesDefinitionResource(Variant handle) => new(handle);

    public ObjectOctreeLODMeshesDefinitionResource(Variant handle) {
        _godotHandle = handle;
    }

    public ObjectOctreeLODMeshesDefinitionResource() {
        _godotHandle = ClassDB.Instantiate("ObjectOctreeLODMeshesDefinitionResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
