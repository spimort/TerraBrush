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

    private int _zonesSize = 256;
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
    private CancellationTokenSource _objectsCreationCancellationTokenSource;

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
                _zonesSize = value;
            } else if (value != _zonesSize) {
                OS.Alert("The ZonesSize property cannot change once the terrain has been created. Make sure you remove the terrain before changing the ZonesSize.");
            }
        }
    }

    [Export]
    public bool CollisionOnly { get;set; }

    [Export(PropertyHint.Dir)]
    public string DataPath {
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
    public int LODRowsPerLevel { get;set; } = 101;

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
    public TextureSetsResource TextureSets { get;set;}

    [Export]
    public int TextureDetail { get;set; } = 20;

    [Export]
    public bool UseAntiTile { get;set; } = true;

    [Export]
    public bool NearestTextureFilter { get;set; } = false;

    [Export]
    public float HeightBlendFactor { get;set; } = 10f;

    [ExportGroup("Foliage")]
    [Export]
    public FoliageResource[] Foliages { get;set; }

    [ExportGroup("Objects")]
    [Export]
    public int DefaultObjectFrequency { get;set; } = 10;

    [Export]
    public ObjectLoadingStrategy ObjectLoadingStrategy { get;set; } = ObjectLoadingStrategy.ThreadedInEditorOnly;

    [Export]
    public ObjectResource[] Objects { get;set; }

    [ExportGroup("Water")]

    [Export]
    public WaterResource WaterDefinition { get;set; }

    [ExportGroup("Snow")]

    [Export]
    public SnowResource SnowDefinition { get;set; }

    [ExportGroup("Zones")]
    [Export]
    public ZonesResource TerrainZones { get;set; }

    public async override void _Ready() {
        base._Ready();

        if (Engine.IsEditorHint()) {
            CompatibilityScript_0_4_Alpha.Convert(this);
        }

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

        return warnings.ToArray();
    }

    public override async void OnCreateTerrain() {
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
                    HeightMapTexture = ZoneUtils.CreateHeightmapImage(ZonesSize, new Vector2I(0, 0), DataPath)
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
                zone.HeightMapTexture = ZoneUtils.CreateHeightmapImage(ZonesSize, zone.ZonePosition, DataPath);
            }

            CreateSplatmaps(zone);
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
        _terrain.TerrainZones = TerrainZones;
        _terrain.HeightMapFactor = HeightMapFactor;
        _terrain.TextureDetail = TextureDetail;
        _terrain.UseAntiTile = UseAntiTile;
        _terrain.NearestTextureFilter = NearestTextureFilter;
        _terrain.HeightBlendFactor = HeightBlendFactor;
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

        TerrainZones.UpdateSplatmapsTextures();
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
        _objectsContainerNode = GetNodeOrNull<Node3D>("Objects");

        var loadInThread = ObjectLoadingStrategy == ObjectLoadingStrategy.Threaded || (ObjectLoadingStrategy == ObjectLoadingStrategy.ThreadedInEditorOnly && Engine.IsEditorHint());

        if (loadInThread) {
            _objectsCreationCancellationTokenSource?.Cancel();
            _objectsCreationCancellationTokenSource = new CancellationTokenSource();

            await Task.Factory.StartNew(async () => {
                await CreateObjectsAsync(_objectsCreationCancellationTokenSource.Token);
            }, _objectsCreationCancellationTokenSource.Token);
        } else {
            await CreateObjectsAsync(CancellationToken.None);
        }
    }

    private async Task CreateObjectsAsync(CancellationToken cancellationToken) {
        if (Objects == null || Objects.Length == 0) {
            return;
        }

        if (_objectsContainerNode == null) {
            _objectsContainerNode = new Node3D();
            CallDeferred("add_child", _objectsContainerNode);
        }

        if (cancellationToken.IsCancellationRequested) {
            return;
        }

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Count(); zoneIndex++) {
            if (cancellationToken.IsCancellationRequested) {
                return;
            }

            var zone = TerrainZones.Zones[zoneIndex];

            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            var newList = new List<ImageTexture>();
            for (var objectIndex = 0; objectIndex < Objects.Count(); objectIndex++) {
                if (cancellationToken.IsCancellationRequested) {
                    return;
                }

                ImageTexture imageTexture = null;
                if (zone.ObjectsTexture?.Length > objectIndex) {
                    imageTexture = zone.ObjectsTexture[objectIndex];
                } else {
                    imageTexture = ZoneUtils.CreateObjectImage(ZonesSize, zone.ZonePosition, objectIndex, DataPath);
                }

                newList.Add(imageTexture);

                var objectItem = Objects[objectIndex];

                if (objectItem.Definition != null) {
                    var noiseTexture = objectItem.Definition?.NoiseTexture != null ? await Utils.WaitForTextureReady(objectItem.Definition.NoiseTexture) : _defaultNoise;
                    Image noiseImage = null;
                    if (noiseTexture != null) {
                        noiseImage = new Image();
                        noiseImage.CopyFrom(noiseTexture.GetImage());
                    }

                    var objectNode = new Node3D();
                    objectNode.Name = $"{zoneIndex}_{objectIndex}";
                    objectNode.Visible = !objectItem.Hide;
                    objectNode.Position = new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize);

                    _objectsContainerNode.CallDeferred("add_child", objectNode);

                    // Load all the objects from the image
                    var objectsImage = imageTexture.GetImage();

                    for (var x = 0; x < objectsImage.GetWidth(); x++) {
                        if (cancellationToken.IsCancellationRequested) {
                            return;
                        }

                        for (var y = 0; y < objectsImage.GetHeight(); y++) {
                            if (cancellationToken.IsCancellationRequested) {
                                return;
                            }

                            var randomItemIndex = Utils.GetNextIntWithSeed((x * 1000) + y, 0, objectItem.Definition.ObjectScenes.Count() - 1);

                            var objectPixel = objectsImage.GetPixel(x, y);

                            if (objectPixel.A > 0.0f) {
                                var resultPosition = new Vector3(x, 0, y);
                                if (noiseImage != null) {
                                    var noisePixel = noiseImage.GetPixel(x, y).R;
                                    var randomValueX = Utils.GetNextFloatWithSeed((int) (noisePixel * 100), -objectItem.Definition.RandomRange, objectItem.Definition.RandomRange);
                                    var randomValueZ = Utils.GetNextFloatWithSeed(1 + (int) (noisePixel * 100), -objectItem.Definition.RandomRange, objectItem.Definition.RandomRange);
                                    resultPosition += new Vector3(randomValueX, 0, randomValueZ);
                                }

                                var resultImagePosition = new Vector2I((int) Math.Round(resultPosition.X), (int) Math.Round(resultPosition.Z));
                                if (resultImagePosition.X >= 0 && resultImagePosition.X < ZonesSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < ZonesSize) {
                                    var heightmapPixel = heightmapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                                    // Check for hole
                                    if (heightmapPixel.G == 0.0) {
                                        var waterHeight = waterImage?.GetPixel(resultImagePosition.X, resultImagePosition.Y).R ?? 0;
                                        resultPosition -= new Vector3(ZonesSize / 2, -((heightmapPixel.R * HeightMapFactor) - (waterHeight * (WaterDefinition?.WaterFactor ?? 0))), ZonesSize / 2);

                                        if (cancellationToken.IsCancellationRequested) {
                                            return;
                                        }

                                        CallDeferred(
                                            nameof(AddObjectNode),
                                            objectItem.Definition.ObjectScenes[randomItemIndex],
                                            objectNode,
                                            $"{x}_{y}",
                                            resultPosition,
                                            objectItem.Definition.RandomYRotation ? new Vector3(0, Utils.GetNextFloatWithSeed((x * 1000) + y, 0f, 360f), 0) : Vector3.Zero
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }

            zone.ObjectsTexture = newList.ToArray();
        }

        TerrainZones.UpdateObjectsTextures();
    }

    private void AddObjectNode(PackedScene packedScene, Node parentNode, string nodeName, Vector3 nodePosition, Vector3 nodeRotation) {
        var newNode = packedScene.Instantiate<Node3D>();
        newNode.Name = nodeName;
        newNode.Position = nodePosition;

        newNode.RotationDegrees = nodeRotation;

        parentNode.AddChild(newNode);
    }

    private async Task CreateWater() {
        if (WaterDefinition == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Count(); i++) {
            var zone = TerrainZones.Zones[i];

            zone.WaterTexture ??= ZoneUtils.CreateWaterImage(ZonesSize, zone.ZonePosition, DataPath);
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

            zone.SnowTexture ??= ZoneUtils.CreateSnowImage(ZonesSize, zone.ZonePosition, DataPath);
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
        if (Objects == null || Objects.Length == 0) {
            return;
        }

        foreach (var zone in zones) {
            var zoneIndex = Array.IndexOf(TerrainZones.Zones, zone);
            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            for (var i = 0; i < Objects.Length; i++) {
                var objectsDefinition = Objects[i].Definition;
                var nodeName = $"{zoneIndex}_{i}";
                var objectsNode = _objectsContainerNode.GetNode(nodeName);

                if (objectsNode != null) {
                    var noiseTexture = objectsDefinition.NoiseTexture != null ? objectsDefinition.NoiseTexture : _defaultNoise;
                    Image noiseImage = null;
                    if (noiseTexture != null) {
                        noiseImage = noiseTexture.GetImage();
                    }

                    foreach (Node3D objectNode in objectsNode.GetChildren()) {
                        var objectNodeName = objectNode.Name;
                        var positions = objectNodeName.ToString().Split("_");
                        var xPosition = int.Parse(positions[0]);
                        var yPosition = int.Parse(positions[1]);

                        var resultPosition = new Vector3(xPosition, 0, yPosition);
                        if (noiseImage != null) {
                            var noisePixel = noiseImage.GetPixel(xPosition, yPosition).R;
                            var randomValueX = Utils.GetNextFloatWithSeed((int) (noisePixel * 100), -objectsDefinition.RandomRange, objectsDefinition.RandomRange);
                            var randomValueZ = Utils.GetNextFloatWithSeed(1 + (int) (noisePixel * 100), -objectsDefinition.RandomRange, objectsDefinition.RandomRange);
                            resultPosition += new Vector3(randomValueX, 0, randomValueZ);
                        }

                        var resultImagePosition = new Vector2I((int) Math.Round(resultPosition.X), (int) Math.Round(resultPosition.Z));
                        if (resultImagePosition.X >= 0 && resultImagePosition.X < ZonesSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < ZonesSize) {
                            var heightmapPixel = heightmapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                            var waterHeight = waterImage?.GetPixel(xPosition, yPosition).R ?? 0;
                            objectNode.Position = new Vector3(objectNode.Position.X, (heightmapPixel.R * HeightMapFactor) - (waterHeight * (WaterDefinition?.WaterFactor ?? 0)), objectNode.Position.Z);
                        }
                    }
                }
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

        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(x, y, ZonesSize);
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

    public override Godot.Collections.Array<Godot.Collections.Dictionary> _GetPropertyList()
    {
        return base._GetPropertyList();
    }
}
