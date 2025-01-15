using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;
using Godot.Bridge;
using Godot.Collections;

namespace TerraBrush;

public enum ObjectLoadingStrategy {
    ThreadedInEditorOnly = 1,
    Threaded = 2,
    NotThreaded = 3
}

public enum TerrainToolType {
    None = 0,
    [ToolType(typeof(SculptTool))] TerrainAdd = 1,
    [ToolType(typeof(SculptTool))] TerrainRemove = 2,
    [ToolType(typeof(SculptTool))] TerrainSmooth = 3,
    [ToolType(typeof(SculptTool))] TerrainFlatten = 4,
    [ToolType(typeof(SetHeightTool))] TerrainSetHeight = 5,
    [ToolType(typeof(SetAngleTool))] TerrainSetAngle = 19,
    [ToolType(typeof(TextureTool))] Paint = 6,
    [ToolType(typeof(FoliageTool))] FoliageAdd = 7,
    [ToolType(typeof(FoliageTool))] FoliageRemove = 8,
    [ToolType(typeof(ObjectTool))] ObjectAdd = 9,
    [ToolType(typeof(ObjectTool))] ObjectRemove = 10,
    [ToolType(typeof(WaterTool))] WaterAdd = 11,
    [ToolType(typeof(WaterTool))] WaterRemove = 12,
    [ToolType(typeof(WaterFlowTool))] WaterFlowAdd = 13,
    [ToolType(typeof(WaterFlowTool))] WaterFlowRemove = 14,
    [ToolType(typeof(SnowTool))] SnowAdd = 15,
    [ToolType(typeof(SnowTool))] SnowRemove = 16,
    [ToolType(typeof(HoleTool))] HoleAdd = 17,
    [ToolType(typeof(HoleTool))] HoleRemove = 18,
    [ToolType(typeof(LockTool))] LockAdd = 20,
    [ToolType(typeof(LockTool))] LockRemove = 21,
}

[GodotClass(Tool=true)]
public partial class TerraBrush : Node3D {
    public const int HeightMapFactor = 1;

	[Signal]
	public delegate void TerrainLoadedEventHandler();

#region Tools
    private int _brushSize = 100;
    private Image _originalBrushImage;
    private Image _brushImage;
    private int? _selectedBrushIndex = null;
    private float _brushStrength = 0.1f;
    private float _selectedSetHeight = 0;
    private float _selectedSetAngle = 0;
    private Vector3? _selectedSetAngleInitialPoint = null;
    private int? _textureSetIndex = null;
    private int? _foliageIndex = null;
    private int? _objectIndex = null;
    private ToolBase _currentTool;
    private TerrainToolType _terrainTool = TerrainToolType.TerrainAdd;
#endregion

    private int _zonesSize = 256;
    private int _resolution = 1;
    private ShaderMaterial _customShader;
    private Terrain _terrain;
    private TextureSetResource[] _texturesSet;
    private ImageTexture[] _splatmaps = new ImageTexture[]{};
    private Node3D _foliagesNode = null;
    private Node3D _objectsContainerNode = null;
    private Node3D _waterNodeContainer = null;
    private Water _waterNode  = null;
    private Node3D _snowNodeContainer = null;
    private Snow _snowNode  = null;
    private Dictionary<ImageTexture, Image> _imageTexturesCache = new();
    private Texture2D _defaultNoise;
    private string _dataPath;

#region Tools
    public TerrainToolType TerrainTool => _terrainTool;
    public ToolBase CurrentTool => _currentTool;
    public EditorUndoRedoManager UndoRedo { get;set; }

    public int BrushSize => _brushSize;
    public float BrushStrength => _brushStrength;
    public Image BrushImage => _brushImage;
    public int? SelectedBrushIndex => _selectedBrushIndex;
    public float SelectedSetHeight => _selectedSetHeight;
    public float SelectedSetAngle => _selectedSetAngle;
    public Vector3? SelectedSetAngleInitialPoint => _selectedSetAngleInitialPoint;
    public int? TextureSetIndex => _textureSetIndex;
    public int? FoliageIndex => _foliageIndex;
    public int? ObjectIndex => _objectIndex;

    public bool CreateTerrain {
        get {
            return false;
        } set {}
    }

    public bool UpdateTerrain {
        get {
            return false;
        } set {}
    }

    public bool RemoveTerrain {
        get {
            return false;
        } set {}
    }

    public bool LockAllTerrain {
        get {
            return false;
        } set {}
    }

    public bool UnlockAllTerrain {
        get {
            return false;
        } set {}
    }

    public bool ImportTerrain {
        get {
            return false;
        } set {}
    }

    public bool ExportTerrain {
        get {
            return false;
        } set {}
    }
#endregion

    public Terrain Terrain => _terrain;
    public Water Water => _waterNode;
    public Snow Snow => _snowNode;
    public Node3D ObjectsContainerNode => _objectsContainerNode;
    public Texture2D DefaultNoise => _defaultNoise;

    public Action TerrainSettingsUpdated { get;set; }
    public bool AutoAddZones { get;set; }

    [BindProperty]
    public int ZonesSize {
        get {
            return _zonesSize;
        } set {
            if (_terrain == null) {
                if (Resolution != 1 && !Utils.IsPowerOfTwo(value - 1)) {
                    OS.Singleton.Alert("When the resolution is not 1, it must be a (power of 2) + 1 (ex. 257).");
                    return;
                }

                _zonesSize = value;

                UpdateConfigurationWarnings();
            } else if (value != _zonesSize) {
                OS.Singleton.Alert("The ZonesSize property cannot change once the terrain has been created. Make sure you remove the terrain before changing the ZonesSize.");
            }
        }
    }

    [BindProperty]
    public int Resolution {
        get {
            return _resolution;
        } set {
            if (_terrain == null) {
                if (value < 1) {
                    OS.Singleton.Alert("The minimum value for the resolution is 1.");
                    return;
                }

                if (value > 1 && !Utils.IsPowerOfTwo(value)){
                    OS.Singleton.Alert("When the resolution is not 1, it must be a power of 2.");
                    return;
                }

                _resolution = value;

                UpdateConfigurationWarnings();
            } else if (value != _resolution) {
                OS.Singleton.Alert("The Resolution property cannot change once the terrain has been created. Make sure you remove the terrain before changing the Resolution.");
            }
        }
    }

    [BindProperty]
    public bool CollisionOnly { get;set; }

    [BindProperty]
    public string DataPath {
        get {
            return _dataPath;
        } set{
            _dataPath = value;

            UpdateConfigurationWarnings();
        }
    }
    [BindProperty]
    public int VisualInstanceLayers { get;set; } = 1;

    [BindProperty]
    public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = (Shader) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }

    [BindProperty]
    public int LODLevels { get;set; } = 5;
    [BindProperty]
    public int LODRowsPerLevel { get;set; } = 101;
    [BindProperty]
    public float LODInitialCellWidth { get;set; } = 1;

    [BindProperty]
    public bool CreateCollisionInThread { get;set; } = true;
    [BindProperty]
    public int CollisionLayers { get;set; } = 1;
    [BindProperty]
    public int CollisionMask { get;set; } = 1;

    // [BindProperty]
    public TextureSetsResource TextureSets { get;set; }
    [BindProperty]
    public int TextureDetail { get;set; } = 20;
    [BindProperty]
    public bool UseAntiTile { get;set; } = true;
    [BindProperty]
    public bool NearestTextureFilter { get;set; } = false;
    [BindProperty]
    public float HeightBlendFactor { get;set; } = 10f;
    [BindProperty]
    public AlphaChannelUsage AlbedoAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;
    [BindProperty]
    public AlphaChannelUsage NormalAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;

    // [BindProperty]
    public FoliageResource[] Foliages { get;set; }

    [BindProperty]
    public int DefaultObjectFrequency { get;set; } = 10;
    [BindProperty]
    public ObjectLoadingStrategy ObjectLoadingStrategy { get;set; } = ObjectLoadingStrategy.ThreadedInEditorOnly;
    // [BindProperty]
    public ObjectResource[] Objects { get;set; }

    // [BindProperty]
    public WaterResource WaterDefinition { get;set; }

    // [BindProperty]
    public SnowResource SnowDefinition { get;set; }

    // [BindProperty]
    public ZonesResource TerrainZones { get;set; }

    protected async override void _Ready() {
        base._Ready();

        SetTerrainTool(_terrainTool);

        if (Engine.Singleton.IsEditorHint()) {
            CompatibilityScript_0_4_Alpha.Convert(this);
        }

        _defaultNoise = (Texture2D) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/DefaultNoise.tres");

        if (string.IsNullOrEmpty(DataPath)) {
            var scenePath = GetTree().EditedSceneRoot.SceneFilePath;
            if (!string.IsNullOrWhiteSpace(scenePath)) {
                DataPath = scenePath.Replace(System.IO.Path.GetFileName(scenePath), (string)GetTree().EditedSceneRoot.Name);
            }
        }

        if (TerrainZones != null) {
            await LoadTerrain();
        }
    }

    protected override PackedStringArray _GetConfigurationWarnings() {
        var warnings = new PackedStringArray();

        if (string.IsNullOrWhiteSpace(DataPath)) {
            warnings.Add($"{nameof(DataPath)} is required");
        }

        if (Resolution != 1) {
            if (!Utils.IsPowerOfTwo(Resolution)) {
                warnings.Add($"{nameof(Resolution)} must be a power of 2");
            }

            if (!Utils.IsPowerOfTwo(ZonesSize - 1)) {
                warnings.Add($"{nameof(ZonesSize)} must be a (power of 2) + 1");
            }

            if (LODInitialCellWidth != Resolution) {
                warnings.Add($"{nameof(LODInitialCellWidth)} should be equals to {nameof(Resolution)} for better result");
            }
        }

        return warnings;
    }

    public async void OnCreateTerrain() {
        if (Resolution != 1) {
            if (!Utils.IsPowerOfTwo(Resolution)) {
                return;
            }

            if (!Utils.IsPowerOfTwo(ZonesSize - 1)) {
                return;
            }
        }

        if (string.IsNullOrWhiteSpace(DataPath)) {
            return;
        }

        if (!string.IsNullOrWhiteSpace(DataPath) && !DirAccess.DirExistsAbsolute(DataPath)) {
            DirAccess.MakeDirAbsolute(DataPath);
        }

        OnRemoveTerrain();

        TerrainZones = new ZonesResource() {
            Zones = new ZoneResource[] {
                new ZoneResource() {
                    HeightMapTexture = ZoneUtils.CreateHeightmapImage(ZonesSize, Resolution, new Vector2I(0, 0), DataPath)
                }
            }
        };

        await LoadTerrain();

        TerrainSettingsUpdated?.Invoke();
    }

    public void OnRemoveTerrain() {
        if (_terrain != null) {
            _terrain.QueueFree();
            _terrain = null;
        }

        if (_foliagesNode != null) {
            _foliagesNode.QueueFree();
            _foliagesNode = null;
        }

        ClearObjects();

        if (_waterNodeContainer != null) {
            _waterNodeContainer.QueueFree();
            _waterNodeContainer = null;

            _waterNode = null;
        }

        if (_snowNodeContainer != null) {
            _snowNodeContainer.QueueFree();
            _snowNodeContainer = null;

            _snowNode = null;
        }

        TerrainZones = null;
    }

    private async Task LoadTerrain() {
        if (TerrainZones == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Count(); i++) {
            var zone = TerrainZones.Zones[i];

            if (zone.HeightMapTexture == null) {
                zone.HeightMapTexture = ZoneUtils.CreateHeightmapImage(ZonesSize, Resolution, zone.ZonePosition, DataPath);
            }

            CreateSplatmaps(zone);
        }
        TerrainZones.UpdateSplatmapsTextures();

        if (Engine.Singleton.IsEditorHint()) {
            TerrainZones.UpdateLockTexture(ZonesSize);
        }
        TerrainZones.UpdateZonesMap();
        TerrainZones.UpdateHeightmaps();

        await Utils.WaitForTextureReady(_defaultNoise);

        if (Engine.Singleton.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            // Water needs to be created first so we have the reference to the image texture
            await CreateWater();
        }

        _terrain = (await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Terrain.tscn", CancellationToken.None)).Instantiate<Terrain>();

        _terrain.TextureSets = TextureSets;
        _terrain.VisualInstanceLayers = VisualInstanceLayers;
        _terrain.CustomShader = CustomShader;
        _terrain.CollisionLayers = CollisionLayers;
        _terrain.CollisionMask = CollisionMask;
        _terrain.ZonesSize = ZonesSize;
        _terrain.Resolution = Resolution;
        _terrain.TerrainZones = TerrainZones;
        _terrain.HeightMapFactor = HeightMapFactor;
        _terrain.TextureDetail = TextureDetail;
        _terrain.UseAntiTile = UseAntiTile;
        _terrain.NearestTextureFilter = NearestTextureFilter;
        _terrain.HeightBlendFactor = HeightBlendFactor;
        _terrain.AlbedoAlphaChannelUsage = AlbedoAlphaChannelUsage;
        _terrain.NormalAlphaChannelUsage = NormalAlphaChannelUsage;
        _terrain.WaterFactor = WaterDefinition?.WaterFactor ?? 0;
        _terrain.LODLevels = LODLevels;
        _terrain.LODRowsPerLevel = LODRowsPerLevel;
        _terrain.LODInitialCellWidth = LODInitialCellWidth;
        _terrain.CollisionOnly = CollisionOnly;
        _terrain.CreateCollisionInThread = CreateCollisionInThread;

        AddChild(_terrain);

        await CreateObjects();

        if (Engine.Singleton.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            await CreateFoliages();
            await CreateSnow();
        }

        EmitSignal(StringNames.TerrainLoaded);
    }

    public async void OnUpdateTerrainSettings() {
        if (_terrain != null) {
            _terrain.QueueFree();
            _terrain = null;
        }

        if (_foliagesNode != null) {
            _foliagesNode.QueueFree();
            _foliagesNode = null;
        }

        ClearObjects();

        if (_waterNodeContainer != null) {
            _waterNodeContainer.QueueFree();
            _waterNodeContainer = null;

            _waterNode = null;
        }

        if (_snowNodeContainer != null) {
            _snowNodeContainer.QueueFree();
            _snowNodeContainer = null;

            _snowNode = null;
        }

        await LoadTerrain();
        TerrainSettingsUpdated?.Invoke();
    }

    public void ClearObjects() {
        if (_objectsContainerNode != null) {
            _objectsContainerNode.QueueFree();
            _objectsContainerNode = null;
        }
    }

    public void CreateSplatmaps(ZoneResource zone) {
        var numberOfSplatmaps = Mathf.CeilToInt((TextureSets?.TextureSets?.Length ?? 0) / 4.0f);

        if (zone.SplatmapsTexture == null || zone.SplatmapsTexture.Length < numberOfSplatmaps) {
            var newList = new List<ImageTexture>(zone.SplatmapsTexture ?? Array.Empty<ImageTexture>());

			for (var i = zone.SplatmapsTexture?.Length ?? 0; i < numberOfSplatmaps; i++) {
                newList.Add(ZoneUtils.CreateSplatmapImage(ZonesSize, zone.ZonePosition, i, DataPath));
            }

            zone.SplatmapsTexture = newList.ToArray();
        }
    }

    private async Task CreateFoliages() {
        if (Foliages == null || Foliages.Length == 0) {
            return;
        }

        var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Foliage.tscn", CancellationToken.None);

        _foliagesNode = GetNodeOrNull<Node3D>((NodePath)"Foliages");
        if (_foliagesNode == null) {
            _foliagesNode = new Node3D();
            AddChild(_foliagesNode);
        }

        foreach (var existingFoliage in _foliagesNode.GetChildren()) {
            existingFoliage.QueueFree();
        }

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Count(); zoneIndex++) {
            var zone = TerrainZones.Zones[zoneIndex];
            var newList = Foliages.Select((foliage, foliageIndex) => {
                if (zone.FoliagesTexture?.Length > foliageIndex) {
                    return zone.FoliagesTexture[foliageIndex];
                } else {
                    return ZoneUtils.CreateFoliageImage(ZonesSize, zone.ZonePosition, foliageIndex, DataPath);
                }
            });

            zone.FoliagesTexture = newList.ToArray();
        }

        TerrainZones.InitializeFoliageTextures(this);
        TerrainZones.UpdateFoliagesTextures();

        for (var i = 0; i < Foliages.Count(); i++) {
            var foliage = Foliages[i];

            if (foliage.Definition != null) {
                var newFoliage = prefab.Instantiate<Foliage>();

                newFoliage.FoliageIndex = i;
                newFoliage.ZonesSize = ZonesSize;
                newFoliage.Resolution = Resolution;
                newFoliage.TerrainZones = TerrainZones;
                newFoliage.TextureSets = TextureSets;
                newFoliage.TextureDetail = TextureDetail;
                newFoliage.WaterFactor = WaterDefinition?.WaterFactor ?? 0;
                newFoliage.Definition = foliage.Definition;

                _foliagesNode.AddChild(newFoliage);
            }
        }
    }

    public async Task CreateObjects() {
        if (Objects == null || Objects.Length == 0) {
            return;
        }

        _objectsContainerNode = GetNodeOrNull<Node3D>((NodePath)"Objects");
        if (_objectsContainerNode == null) {
            _objectsContainerNode = new Node3D();
            AddChild(_objectsContainerNode);
        }

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Length; zoneIndex++) {
            var zone = TerrainZones.Zones[zoneIndex];
            var newList = Objects.Select((objectItem, objectIndex) => {
                if (zone.ObjectsTexture?.Length > objectIndex) {
                    return zone.ObjectsTexture[objectIndex];
                } else {
                    return  ZoneUtils.CreateObjectImage(ZonesSize, zone.ZonePosition, objectIndex, DataPath);
                }
            });

            zone.ObjectsTexture = newList.ToArray();
        }

        var loadInThread = ObjectLoadingStrategy == ObjectLoadingStrategy.Threaded || (ObjectLoadingStrategy == ObjectLoadingStrategy.ThreadedInEditorOnly && Engine.Singleton.IsEditorHint());
        for (var objectIndex = 0; objectIndex < Objects.Length; objectIndex++) {
            var objectItem = Objects[objectIndex];
            if (objectItem.Hide) {
                continue;
            }

            var prefab = objectItem.Definition.Strategy switch {
                ObjectStrategy.OctreeMultiMeshes => await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/ObjectsOctreeMultiMesh.tscn", CancellationToken.None),
                ObjectStrategy.PackedScenes => await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Objects.tscn", CancellationToken.None),
                _ => throw new NotImplementedException()
            };

            var objectNode = prefab.Instantiate<IObjectsNode>();
            ((Node3D) objectNode).Name = (StringName)$"{objectIndex}";

            objectNode.ObjectsIndex = objectIndex;
            objectNode.Definition = objectItem.Definition;
            objectNode.TerrainZones = TerrainZones;
            objectNode.ZonesSize = ZonesSize;
            objectNode.Resolution = Resolution;
            objectNode.WaterFactor = WaterDefinition?.WaterFactor ?? 0;
            objectNode.LoadInThread = loadInThread;
            objectNode.DefaultObjectFrequency = DefaultObjectFrequency;

            _objectsContainerNode.AddChild((Node3D) objectNode);
        }

        TerrainZones.UpdateObjectsTextures();
    }

    private async Task CreateWater() {
        if (WaterDefinition == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Count(); i++) {
            var zone = TerrainZones.Zones[i];

            zone.WaterTexture ??= ZoneUtils.CreateWaterImage(ZonesSize, Resolution, zone.ZonePosition, DataPath);
        }

        TerrainZones.UpdateWaterTextures();

        _waterNodeContainer = GetNodeOrNull<Node3D>((NodePath)"Water");
        if (_waterNodeContainer == null) {
            _waterNodeContainer = new Node3D();
            AddChild(_waterNodeContainer);

            var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Water.tscn", CancellationToken.None);
            _waterNode = prefab.Instantiate<Water>();

            _waterNode.TerrainZones = TerrainZones;
            _waterNode.ZonesSize = ZonesSize;
            _waterNode.Resolution = Resolution;
            _waterNode.WaterFactor = WaterDefinition.WaterFactor;
            _waterNode.WaterInnerOffset = WaterDefinition.WaterInnerOffset;
            _waterNode.HeightMapFactor = HeightMapFactor;
            _waterNode.WaterColor = WaterDefinition.WaterColor;
            _waterNode.FresnelColor = WaterDefinition.WaterFresnelColor;
            _waterNode.Metallic = WaterDefinition.WaterMetallic;
            _waterNode.Roughness = WaterDefinition.WaterRoughness;
            _waterNode.TimeScale = WaterDefinition.WaterTimeScale;
            _waterNode.Strength = WaterDefinition.WaterStrength;
            _waterNode.NoiseScale = WaterDefinition.WaterNoiseScale;
            _waterNode.HeightScale = WaterDefinition.WaterHeightScale;
            _waterNode.ColorDeep = WaterDefinition.WaterColorDeep;
            _waterNode.ColorShallow = WaterDefinition.WaterColorShallow;
            _waterNode.BeersLaw = WaterDefinition.WaterBeersLaw;
            _waterNode.DepthOffset = WaterDefinition.WaterDepthOffset;
            _waterNode.EdgeScale = WaterDefinition.WaterEdgeScale;
            _waterNode.Near = WaterDefinition.WaterNear;
            _waterNode.Far = WaterDefinition.WaterFar;
            _waterNode.EdgeColor = WaterDefinition.WaterEdgeColor;
            _waterNode.VisualInstanceLayers = WaterDefinition.VisualInstanceLayers;
            _waterNode.LODLevels = LODLevels;
            _waterNode.LODRowsPerLevel = LODRowsPerLevel;
            _waterNode.LODInitialCellWidth = LODInitialCellWidth;
            _waterNode.CustomShader = WaterDefinition.CustomShader;

            _waterNode.Wave = await Utils.WaitForTextureReady(WaterDefinition.WaterWave);
            _waterNode.NormalMap = await Utils.WaitForTextureReady(WaterDefinition.WaterNormalMap);
            _waterNode.NormalMap2 = await Utils.WaitForTextureReady(WaterDefinition.WaterNormalMap2);

            _waterNodeContainer.AddChild(_waterNode);
        }
    }

    private async Task CreateSnow() {
        if (SnowDefinition == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Length; i++) {
            var zone = TerrainZones.Zones[i];

            zone.SnowTexture ??= ZoneUtils.CreateSnowImage(ZonesSize, Resolution, zone.ZonePosition, DataPath);
        }

        _snowNodeContainer = GetNodeOrNull<Node3D>((NodePath)"Snow");
        if (_snowNodeContainer == null) {
            _snowNodeContainer = new Node3D();
            AddChild(_snowNodeContainer);
        }

        TerrainZones.UpdateSnowTextures();

        var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Snow.tscn", CancellationToken.None);
        _snowNode = prefab.Instantiate<Snow>();

        _snowNode.TerrainZones = TerrainZones;
        _snowNode.ZonesSize = ZonesSize;
        _snowNode.Resolution = Resolution;
        _snowNode.SnowDefinition = SnowDefinition;
        _snowNode.LODLevels = LODLevels;
        _snowNode.LODRowsPerLevel = LODRowsPerLevel;
        _snowNode.LODInitialCellWidth = LODInitialCellWidth;

        if (SnowDefinition.Noise != null) {
            await Utils.WaitForTextureReady(SnowDefinition.Noise);
        }

        _snowNodeContainer.AddChild(_snowNode);
    }

    public void UpdateObjectsHeight(List<ZoneResource> zones) {
        for (var i = 0; i < Objects?.Length; i++) {
            var objectsNode = _objectsContainerNode.GetNode<IObjectsNode>((NodePath)$"{i}");
            objectsNode.UpdateObjectsHeight(zones);
        }
    }

    public void UpdateCameraPosition(Camera3D viewportCamera) {
        _terrain?.Clipmap.UpdateEditorCameraPosition(viewportCamera);
        _waterNode?.Clipmap.UpdateEditorCameraPosition(viewportCamera);
        _snowNode?.Clipmap.UpdateEditorCameraPosition(viewportCamera);

        if (_foliagesNode != null) {
            foreach (var foliageNode in _foliagesNode.GetChildren()) {
                ((Foliage) foliageNode).UpdateEditorCameraPosition(viewportCamera);
            }
        }
    }

    public void SaveResources() {
        if (!string.IsNullOrWhiteSpace(DataPath)) {
            TerrainZones?.SaveResources();
        }
    }

    public void AddInteractionPoint(float x, float y) {
        x += ZonesSize / 2;
        y += ZonesSize / 2;

        if (ZonesSize % 2 == 0) {
            x -= LODInitialCellWidth / 2.0f;
            y -= LODInitialCellWidth / 2.0f;
        }

        _snowNode?.AddCompressedSnow(x, y);
        _waterNode?.AddRippleEffect(x, y);
    }

    public TerrainPositionInformation GetPositionInformation(float x, float y) {
        x += ZonesSize / 2;
        y += ZonesSize / 2;

        if (ZonesSize % 2 == 0) {
            x -= LODInitialCellWidth / 2.0f;
            y -= LODInitialCellWidth / 2.0f;
        }

        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(x, y, ZonesSize, Resolution);
        var zone = TerrainZones.GetZoneForZoneInfo(zoneInfo);

        if (zone != null) {
            float? waterFactor = null;
            float? snowFactor = null;

            if (zone.WaterTexture != null) {
                waterFactor = GetImageFromImageTexture(zone.WaterTexture).GetPixel(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y).R;
            }

            if (zone.SnowTexture != null) {
                snowFactor = GetImageFromImageTexture(zone.SnowTexture).GetPixel(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y).R;
            }

            return new TerrainPositionInformation() {
                Textures = zone.SplatmapsTexture?.Length > 0 ? TextureSets?.TextureSets?.Select((textureSet, index) => {
                    var splatmapIndex = Mathf.FloorToInt(index / 4);
                    var splatmapImage = zone.SplatmapsTexture[splatmapIndex];
                    var pixel = GetImageFromImageTexture(splatmapImage).GetPixel(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
                    var colorIndex = index % 4;

                    return new TerrainPositionTextureInformation() {
                        Index = index,
                        Name = textureSet.Name,
                        Factor = pixel[colorIndex]
                    };
                }).OrderByDescending(item => item.Factor).ToArray() : new TerrainPositionTextureInformation[] {},
                WaterFactor = waterFactor ?? 0,
                WaterDeepness = waterFactor * WaterDefinition?.WaterFactor ?? 0,
                SnowFactor = snowFactor ?? 0,
                SnowHeight = snowFactor * SnowDefinition?.SnowFactor ?? 0
            };
        }

        return null;
    }

    private Image GetImageFromImageTexture(ImageTexture texture) {
        _imageTexturesCache.TryGetValue(texture, out var image);

        if (image == null) {
            image = texture.GetImage();
            _imageTexturesCache.Add(texture, image);
        }

        return image;
    }

    public void OnLockTerrain() {
        if (TerrainZones?.Zones != null) {
            foreach (var zone in TerrainZones.Zones) {
                zone.LockTexture = ZoneUtils.CreateLockImage(ZonesSize, zone.ZonePosition, true);
            }

            TerrainZones.UpdateLockTexture(ZonesSize);
        }
    }

    public void OnUnlockTerrain() {
        if (TerrainZones?.Zones != null) {
            foreach (var zone in TerrainZones.Zones) {
                zone.LockTexture = null;
            }

            TerrainZones.UpdateLockTexture(ZonesSize);
        }
    }

#region Tools
    public void SetTerrainTool(TerrainToolType terrainToolType) {
        _terrainTool = terrainToolType;

        var terrainToolTypeAttribute = AttributeUtils.GetAttribute<ToolTypeAttribute>(terrainToolType);
        if (terrainToolTypeAttribute == null) {
            _currentTool?.BeforeDeselect();
            _currentTool = null;
        } else if (_currentTool == null || _currentTool.GetType() != terrainToolTypeAttribute.PaintToolType) {
            _currentTool?.BeforeDeselect();
            _currentTool = (ToolBase) Activator.CreateInstance(terrainToolTypeAttribute.PaintToolType);
            _currentTool.Initialize((TerraBrush) this);
        }
    }

    public void BeingEditTerrain() {
        _currentTool?.BeginPaint();
    }

    public void EditTerrain(Vector3 meshPosition) {
        var meshToImagePosition = meshPosition + new Vector3(ZonesSize / 2, 0, ZonesSize / 2);
        var imagePosition = new Vector2(meshToImagePosition.X, meshToImagePosition.Z);

        _currentTool?.Paint(_terrainTool, _brushImage, _brushSize, _brushStrength, imagePosition);
    }

    public void EndEditTerrain() {
        _currentTool?.EndPaint();
    }

    public void SetCurrentBrush(int brushIndex, Image brushImage) {
        _selectedBrushIndex = brushIndex;
        _originalBrushImage = brushImage;

        SetBrushSize(_brushSize);
    }

    public void SetBrushSize(int value) {
        _brushImage = new Image();
        _brushImage.CopyFrom(_originalBrushImage);
        _brushImage.Resize(value, value);

        _brushSize = value;
    }

    public void SetBrushStrength(float value) {
        _brushStrength = value;
    }

    public void SetTextureSet(int? textureSetIndex) {
        _textureSetIndex = textureSetIndex;
    }

    public void SetFoliage(int? foliageIndex) {
        _foliageIndex = foliageIndex;
    }

    public void SetObject(int? objectIndex) {
        _objectIndex = objectIndex;
    }

    public void UpdateSetHeightValue(float value) {
        _selectedSetHeight = value;
    }

    public void UpdateSetAngleValue(float value, Vector3? initialPoint) {
        _selectedSetAngle = value;
        _selectedSetAngleInitialPoint = initialPoint;
    }

    public async Task OnImportTerrain() {
        var settings = await DialogUtils.ShowImportDialog(GetParent(), this);
        if (settings != null) {
            ImporterEngine.ImportTerrain(this, settings);
            OnUpdateTerrainSettings();
        }
    }

    public async Task OnExportTerrain() {
        var folder = await DialogUtils.ShowFileDialog(GetTree().Root, fileMode: EditorFileDialog.FileModeEnum.OpenDir);
        if (string.IsNullOrWhiteSpace(folder)) {
            return;
        }

        ExporterEngine.ExportTerrain(this, folder);
    }
#endregion
}
