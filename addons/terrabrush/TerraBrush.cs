using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public enum TerrainToolType {
    None = 0,
    [ToolType(typeof(SculptTool))]
    TerrainAdd = 1,
    [ToolType(typeof(SculptTool))]
    TerrainRemove = 2,
    [ToolType(typeof(SculptTool))]
    TerrainSmooth = 3,
    [ToolType(typeof(SculptTool))]
    TerrainFlattern = 4,
    [ToolType(typeof(TextureTool))]
    Paint = 5,
    [ToolType(typeof(FoliageTool))]
    FoliageAdd = 6,
    [ToolType(typeof(FoliageTool))]
    FoliagRemove = 7,
    [ToolType(typeof(ObjectTool))]
    ObjectAdd = 8,
    [ToolType(typeof(ObjectTool))]
    ObjectRemove = 9,
    [ToolType(typeof(WaterTool))]
    WaterAdd = 10,
    [ToolType(typeof(WaterTool))]
    WaterRemove = 11,
    [ToolType(typeof(WaterFlowTool))]
    WaterFlowAdd = 12,
    [ToolType(typeof(WaterFlowTool))]
    WaterFlowRemove = 13,
    [ToolType(typeof(SnowTool))]
    SnowAdd = 14,
    [ToolType(typeof(SnowTool))]
    SnowRemove = 15,
}

[Tool]
public partial class TerraBrush : Node3D {
    public const int HeightMapFactor = 1;

    private ToolBase _currentTool;
    private Terrain _terrain;
    private int _brushSize = 100;
    private Image _originalBrushImage;
    private Image _brushImage;
    private float _brushStrength = 0.1f;
    private TerrainToolType _terrainTool = TerrainToolType.TerrainAdd;
    private int? _textureSetIndex = null;
    private TextureSetResource[] _texturesSet;
    private ImageTexture[] _splatmaps = new ImageTexture[]{};
    private Node3D _foliagesNode = null;
    private int? _foliageIndex = null;
    private int? _objectIndex = null;
    private Node3D _objectsContainerNode = null;
    private Node3D _waterNodeContainer = null;
    private Water _waterNode  = null;
    private Node3D _snowNodeContainer = null;
    private Snow _snowNode  = null;
    private Vector2 _previousWaterMousePosition = Vector2.Zero;
    private Vector2 _previousWaterMouseDirection = Vector2.Zero;
    private Dictionary<ImageTexture, Image> _imageTexturesCache = new();
    private Texture2D _defaultNoise;
    private string _dataPath;

    public Terrain Terrain => _terrain;
    public Water Water => _waterNode;
    public int BrushSize => _brushSize;
    public float BrushStrength => _brushStrength;
    public Image BrushImage => _brushImage;
    public TerrainToolType TerrainTool => _terrainTool;
    public int? TextureSetIndex => _textureSetIndex;
    public int? FoliageIndex => _foliageIndex;
    public int? ObjectIndex => _objectIndex;
    public Node3D ObjectsContainerNode => _objectsContainerNode;
    public Texture2D DefaultNoise => _defaultNoise;

    public Action TerrainSettingsUpdated { get;set; }
    public bool AutoAddZones { get;set; }
    public EditorUndoRedoManager UndoRedo { get;set; }

    [ExportGroup("TerrainSettings")]
    [Export]
    public int TerrainSize { get;set; } = 256;

    // [Export]
    // public ImageTexture HeightMap { get;set; }

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

    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnCreateTerrain)}")]
    public bool CreateTerrain {
        get {
            return false;
        } set {}
    }

    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnUpdateTerrainSettings)}")]
    public bool UpdateTerrain {
        get {
            return false;
        } set {}
    }

    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnRemoveTerrain)}")]
    public bool RemoveTerrain {
        get {
            return false;
        } set {}
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

    // [Export]
    // public ImageTexture[] Splatmaps { get;set; }

    [ExportGroup("Foliage")]
    [Export]
    public FoliageResource[] Foliages { get;set; }

    [ExportGroup("Objects")]
    [Export]
    public int DefaultObjectFrequency { get;set; } = 10;

    [Export]
    public ObjectResource[] Objects { get;set; }

    [ExportGroup("Water")]
    // [Export]
    // public ImageTexture WaterTexture { get;set; }

    [Export]
    public WaterResource WaterDefinition { get;set; }

    [ExportGroup("Snow")]
    // [Export]
    // public ImageTexture SnowTexture { get;set; }

    [Export]
    public SnowResource SnowDefinition { get;set; }

    [ExportGroup("Zones")]
    [Export]
    public ZonesResource TerrainZones { get;set; }

    public async override void _Ready() {
        base._Ready();

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

        SetTerrainTool(_terrainTool);
    }

    public override string[] _GetConfigurationWarnings() {
        var warnings = new List<string>();

        if (string.IsNullOrWhiteSpace(DataPath)) {
            warnings.Add($"{nameof(DataPath)} is required");
        }

        return warnings.ToArray();
    }

    public async void OnCreateTerrain() {
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
                    HeightMapTexture = ZoneUtils.CreateHeightmapImage(TerrainSize, 0, DataPath)
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
                zone.HeightMapTexture = ZoneUtils.CreateHeightmapImage(TerrainSize, i, DataPath);
            }

            CreateSplatmaps(i, zone);
        }

        TerrainZones.UpdateZonesMap();
        TerrainZones.UpdateHeightmaps();

        await WaitForTextureReady(_defaultNoise);

        if (Engine.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            // Water needs to be created first so we have the reference to the image texture
            await CreateWater();
        }

        _terrain = (await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Terrain.tscn", CancellationToken.None)).Instantiate<Terrain>();

        _terrain.TextureSets = TextureSets;
        _terrain.VisualInstanceLayers = VisualInstanceLayers;
        _terrain.CollisionLayers = CollisionLayers;
        _terrain.CollisionMask = CollisionMask;
        _terrain.ZonesSize = TerrainSize;
        _terrain.TerrainZones = TerrainZones;
        _terrain.HeightMapFactor = HeightMapFactor;
        _terrain.TextureDetail = TextureDetail;
        _terrain.WaterFactor = WaterDefinition?.WaterFactor ?? 0;
        _terrain.LODLevels = LODLevels;
        _terrain.LODRowsPerLevel = LODRowsPerLevel;
        _terrain.LODInitialCellWidth = LODInitialCellWidth;
        _terrain.CreateCollisionInThread = CreateCollisionInThread;

        AddChild(_terrain);
        _terrain.BuildTerrain(!Engine.IsEditorHint() && (CollisionOnly || DefaultSettings.CollisionOnly));

        await CreateObjects();

        if (Engine.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            await CreateFoliages();
            await CreateSnow();
        }
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

    public void BeingEditTerrain() {
        _currentTool?.BeginPaint();
    }

    public void EditTerrain(Vector3 meshPosition) {
        var meshToImagePosition = meshPosition + new Vector3(TerrainSize / 2, 0, TerrainSize / 2);
        var imagePosition = new Vector2(meshToImagePosition.X, meshToImagePosition.Z);

        _currentTool?.Paint(_terrainTool, _brushImage, _brushSize, _brushStrength, imagePosition);
    }

    public void EndEditTerrain() {
        _currentTool?.EndPaint();
    }

    public void CreateSplatmaps(int zoneIndex, ZoneResource zone) {
        var numberOfSplatmaps = Mathf.CeilToInt((TextureSets?.TextureSets?.Length ?? 0) / 4.0f);

        if (zone.SplatmapsTexture == null || zone.SplatmapsTexture.Length < numberOfSplatmaps) {
            var newList = new List<ImageTexture>(zone.SplatmapsTexture ?? Array.Empty<ImageTexture>());

            for (var i = 0; i < numberOfSplatmaps - (zone.SplatmapsTexture?.Length ?? 0); i++) {
                newList.Add(ZoneUtils.CreateSplatmapImage(TerrainSize, zoneIndex, i, DataPath));
            }

            zone.SplatmapsTexture = newList.ToArray();
        }

        TerrainZones.UpdateSplatmapsTextures();
    }

    public void SetCurrentBrush(Image brushImage) {
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

    public void SetTerrainTool(TerrainToolType terrainToolType) {
        _terrainTool = terrainToolType;

        var terrainToolTypeAttribute = AttributeUtils.GetAttribute<ToolTypeAttribute>(terrainToolType);
        if (terrainToolTypeAttribute == null) {
            _currentTool = null;
        } else if (_currentTool == null || _currentTool.GetType() != terrainToolTypeAttribute.PaintToolType) {
            _currentTool = (ToolBase) Activator.CreateInstance(terrainToolTypeAttribute.PaintToolType, this);
        }
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
                    return ZoneUtils.CreateFoliageImage(TerrainSize, zoneIndex, foliageIndex, DataPath);
                }
            });

            zone.FoliagesTexture = newList.ToArray();
        }

        TerrainZones.UpdateFoliagesTextures();

        for (var i = 0; i < Foliages.Count(); i++) {
            var foliage = Foliages[i];

            if (foliage.Definition != null) {
                var newFoliage = prefab.Instantiate<Foliage>();
                _foliagesNode.AddChild(newFoliage);

                newFoliage.ZonesSize = TerrainSize;
                newFoliage.TerrainZones = TerrainZones;
                newFoliage.TextureSets = TextureSets;
                newFoliage.TextureDetail = TextureDetail;
                newFoliage.VisualInstanceLayers = foliage.Definition.VisualInstanceLayers;
                newFoliage.Mesh = foliage.Definition.Mesh;
                newFoliage.MeshScale = foliage.Definition.MeshScale;
                newFoliage.MaximumRenderDistance = foliage.Definition.MaximumRenderDistance;
                newFoliage.EditorMaximumRenderDistance = foliage.Definition.EditorMaximumRenderDistance;
                newFoliage.WindStrength = foliage.Definition.WindStrength;
                newFoliage.MeshMaterial = foliage.Definition.MeshMaterial;
                newFoliage.WaterFactor = WaterDefinition?.WaterFactor ?? 0;
                newFoliage.NoiseTexture = foliage.Definition.NoiseTexture != null ? await WaitForTextureReady(foliage.Definition.NoiseTexture) : _defaultNoise;

                newFoliage.UpdateFoliage();
            }
        }
    }

    public async Task CreateObjects() {
        _objectsContainerNode = GetNodeOrNull<Node3D>("Objects");

        await Task.Factory.StartNew(async () => {
            await CreateObjectsAsync();
        });
    }

    private async Task CreateObjectsAsync() {
        if (Objects == null || Objects.Length == 0) {
            return;
        }

        if (_objectsContainerNode == null) {
            _objectsContainerNode = new Node3D();
            CallDeferred("add_child", _objectsContainerNode);
        }

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Count(); zoneIndex++) {
            var zone = TerrainZones.Zones[zoneIndex];

            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            var newList = new List<ImageTexture>();
            for (var objectIndex = 0; objectIndex < Objects.Count(); objectIndex++) {
                ImageTexture imageTexture = null;
                if (zone.ObjectsTexture?.Length > objectIndex) {
                    imageTexture = zone.ObjectsTexture[objectIndex];
                } else {
                    imageTexture = ZoneUtils.CreateObjectImage(TerrainSize, zoneIndex, objectIndex, DataPath);
                }

                newList.Add(imageTexture);

                var objectItem = Objects[objectIndex];

                if (objectItem.Definition != null) {
                    var noiseTexture = objectItem.Definition?.NoiseTexture != null ? await WaitForTextureReady(objectItem.Definition.NoiseTexture) : _defaultNoise;
                    Image noiseImage = null;
                    if (noiseTexture != null) {
                        noiseImage = new Image();
                        noiseImage.CopyFrom(noiseTexture.GetImage());
                    }

                    var objectNode = new Node3D();
                    objectNode.Name = $"{zoneIndex}_{objectIndex}";
                    objectNode.Visible = !objectItem.Hide;
                    objectNode.Position = new Vector3(zone.ZonePosition.X * TerrainSize, 0, zone.ZonePosition.Y * TerrainSize);

                    _objectsContainerNode.CallDeferred("add_child", objectNode);

                    // Load all the objects from the image
                    var objectsImage = imageTexture.GetImage();

                    for (var x = 0; x < objectsImage.GetWidth(); x++) {
                        for (var y = 0; y < objectsImage.GetHeight(); y++) {
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
                                if (resultImagePosition.X >= 0 && resultImagePosition.X < TerrainSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < TerrainSize) {
                                    var heightmapPixel = heightmapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                                    var waterHeight = waterImage?.GetPixel(resultImagePosition.X, resultImagePosition.Y).R ?? 0;
                                    resultPosition -= new Vector3(TerrainSize / 2, -((heightmapPixel.R * HeightMapFactor) - (waterHeight * (WaterDefinition?.WaterFactor ?? 0))), TerrainSize / 2);

                                    var nodeName = $"{x}_{y}";

                                    var newNode = objectItem.Definition.ObjectScenes[randomItemIndex].Instantiate<Node3D>();
                                    newNode.Name = nodeName;
                                    newNode.Position = resultPosition;

                                    if (objectItem.Definition.RandomYRotation) {
                                        newNode.RotationDegrees = new Vector3(0, Utils.GetNextFloatWithSeed((x * 1000) + y, 0f, 360f), 0);
                                    }

                                    objectNode.CallDeferred("add_child", newNode);
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

    private async Task CreateWater() {
        if (WaterDefinition == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Count(); i++) {
            var zone = TerrainZones.Zones[i];

            zone.WaterTexture ??= ZoneUtils.CreateWaterImage(TerrainSize, i, DataPath);
        }

        TerrainZones.UpdateWaterTextures();

        _waterNodeContainer = GetNodeOrNull<Node3D>("Water");
        if (_waterNodeContainer == null) {
            _waterNodeContainer = new Node3D();
            AddChild(_waterNodeContainer);

            var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Water.tscn", CancellationToken.None);
            _waterNode = prefab.Instantiate<Water>();

            _waterNode.TerrainZones = TerrainZones;
            _waterNode.ZonesSize = TerrainSize;
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

            _waterNode.Wave = await WaitForTextureReady(WaterDefinition.WaterWave);
            _waterNode.NormalMap = await WaitForTextureReady(WaterDefinition.WaterNormalMap);
            _waterNode.NormalMap2 = await WaitForTextureReady(WaterDefinition.WaterNormalMap2);

            _waterNodeContainer.AddChild(_waterNode);

            _waterNode.UpdateWater();
        }
    }

    private async Task CreateSnow() {
        if (SnowDefinition == null) {
            return;
        }

        for (var i = 0; i < TerrainZones.Zones?.Length; i++) {
            var zone = TerrainZones.Zones[i];

            zone.SnowTexture ??= ZoneUtils.CreateSnowImage(TerrainSize, i, DataPath);
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
        _snowNode.ZonesSize = TerrainSize;
        _snowNode.SnowDefinition = SnowDefinition;
        _snowNode.LODLevels = LODLevels;
        _snowNode.LODRowsPerLevel = LODRowsPerLevel;
        _snowNode.LODInitialCellWidth = LODInitialCellWidth;

        if (SnowDefinition.Noise != null) {
            await WaitForTextureReady(SnowDefinition.Noise);
        }

        _snowNodeContainer.AddChild(_snowNode);

        _snowNode.UpdateSnow();
    }

    public void UpdateObjectsHeight(List<ZoneResource> zones) {
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
                        if (resultImagePosition.X >= 0 && resultImagePosition.X < TerrainSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < TerrainSize) {
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
        if (_foliagesNode == null) {
            return;
        }

        _terrain.Clipmap.UpdateEditorCameraPosition(viewportCamera);
        _waterNode?.Clipmap.UpdateEditorCameraPosition(viewportCamera);
        _snowNode?.Clipmap.UpdateEditorCameraPosition(viewportCamera);

        foreach (var foliageNode in _foliagesNode.GetChildren()) {
            ((Foliage) foliageNode).UpdateEditorCameraPosition(viewportCamera);
        }
    }

    public void SaveResources() {
        if (!string.IsNullOrWhiteSpace(DataPath)) {
            TerrainZones?.SaveResources();
        }
    }

    public void AddInteractionPoint(float x, float y) {
        _snowNode?.AddCompressedSnow(x, y);
        _waterNode?.AddRippleEffect(x, y);
    }

    public TerrainPositionInformation? GetPositionInformation(float x, float y) {
        x = x + (TerrainSize / 2);
        y = y + (TerrainSize / 2);

		var xPosition = (int) Math.Round(x);
		var yPosition = (int) Math.Round(y);

        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(xPosition, yPosition, TerrainSize);
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

    private Task<Texture2D> WaitForTextureReady(Texture2D texture) {
        var asyncEvent = new TaskCompletionSource<Texture2D>();
        if (texture is NoiseTexture2D noiseTexture && noiseTexture.GetImage() == null) {
            void afterChanged() {
                asyncEvent.SetResult(texture);
                noiseTexture.Changed -= afterChanged;
            }

            noiseTexture.Changed += afterChanged;
        } else {
            asyncEvent.SetResult(texture);
        }

        return asyncEvent.Task;
    }
}
