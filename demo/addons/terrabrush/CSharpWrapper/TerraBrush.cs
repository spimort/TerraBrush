using System.Linq;
using Godot;

namespace TerraBrush;

public partial class TerraBrush : Node {
    private Variant _godotHandle;

    public string Version => _godotHandle.AsGodotObject().Call("get_version").AsString();

    public int ZonesSize {
        get {
            return _godotHandle.AsGodotObject().Call("get_zonesSize").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_zonesSize", value);
        }
    }

    public int Resolution {
        get {
            return _godotHandle.AsGodotObject().Call("get_resolution").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_resolution", value);
        }
    }

    public bool CollisionOnly {
        get {
            return _godotHandle.AsGodotObject().Call("get_collisionOnly").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_collisionOnly", value);
        }
    }

    public string DataPath {
        get {
            return _godotHandle.AsGodotObject().Call("get_dataPath").AsString();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_dataPath", value);
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

    public ShaderMaterial CustomShader {
        get {
            return _godotHandle.AsGodotObject().Call("get_customShader").As<ShaderMaterial>();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_customShader", value);
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

    public bool CreateCollisionInThread {
        get {
            return _godotHandle.AsGodotObject().Call("get_createCollisionInThread").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_createCollisionInThread", value);
        }
    }

    public int CollisionLayers {
        get {
            return _godotHandle.AsGodotObject().Call("get_collisionLayers").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_collisionLayers", value);
        }
    }

    public int CollisionMask {
        get {
            return _godotHandle.AsGodotObject().Call("get_collisionMask").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_collisionMask", value);
        }
    }

    public TextureSetsResource TextureSets {
        get {
            return _godotHandle.AsGodotObject().Call("get_textureSets");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_textureSets", value);
        }
    }

    public int TextureDetail {
        get {
            return _godotHandle.AsGodotObject().Call("get_textureDetail").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_textureDetail", value);
        }
    }

    public bool UseAntiTile {
        get {
            return _godotHandle.AsGodotObject().Call("get_useAntiTile").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_useAntiTile", value);
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

    public float HeightBlendFactor {
        get {
            return (float) _godotHandle.AsGodotObject().Call("get_heightBlendFactor").AsDouble();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_heightBlendFactor", value);
        }
    }

    public AlphaChannelUsage AlbedoAlphaChannelUsage {
        get {
            return (AlphaChannelUsage) _godotHandle.AsGodotObject().Call("get_albedoAlphaChannelUsage").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_albedoAlphaChannelUsage", (int) value);
        }
    }

    public AlphaChannelUsage NormalAlphaChannelUsage {
        get {
            return (AlphaChannelUsage) _godotHandle.AsGodotObject().Call("get_normalAlphaChannelUsage").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_normalAlphaChannelUsage", (int) value);
        }
    }

    public bool UseSharpTransitions {
        get {
            return _godotHandle.AsGodotObject().Call("get_useSharpTransitions").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_useSharpTransitions", value);
        }
    }

    public FoliageResource[] Foliages {
        get {
            return _godotHandle.AsGodotObject().Call("get_foliages").AsGodotArray<Variant>()?.Select(x => new FoliageResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_foliages", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public int DefaultObjectFrequency {
        get {
            return _godotHandle.AsGodotObject().Call("get_defaultObjectFrequency").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_defaultObjectFrequency", value);
        }
    }

    public ObjectLoadingStrategy ObjectLoadingStrategy {
        get {
            return (ObjectLoadingStrategy) _godotHandle.AsGodotObject().Call("get_objectLoadingStrategy").AsInt32();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objectLoadingStrategy", (int) value);
        }
    }

    public ObjectResource[] Objects {
        get {
            return _godotHandle.AsGodotObject().Call("get_objects").AsGodotArray<Variant>()?.Select(x => new ObjectResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_objects", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public WaterResource WaterDefinition {
        get {
            return _godotHandle.AsGodotObject().Call("get_waterDefinition");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_waterDefinition", value);
        }
    }

    public SnowResource SnowDefinition {
        get {
            return _godotHandle.AsGodotObject().Call("get_snowDefinition");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_snowDefinition", value);
        }
    }

    public bool ShowMetaInfo {
        get {
            return _godotHandle.AsGodotObject().Call("get_showMetaInfo").AsBool();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_showMetaInfo", value);
        }
    }

    public MetaInfoLayerResource[] MetaInfoLayers {
        get {
            return _godotHandle.AsGodotObject().Call("get_metaInfoLayers").AsGodotArray<Variant>()?.Select(x => new MetaInfoLayerResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_metaInfoLayers", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public ZonesResource TerrainZones {
        get {
            return _godotHandle.AsGodotObject().Call("get_terrainZones");
        }
        set {
            _godotHandle.AsGodotObject().Call("set_terrainZones", value);
        }
    }

    public static implicit operator TerraBrush(Variant handle) => new(handle);

    public TerraBrush(Variant handle) {
        _godotHandle = handle;
    }

    public TerraBrush() {
        _godotHandle = ClassDB.Instantiate("TerraBrush");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();

    public TerrainPositionInformation GetPositionInformation(float x, float y) {
        return _godotHandle.AsGodotObject().Call("getPositionInformation", x, y);
    }

    public void AddInteractionPoint(float x, float y) {
        _godotHandle.AsGodotObject().Call("addInteractionPoint", x, y);
    }

    public void OnUpdateTerrainSettings() {
        _godotHandle.AsGodotObject().Call("onUpdateTerrainSettings");
    }

    public StaticBody3D GetTerrainCollider() {
        return _godotHandle.AsGodotObject().Call("getTerrainCollider").As<StaticBody3D>();
    }

    public float GetHeightAtPosition(float x, float z, bool useGlobalPosition) {
        return (float) _godotHandle.AsGodotObject().Call("getHeightAtPosition", x, z, useGlobalPosition).AsDouble();
    }

    public Vector3 GetHeightForMousePosition(Camera3D camera) {
        return _godotHandle.AsGodotObject().Call("getHeightForMousePosition", camera).AsVector3();
    }

    public Vector3 GetHeightForScreenPosition(Camera3D camera, Vector2 screenPosition) {
        return _godotHandle.AsGodotObject().Call("getHeightForScreenPosition", camera, screenPosition).AsVector3();
    }

    public void HideObject(int objectLayerIndex, long objectId) {
        _godotHandle.AsGodotObject().Call("hideObject", objectLayerIndex, objectId);
    }

    public void ShowObject(int objectLayerIndex, long objectId) {
        _godotHandle.AsGodotObject().Call("showObject", objectLayerIndex, objectId);
    }
}
