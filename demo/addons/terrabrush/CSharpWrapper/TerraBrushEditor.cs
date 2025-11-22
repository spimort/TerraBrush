using System.Linq;
using Godot;

namespace TerraBrush;

public partial class TerraBrushEditor : Node {
    private Variant _godotHandle;

    public static implicit operator TerraBrushEditor(Variant handle) => new(handle);

    public bool Enabled {
        get {
            return _godotHandle.AsGodotObject().Call("get_enabled").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_enabled", value);
        }
    }

    public bool EnableOnReady {
        get {
            return _godotHandle.AsGodotObject().Call("get_enableOnReady").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_enableOnReady", value);
        }
    }

    public bool AllowBuiltInToolSelectors {
        get {
            return _godotHandle.AsGodotObject().Call("get_allowBuiltInToolSelectors").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_allowBuiltInToolSelectors", value);
        }
    }

    public TerraBrush TerraBrushNode {
        get {
            var node = _godotHandle.AsGodotObject().Call("get_terraBrushNode").As<Node>();
            return node == null ? null : new TerraBrush(node);
        }
        set {
            _godotHandle.AsGodotObject().Call("set_terraBrushNode", value.GodotHandle());
        }
    }

    public int BrushIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_brushIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_brushIndex", value);
        }
    }

    public int BrushSize {
        get {
            return _godotHandle.AsGodotObject().Call("get_brushSize").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_brushSize", value);
        }
    }

    public float BrushStrength {
        get {
            return _godotHandle.AsGodotObject().Call("get_brushStrength").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_brushStrength", value);
        }
    }

    public TerrainToolType SelectedToolType {
        get {
            return (TerrainToolType) _godotHandle.AsGodotObject().Call("get_selectedToolType").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_selectedToolType", (int) value);
        }
    }

    public int SelectedTextureIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_selectedTextureIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_selectedTextureIndex", value);
        }
    }

    public int SelectedFoliageIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_selectedFoliageIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_selectedFoliageIndex", value);
        }
    }

    public int SelectedObjectIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_selectedObjectIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_selectedObjectIndex", value);
        }
    }

    public int SelectedMetaInfoIndex {
        get {
            return _godotHandle.AsGodotObject().Call("get_selectedMetaInfoIndex").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_selectedMetaInfoIndex", value);
        }
    }

    public bool AutoAddZones {
        get {
            return _godotHandle.AsGodotObject().Call("get_autoAddZones").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_autoAddZones", value);
        }
    }

    public TerraBrushEditor(Variant handle) {
        _godotHandle = handle;
    }

    public TerraBrushEditor() {
        _godotHandle = ClassDB.Instantiate("TerraBrushEditor");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}