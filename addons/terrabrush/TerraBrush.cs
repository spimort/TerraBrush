using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public enum ObjectLoadingStrategy {
    ThreadedInEditorOnly = 1,
    Threaded = 2,
    NotThreaded = 3
}

[Tool]
public partial class TerraBrush : TerraBrushTool {
    public const int HeightMapFactor = 1;

	[Signal]
	public delegate void TerrainLoadedEventHandler();

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

    public Terrain Terrain => _terrain;
    public Water Water => _waterNode;
    public Snow Snow => _snowNode;
    public Node3D ObjectsContainerNode => _objectsContainerNode;
    public Texture2D DefaultNoise => _defaultNoise;

    public Action TerrainSettingsUpdated { get;set; }
    public bool AutoAddZones { get;set; }

    [ExportGroup("TerrainSettings")]
    [Export]
    public override int ZonesSize {
        get {
            return _zonesSize;
        } set {
            if (_terrain == null) {
                if (Resolution != 1 && !Utils.IsPowerOfTwo(value - 1)) {
                    OS.Alert("When the resolution is not 1, it must be a (power of 2) + 1 (ex. 257).");
                    return;
                }

                _zonesSize = value;

                UpdateConfigurationWarnings();
            } else if (value != _zonesSize) {
                OS.Alert("The ZonesSize property cannot change once the terrain has been created. Make sure you remove the terrain before changing the ZonesSize.");
            }
        }
    }

    [Export]
    public override int Resolution {
        get {
            return _resolution;
        } set {
            if (_terrain == null) {
                if (value < 1) {
                    OS.Alert("The minimum value for the resolution is 1.");
                    return;
                }

                if (value > 1 && !Utils.IsPowerOfTwo(value)){
                    OS.Alert("When the resolution is not 1, it must be a power of 2.");
                    return;
                }

                _resolution = value;

                UpdateConfigurationWarnings();
            } else if (value != _resolution) {
                OS.Alert("The Resolution property cannot change once the terrain has been created. Make sure you remove the terrain before changing the Resolution.");
            }
        }
    }

    [Export]
    public bool CollisionOnly { get;set; }

    [Export(PropertyHint.Dir)]
    public override string DataPath {
        get {
            return _dataPath;
        } set{
            _dataPath = value;

            UpdateConfigurationWarnings();
        }
    }

    [Export(PropertyHint.Layers3DRender)]
    public int VisualInstanceLayers { get;set; } = 1;

    [Export] public ShaderMaterial CustomShader {
        get {
            return _customShader;
        } set {
            _customShader = value;

            if (value != null && value.Shader == null) {
                var defaultShader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader.gdshader");
                var defaultCode = defaultShader.Code;

                var shader = new Shader {
                    Code = defaultCode
                };
                value.Shader = shader;
            }
        }
    }

    [ExportGroup("LOD")]
    [Export]
    public int LODLevels { get;set; } = 5;

    [Export]
    public int LODRowsPerLevel { get;set; } = 50;

    [Export]
    public float LODInitialCellWidth { get;set; } = 1;

    [ExportGroup("Collisions")]
    [Export]
    public bool CreateCollisionInThread { get;set; } = true;

    [Export(PropertyHint.Layers3DPhysics)]
    public int CollisionLayers { get;set; } = 1;

    [Export(PropertyHint.Layers3DPhysics)]
    public int CollisionMask { get;set; } = 1;

    [ExportGroup("Textures")]
    [Export]
    public override TextureSetsResource TextureSets { get;set; }

    [Export]
    public int TextureDetail { get;set; } = 20;

    [Export]
    public bool UseAntiTile { get;set; } = true;

    [Export]
    public bool NearestTextureFilter { get;set; } = false;

    [Export]
    public float HeightBlendFactor { get;set; } = 10f;

    [Export]
    public AlphaChannelUsage AlbedoAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;

    [Export]
    public AlphaChannelUsage NormalAlphaChannelUsage { get;set; } = AlphaChannelUsage.None;

    [ExportGroup("Foliage")]
    [Export]
    public override FoliageResource[] Foliages { get;set; }

    [ExportGroup("Objects")]
    [Export]
    public int DefaultObjectFrequency { get;set; } = 10;

    [Export]
    public ObjectLoadingStrategy ObjectLoadingStrategy { get;set; } = ObjectLoadingStrategy.ThreadedInEditorOnly;

    [Export]
    public override ObjectResource[] Objects { get;set; }

    [ExportGroup("Water")]

    [Export]
    public override WaterResource WaterDefinition { get;set; }

    [ExportGroup("Snow")]

    [Export]
    public override SnowResource SnowDefinition { get;set; }

    [ExportGroup("Zones")]
    [Export]
    public override ZonesResource TerrainZones { get;set; }

    public async override void _Ready() {
        base._Ready();

#if TOOLS
        if (Engine.IsEditorHint()) {
            CompatibilityScript_0_4_Alpha.Convert(this);
        }
#endif

        _defaultNoise = ResourceLoader.Load<Texture2D>("res://addons/terrabrush/Resources/DefaultNoise.tres");

        if (string.IsNullOrEmpty(DataPath)) {
            var scenePath = GetTree().EditedSceneRoot.SceneFilePath;
            if (!string.IsNullOrWhiteSpace(scenePath)) {
                DataPath = scenePath.Replace(System.IO.Path.GetFileName(scenePath), GetTree().EditedSceneRoot.Name);
            }
        }

        if (TerrainZones != null) {
            await LoadTerrain();
        }
    }

    public override string[] _GetConfigurationWarnings() {
        var warnings = new List<string>();

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

        return warnings.ToArray();
    }

    public override async void OnCreateTerrain() {
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

    public override void OnRemoveTerrain() {
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

        if (Engine.IsEditorHint()) {
            TerrainZones.UpdateLockTexture(ZonesSize);
        }
        TerrainZones.UpdateZonesMap();
        TerrainZones.UpdateHeightmaps();

        await Utils.WaitForTextureReady(_defaultNoise);

        if (Engine.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
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

        if (Engine.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            await CreateFoliages();
            await CreateSnow();
        }

        EmitSignal(StringNames.TerrainLoaded);
    }

    public override async void OnUpdateTerrainSettings() {
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

        _foliagesNode = GetNodeOrNull<Node3D>("Foliages");
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

        _objectsContainerNode = GetNodeOrNull<Node3D>("Objects");
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

        var loadInThread = ObjectLoadingStrategy == ObjectLoadingStrategy.Threaded || (ObjectLoadingStrategy == ObjectLoadingStrategy.ThreadedInEditorOnly && Engine.IsEditorHint());
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
            ((Node3D) objectNode).Name = $"{objectIndex}";

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

        _waterNodeContainer = GetNodeOrNull<Node3D>("Water");
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

        _snowNodeContainer = GetNodeOrNull<Node3D>("Snow");
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
            var objectItem = Objects[i];
            if (!objectItem.Hide) {
                var objectsNode = _objectsContainerNode.GetNode<IObjectsNode>($"{i}");
                objectsNode.UpdateObjectsHeight(zones);
            }
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

    public override void OnLockTerrain() {
        if (TerrainZones?.Zones != null) {
            foreach (var zone in TerrainZones.Zones) {
                zone.LockTexture = ZoneUtils.CreateLockImage(ZonesSize, zone.ZonePosition, true);
            }

            TerrainZones.UpdateLockTexture(ZonesSize);
        }
    }

    public override void OnUnlockTerrain() {
        if (TerrainZones?.Zones != null) {
            foreach (var zone in TerrainZones.Zones) {
                zone.LockTexture = null;
            }

            TerrainZones.UpdateLockTexture(ZonesSize);
        }
    }
}
