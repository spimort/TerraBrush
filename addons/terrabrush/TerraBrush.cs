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
    private const string HeightmapFileName = "Heightmap.res";
    private const string SplatmapFileName = "Splatmap{0}.res";
    private const string FoliageFileName = "Foliage{0}.res";
    private const string ObjectFileName = "Object{0}.res";
    private const string WaterFileName = "Water.res";
    private const string SnowFileName = "Snow.res";

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
    private Image[] _splatmapImagesCache = null;
    private Image _waterImageCache = null;
    private Image _snowImageCache = null;
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

    [ExportGroup("TerrainSettings")]
    [Export]
    public int TerrainSize { get;set; } = 256;

    [Export]
    public ImageTexture HeightMap { get;set; }

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

    [Export]
    public ImageTexture[] Splatmaps { get;set; }

    [ExportGroup("Foliage")]
    [Export]
    public FoliageResource[] Foliages { get;set; }

    [ExportGroup("Objects")]
    [Export]
    public int DefaultObjectFrequency { get;set; } = 10;

    [Export]
    public ObjectResource[] Objects { get;set; }

    [ExportGroup("Water")]
    [Export]
    public ImageTexture WaterTexture { get;set; }

    [Export]
    public WaterResource WaterDefinition { get;set; }

    [ExportGroup("Snow")]
    [Export]
    public ImageTexture SnowTexture { get;set; }

    [Export]
    public SnowResource SnowDefinition { get;set; }

    public async override void _Ready() {
        base._Ready();

        _defaultNoise = ResourceLoader.Load<Texture2D>("res://addons/terrabrush/Resources/DefaultNoise.tres");

        if (string.IsNullOrEmpty(DataPath)) {
            var scenePath = GetTree().EditedSceneRoot.SceneFilePath;
            if (!string.IsNullOrWhiteSpace(scenePath)) {
                DataPath = scenePath.Replace(System.IO.Path.GetFileName(scenePath), GetTree().EditedSceneRoot.Name);
            }
        }

        if (HeightMap != null) {
            await LoadTerrain();
        }

        if (!Engine.IsEditorHint()) {
            if (_splatmaps != null) {
                _splatmapImagesCache = Splatmaps.Select(splatmap => splatmap.GetImage()).ToArray();
            }

            if (WaterTexture != null) {
                _waterImageCache = WaterTexture.GetImage();
            }

            if (SnowTexture != null) {
                _snowImageCache = SnowTexture.GetImage();
            }
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

        var image = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rf);
        HeightMap = GetImageTextureResource(image, HeightmapFileName);

        await LoadTerrain();

        TerrainSettingsUpdated?.Invoke();
    }

    public void OnRemoveTerrain() {
        Splatmaps = new ImageTexture[]{};

        if (_terrain != null) {
            _terrain.QueueFree();
            _terrain = null;
        }

        if (_foliagesNode != null) {
            _foliagesNode.QueueFree();
            _foliagesNode = null;
        }

        ClearObjects();

        if (Foliages != null) {
            foreach (var foliage in Foliages) {
                foliage.Texture = null;
            }
        }

        if (Objects != null) {
            foreach (var objectItem in Objects) {
                objectItem.Texture = null;
            }
        }

        if (_waterNodeContainer != null) {
            _waterNodeContainer.QueueFree();
            _waterNodeContainer = null;

            _waterNode = null;
        }
        WaterTexture = null;

        if (_snowNodeContainer != null) {
            _snowNodeContainer.QueueFree();
            _snowNodeContainer = null;

            _snowNode = null;
        }
        SnowTexture = null;
    }

    private async Task LoadTerrain() {
        if (HeightMap == null) {
            return;
        }

        await WaitForTextureReady(_defaultNoise);

        if (Engine.IsEditorHint() || (!CollisionOnly && !DefaultSettings.CollisionOnly)) {
            // Water needs to be created first so we have the reference to the image texture
            await CreateWater();
        }

        _terrain = (await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Terrain.tscn", CancellationToken.None)).Instantiate<Terrain>();

        CreateSplatmaps();

        _terrain.TextureSets = TextureSets;
        _terrain.Splatmaps = Splatmaps;

        _terrain.VisualInstanceLayers = VisualInstanceLayers;
        _terrain.CollisionLayers = CollisionLayers;
        _terrain.CollisionMask = CollisionMask;
        _terrain.TerrainSize = TerrainSize;
        _terrain.TerrainSubDivision = TerrainSize;
        _terrain.HeightMap = HeightMap;
        _terrain.HeightMapFactor = HeightMapFactor;
        _terrain.TextureDetail = TextureDetail;
        _terrain.WaterTexture = WaterTexture;
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
            void afterRender() {
                UpdateFoliagesGroudTexture();
                RenderingServer.FramePostDraw -= afterRender;
            }
            RenderingServer.FramePostDraw += afterRender;
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

    public void EditTerrain(Vector3 meshPosition) {
        var meshToImagePosition = meshPosition + new Vector3(TerrainSize / 2, 0, TerrainSize / 2);
        var imagePosition = new Vector2(meshToImagePosition.X, meshToImagePosition.Z);

        _currentTool?.Paint(this, _terrainTool, _brushImage, _brushSize, _brushStrength, imagePosition);
    }

    public void CreateSplatmaps() {
        var numberOfSplatmaps = Mathf.CeilToInt((TextureSets?.TextureSets?.Length ?? 0) / 4.0f);

        if (Splatmaps.Length < numberOfSplatmaps) {
            var newList = new List<ImageTexture>(Splatmaps);

            for (var i = 0; i < numberOfSplatmaps - Splatmaps.Length; i++) {
                var splatmapImage = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);

                if (newList.Count == 0) {
                    splatmapImage.Fill(new Color(1, 0, 0, 0));
                } else {
                    splatmapImage.Fill(new Color(0, 0, 0, 0));
                }
                newList.Add(GetImageTextureResource(splatmapImage, string.Format(SplatmapFileName, i)));
            }

            Splatmaps = newList.ToArray();
            _terrain.Splatmaps = Splatmaps;
        }
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
        } else {
            _currentTool = (ToolBase) Activator.CreateInstance(terrainToolTypeAttribute.PaintToolType);
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
        var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Foliage.tscn", CancellationToken.None);

        _foliagesNode = GetNodeOrNull<Node3D>("Foliages");
        if (_foliagesNode == null) {
            _foliagesNode = new Node3D();
            AddChild(_foliagesNode);
        }

        foreach (var existingFoliage in _foliagesNode.GetChildren()) {
            existingFoliage.QueueFree();
        }

        if (Foliages == null) {
            return;
        }

        for (var i = 0; i < Foliages.Count(); i++) {
            var foliage = Foliages[i];

            if (foliage.Definition != null) {
                var newFoliage = prefab.Instantiate<Foliage>();
                _foliagesNode.AddChild(newFoliage);

                newFoliage.TerrainSize = TerrainSize;
                newFoliage.HeightMapTexture = HeightMap;
                newFoliage.HeightMapFactor = HeightMapFactor;
                newFoliage.VisualInstanceLayers = foliage.Definition.VisualInstanceLayers;
                newFoliage.Mesh = foliage.Definition.Mesh;
                newFoliage.MeshScale = foliage.Definition.MeshScale;
                newFoliage.MaximumRenderDistance = foliage.Definition.MaximumRenderDistance;
                newFoliage.EditorMaximumRenderDistance = foliage.Definition.EditorMaximumRenderDistance;
                newFoliage.WindStrength = foliage.Definition.WindStrength;
                newFoliage.MeshMaterial = foliage.Definition.MeshMaterial;
                newFoliage.WaterTexture = WaterTexture;
                newFoliage.WaterFactor = WaterDefinition?.WaterFactor ?? 0;

                newFoliage.NoiseTexture = foliage.Definition.NoiseTexture != null ? await WaitForTextureReady(foliage.Texture) : _defaultNoise;

                if (foliage.Texture == null) {
                    var image = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);
                    foliage.Texture = GetImageTextureResource(image, string.Format(FoliageFileName, i));
                }

                newFoliage.FoliageTexture = foliage.Texture;

                newFoliage.UpdateFoliage();
            }
        }
    }

    public async Task CreateObjects() {
        _objectsContainerNode = GetNodeOrNull<Node3D>("Objects");
        if (_objectsContainerNode == null) {
            _objectsContainerNode = new Node3D();
            AddChild(_objectsContainerNode);
        }

        if (Objects == null) {
            return;
        }

        var heightMapImage = HeightMap.GetImage();
        var waterImage = WaterTexture?.GetImage();

        for (var i = 0; i < Objects.Count(); i++) {
            var objectItem = Objects[i];

            if (objectItem.Definition != null) {
                var noiseTexture = objectItem.Definition?.NoiseTexture != null ? await WaitForTextureReady(objectItem.Definition.NoiseTexture) : _defaultNoise;
                Image noiseImage = null;
                if (noiseTexture != null) {
                    noiseImage = new Image();
                    noiseImage.CopyFrom(noiseTexture.GetImage());
                }

                var objectNode = new Node3D();
                objectNode.Name = i.ToString();
                objectNode.Visible = !objectItem.Hide;

                _objectsContainerNode.AddChild(objectNode);

                // Load all the objects from the image
                if (objectItem.Texture == null) {
                    var image = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);
                    objectItem.Texture = GetImageTextureResource(image, string.Format(ObjectFileName, i));
                } else {
                    var objectsImage = objectItem.Texture.GetImage();

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
                                    var heightmapPixel = heightMapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                                    var waterHeight = waterImage?.GetPixel(resultImagePosition.X, resultImagePosition.Y).R ?? 0;
                                    resultPosition -= new Vector3(TerrainSize / 2, -((heightmapPixel.R * HeightMapFactor) - (waterHeight * (WaterDefinition?.WaterFactor ?? 0))), TerrainSize / 2);

                                    var nodeName = $"{x}_{y}";

                                    var newNode = objectItem.Definition.ObjectScenes[randomItemIndex].Instantiate<Node3D>();
                                    newNode.Name = nodeName;
                                    newNode.Position = resultPosition;

                                    if (objectItem.Definition.RandomYRotation) {
                                        newNode.RotationDegrees = new Vector3(newNode.RotationDegrees.X, Utils.GetNextFloatWithSeed((x * 1000) + y, 0f, 360f), newNode.RotationDegrees.Z);
                                    }

                                    objectNode.AddChild(newNode);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    private async Task CreateWater() {
        if (WaterDefinition == null) {
            return;
        }

        if (WaterTexture == null) {
            var waterImage = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);
            waterImage.Fill(new Color(0, 0.5f, 0.5f, 1));

            WaterTexture = GetImageTextureResource(waterImage, WaterFileName);
        }

        _waterNodeContainer = GetNodeOrNull<Node3D>("Water");
        if (_waterNodeContainer == null) {
            _waterNodeContainer = new Node3D();
            AddChild(_waterNodeContainer);

            var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Water.tscn", CancellationToken.None);
            _waterNode = prefab.Instantiate<Water>();

            _waterNode.TerrainSize = TerrainSize;
            _waterNode.TerrainSubDivision = TerrainSize;
            _waterNode.WaterTexture = WaterTexture;
            _waterNode.HeightMapTexture = HeightMap;
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

        _snowNodeContainer = GetNodeOrNull<Node3D>("Snow");
        if (_snowNodeContainer == null) {
            _snowNodeContainer = new Node3D();
            AddChild(_snowNodeContainer);
        }
        if (SnowTexture == null) {
            var snowImage = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);

            SnowTexture = GetImageTextureResource(snowImage, SnowFileName);
        }

        var prefab = await AsyncUtils.LoadResourceAsync<PackedScene>("res://addons/terrabrush/Components/Snow.tscn", CancellationToken.None);
        _snowNode = prefab.Instantiate<Snow>();

        _snowNode.TerrainSize = TerrainSize;
        _snowNode.TerrainSubDivision = TerrainSize;
        _snowNode.HeightMapTexture = HeightMap;
        _snowNode.HeightMapFactor = HeightMapFactor;
        _snowNode.SnowTexture = SnowTexture;
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

    public void UpdateObjectsHeight() {
        var heightmapImage = HeightMap.GetImage();
        var waterImage = WaterTexture?.GetImage();

        for (var i = 0; i < _objectsContainerNode.GetChildCount(); i++) {
            var objectsNode = _objectsContainerNode.GetChild(i);
            var objectsDefinition = Objects[i].Definition;

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

    public async void UpdateFoliagesGroudTexture() {
        await ToSignal(GetTree(), "process_frame");
        await ToSignal(GetTree(), "process_frame");

        var resultImage = _terrain.ResultViewport.GetTexture().GetImage();
        resultImage.Resize(TerrainSize, TerrainSize);
        var resultTexture = ImageTexture.CreateFromImage(resultImage);

        foreach (var foliageNode in _foliagesNode?.GetChildren()) {
            ((Foliage) foliageNode).UpdateGroudTexture(resultTexture);
        }
    }

    private ImageTexture GetImageTextureResource(Image image, string filePath) {
        ImageTexture imageTexture;
        if (string.IsNullOrWhiteSpace(DataPath)) {
            imageTexture = ImageTexture.CreateFromImage(image);
        } else {
            var resourcePath = Utils.PathCombineForwardSlash(DataPath, filePath);
            if (Godot.FileAccess.FileExists(resourcePath)) {
                imageTexture = ResourceLoader.Load<ImageTexture>(resourcePath);
                imageTexture.SetImage(image);
            } else {
                imageTexture = ImageTexture.CreateFromImage(image);
                ResourceSaver.Save(imageTexture, resourcePath);
                imageTexture = ResourceLoader.Load<ImageTexture>(resourcePath);
            }
        }

        return imageTexture;
    }

    public void SaveResources() {
        if (!string.IsNullOrWhiteSpace(DataPath)) {
            if (HeightMap != null) {
                SaveImageResource(HeightMap);
            }

            if (WaterTexture != null) {
                SaveImageResource(WaterTexture);
            }

            if (SnowTexture != null) {
                SaveImageResource(SnowTexture);
            }

            if (Splatmaps != null) {
                foreach (var splatMap in Splatmaps) {
                    SaveImageResource(splatMap);
                }
            }

            if (Foliages != null) {
                foreach (var foliage in Foliages) {
                    if (foliage.Texture != null) {
                        SaveImageResource(foliage.Texture);
                    }
                }
            }

            if (Objects != null) {
                foreach (var objectItem in Objects) {
                    if (objectItem.Texture != null) {
                        SaveImageResource(objectItem.Texture);
                    }
                }
            }
        }
    }

    private void SaveImageResource(ImageTexture image) {
        if (!string.IsNullOrWhiteSpace(image.ResourcePath) && FileAccess.FileExists(image.ResourcePath)) {
            ResourceSaver.Save(image, image.ResourcePath);
        }
    }

    public void AddInteractionPoint(float x, float y) {
        _snowNode?.AddCompressedSnow(x, y);
        _waterNode?.AddRippleEffect(x, y);
    }

    public TerrainPositionInformation? GetPositionInformation(float x, float y) {
		var xPosition = (int) Math.Round(x);
		var yPosition = (int) Math.Round(y);

        if (x < -(TerrainSize / 2) || x > TerrainSize / 2 || y < -(TerrainSize / 2) || y > TerrainSize / 2) {
            return null;
        }

        var waterFactor = _waterImageCache?.GetPixel(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2)).R;
        var snowFactor = _snowImageCache?.GetPixel(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2)).R;

        return new TerrainPositionInformation() {
            Textures = _splatmapImagesCache?.Length > 0 ? TextureSets?.TextureSets?.Select((textureSet, index) => {
                var splatmapIndex = Mathf.FloorToInt(index / 4);
                var splatmapImage = _splatmapImagesCache[splatmapIndex];
                var pixel = splatmapImage.GetPixel(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2));
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


