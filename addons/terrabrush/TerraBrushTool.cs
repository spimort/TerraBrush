using System;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

#if TOOLS
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
#endif

public partial class TerraBrushTool : Node3D {
#if TOOLS
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

    [ExportGroup("Lock | Unlock")]
    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnLockTerrain)}")]
    public bool LockAllTerrain {
        get {
            return false;
        } set {}
    }

    [ExportGroup("Lock | Unlock")]
    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnUnlockTerrain)}")]
    public bool UnlockAllTerrain {
        get {
            return false;
        } set {}
    }

    [ExportGroup("Import | Export")]
    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnImportTerrain)}")]
    public bool ImportTerrain {
        get {
            return false;
        } set {}
    }

    [ExportGroup("Import | Export")]
    [Export(PropertyHint.None, $"{ButtonInspectorPlugin.ButtonInspectorHintString}_{nameof(OnExportTerrain)}")]
    public bool ExportTerrain {
        get {
            return false;
        } set {}
    }

    public override void _Ready() {
        base._Ready();

        SetTerrainTool(_terrainTool);
    }

    public void SetTerrainTool(TerrainToolType terrainToolType) {
        _terrainTool = terrainToolType;

        var terrainToolTypeAttribute = AttributeUtils.GetAttribute<ToolTypeAttribute>(terrainToolType);
        if (terrainToolTypeAttribute == null) {
            _currentTool?.BeforeDeselect();
            _currentTool = null;
        } else if (_currentTool == null || _currentTool.GetType() != terrainToolTypeAttribute.PaintToolType) {
            _currentTool?.BeforeDeselect();
            _currentTool = (ToolBase) Activator.CreateInstance(terrainToolTypeAttribute.PaintToolType, this);
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
#endif

#region  " Virtual overrides "
    public virtual int ZonesSize { get;set; }
    public virtual string DataPath { get;set; }
    public virtual ZonesResource TerrainZones { get;set; }
    public virtual TextureSetsResource TextureSets { get;set; }
    public virtual FoliageResource[] Foliages { get;set; }
    public virtual ObjectResource[] Objects { get;set; }
    public virtual WaterResource WaterDefinition { get;set; }
    public virtual SnowResource SnowDefinition { get;set; }
    public virtual void OnCreateTerrain() {}
    public virtual void OnUpdateTerrainSettings() {}
    public virtual void OnRemoveTerrain() {}
    public virtual void OnLockTerrain() {}
    public virtual void OnUnlockTerrain() {}
#endregion
}
