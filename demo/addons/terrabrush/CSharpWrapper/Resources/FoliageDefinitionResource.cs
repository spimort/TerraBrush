using Godot;

namespace TerraBrush;

public partial class FoliageDefinitionResource : Resource {
    private Variant _godotHandle;

    public FoliageStrategy Strategy {
        get {
            return (FoliageStrategy)_godotHandle.AsGodotObject().Call("get_strategy").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_strategy", (int)value);
        }
    }

    public Mesh Mesh {
        get {
            return _godotHandle.AsGodotObject().Call("get_mesh").As<Mesh>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_mesh", value);
        }
    }

    public Vector3 MeshScale {
        get {
            return _godotHandle.AsGodotObject().Call("get_meshScale").AsVector3();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_meshScale", value);
        }
    }

    public float WindStrength {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_windStrength").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_windStrength", value);
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

    public int VisualInstanceLayers {
        get {
            return _godotHandle.AsGodotObject().Call("get_visualInstanceLayers").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_visualInstanceLayers", value);
        }
    }

    public int LODLevels {
        get {
            return _godotHandle.AsGodotObject().Call("get_lodLevels").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_lodLevels", value);
        }
    }

    public int LODRowsPerLevel {
        get {
            return _godotHandle.AsGodotObject().Call("get_lodRowsPerLevel").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_lodRowsPerLevel", value);
        }
    }

    public float LODInitialCellWidth {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_lodInitialCellWidth").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_lodInitialCellWidth", value);
        }
    }

    public Color Albedo {
        get {
            return _godotHandle.AsGodotObject().Call("get_albedo").AsColor();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_albedo", value);
        }
    }

    public Texture2D[] AlbedoTextures {
        get {
            return _godotHandle.AsGodotObject().Call("get_albedoTextures").AsGodotObjectArray<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_albedoTextures", value);
        }
    }

    public bool NearestTextureFilter {
        get {
            return _godotHandle.AsGodotObject().Call("get_nearestTextureFilter").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_nearestTextureFilter", value);
        }
    }

    public bool UseGroundColor {
        get {
            return _godotHandle.AsGodotObject().Call("get_useGroundColor").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_useGroundColor", value);
        }
    }

    public bool CastShadow {
        get {
            return _godotHandle.AsGodotObject().Call("get_castShadow").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_castShadow", value);
        }
    }

    public bool UseBrushScale {
        get {
            return _godotHandle.AsGodotObject().Call("get_useBrushScale").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_useBrushScale", value);
        }
    }

    public Texture2D ScaleNoiseTexture {
        get {
            return _godotHandle.AsGodotObject().Call("get_scaleNoiseTexture").As<Texture2D>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_scaleNoiseTexture", value);
        }
    }

    public float RandomPlacementRange {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_randomPlacementRange").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_randomPlacementRange", value);
        }
    }

    public int[] ApplyOnTextureIndexes {
        get {
            return _godotHandle.AsGodotObject().Call("get_applyOnTextureIndexes").AsInt32Array();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_applyOnTextureIndexes", value);
        }
    }

    public ShaderMaterial CustomShader {
        get {
            return _godotHandle.AsGodotObject().Call("get_customShader").As<ShaderMaterial>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_customShader", value);
        }
    }

    public Material MeshMaterial {
        get {
            return _godotHandle.AsGodotObject().Call("get_meshMaterial").As<Material>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_meshMaterial", value);
        }
    }

    public int MaximumRenderDistance {
        get {
            return _godotHandle.AsGodotObject().Call("get_maximumRenderDistance").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_maximumRenderDistance", value);
        }
    }

    public int EditorMaximumRenderDistance {
        get {
            return _godotHandle.AsGodotObject().Call("get_editorMaximumRenderDistance").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_editorMaximumRenderDistance", value);
        }
    }

    public static implicit operator FoliageDefinitionResource(Variant handle) => new(handle);

    public FoliageDefinitionResource(Variant handle) {
        _godotHandle = handle;
    }

    public FoliageDefinitionResource() {
        _godotHandle = ClassDB.Instantiate("FoliageDefinitionResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
