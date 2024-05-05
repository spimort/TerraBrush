#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class TerrainControlDock : Control {
    private int _selectedBrushIndex = 0;
    private TerrainToolType _selectedTool = TerrainToolType.TerrainAdd;
    private TerrainToolType _temporaryTool = TerrainToolType.None;
    private int? _selectedTextureIndex = null;
    private int? _selectedFoliageIndex = null;
    private int? _selectedObjectIndex = null;

    [NodePath] private FlowContainer _brushesContainer;
    [NodePath] private FlowContainer _toolTypesContainer;
    [NodePath] private FlowContainer _texturesContainer;
    [NodePath] private FlowContainer _foliagesContainer;
    [NodePath] private FlowContainer _objectsContainer;
    [NodePath] private Slider _brushSizeSlider;
    [NodePath] private Slider _brushStrengthSlider;

    public TerraBrush TerraBrush { get;set; }
    public BrushDecal BrushDecal { get;set; }
    public EditorResourcePreview EditorResourcePreview { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        if (TerraBrush != null) {
            _brushSizeSlider.Value = TerraBrush.BrushSize;
            _brushStrengthSlider.Value = TerraBrush.BrushStrength;
            _selectedTool = TerraBrush.TerrainTool;
            _selectedBrushIndex = TerraBrush.SelectedBrushIndex.GetValueOrDefault();
            _selectedTextureIndex = TerraBrush.TextureSetIndex;
            _selectedFoliageIndex = TerraBrush.FoliageIndex;
            _selectedObjectIndex = TerraBrush.ObjectIndex;

            InitializeBrushes();
            InitializeToolPreview();
            InitializeTextures();
            InitializeFoliages();
            InitializeObjects();
        }

        _brushSizeSlider.ValueChanged += value => {
            SetBrushSize((int) value);
        };

        _brushStrengthSlider.ValueChanged += value => {
            SetBrushStrength((float) value);
        };
    }

    public override void _ExitTree() {
        base._ExitTree();
    }

    private void InitializeBrushes() {
        CustomContentLoader.AddBrushesPreviewToParent(_brushesContainer, index => {
            SetSelectedBrushIndex(index);
        });

        UpdateSelectedBrush();
    }

    public void SetBrushSize(int value) {
        _brushSizeSlider.Value = value;
        TerraBrush.SetBrushSize(value);
        BrushDecal.SetSize(value);
    }

    public void SetBrushStrength(float value) {
        _brushStrengthSlider.Value = value;
        TerraBrush.SetBrushStrength(value);
    }

    public void SetSelectedBrushIndex(int index) {
        _selectedBrushIndex = index;

        UpdateSelectedBrush();
    }

    private void UpdateSelectedBrush() {
        var brushesPreview = _brushesContainer.GetChildren();

        for (var i = 0; i < brushesPreview.Count; i++) {
            var brushPreview = brushesPreview[i];

            ((DockPreviewButton) brushPreview).ButtonPressed = i == _selectedBrushIndex;
        }

        var brushImage = ((DockPreviewButton) brushesPreview[_selectedBrushIndex]).ButtonImage.GetImage();
        brushImage.FlipY();
        brushImage.FlipX();
        TerraBrush?.SetCurrentBrush(_selectedBrushIndex, brushImage);
        BrushDecal?.SetBrushImage(brushImage);
    }

    private void InitializeToolPreview() {
        foreach (ToolPreview toolPreview in _toolTypesContainer.GetChildren()) {
            toolPreview.OnSelect = () => {
                SelectToolType(toolPreview.ToolType);
            };
        }

        UpdateSelectedTerrainTool();
    }

    public void SelectToolType(TerrainToolType toolType) {
        _selectedTool = toolType;
        UpdateSelectedTerrainTool();
        TerraBrush.SetTerrainTool(_selectedTool);
    }

    private void UpdateSelectedTerrainTool() {
        foreach (ToolPreview toolPreview in _toolTypesContainer.GetChildren()) {
            toolPreview.ButtonPressed = toolPreview.ToolType == (_temporaryTool == TerrainToolType.None ? _selectedTool : _temporaryTool);
        }
    }

    private void InitializeTextures() {
        CustomContentLoader.AddTexturesPreviewToParent(TerraBrush, _texturesContainer, index => {
            SetSelectedTextureIndex(index);
        });

        if (_texturesContainer.GetChildCount() > 0 && _selectedTextureIndex == null) {
            _selectedTextureIndex = 0;
        }
        UpateSelectedTextureSet();
    }

    public void SetSelectedTextureIndex(int index) {
        _selectedTextureIndex = index;

        UpateSelectedTextureSet();
    }

    private void UpateSelectedTextureSet() {
        var texturesPreview = _texturesContainer.GetChildren();

        for (var i = 0; i < texturesPreview.Count; i++) {
            var texturePreview = texturesPreview[i];

            ((DockPreviewButton) texturePreview).ButtonPressed = i == _selectedTextureIndex;
        }

        TerraBrush?.SetTextureSet(_selectedTextureIndex);
    }

    private void InitializeFoliages() {
        CustomContentLoader.AddFoliagesPreviewToParent(TerraBrush, _foliagesContainer, index => {
            SetSelectedFoliageIndex(index);
        });

        if (TerraBrush.Foliages?.Length > 0 && _selectedFoliageIndex == null) {
            _selectedFoliageIndex = 0;
        }

        UpdateSelectedFoliage();
    }

    public void SetSelectedFoliageIndex(int index) {
        _selectedFoliageIndex = index;

        UpdateSelectedFoliage();
    }

    private void UpdateSelectedFoliage() {
        var foliagesPreview = _foliagesContainer.GetChildren();

        for (var i = 0; i < foliagesPreview.Count; i++) {
            var foliagePreview = foliagesPreview[i];

            ((DockPreviewButton) foliagePreview).ButtonPressed = i == _selectedFoliageIndex;
        }

        TerraBrush?.SetFoliage(_selectedFoliageIndex);
    }

    private void InitializeObjects() {
        CustomContentLoader.AddObjectsPreviewToParent(TerraBrush, _objectsContainer, index => {
            SetSelectedObjectIndex(index);
        });

        if (TerraBrush.Objects?.Length > 0 && _selectedObjectIndex == null) {
            _selectedObjectIndex = 0;
        }

        UpdateSelectedObject();
    }

    public void SetSelectedObjectIndex(int index) {
        _selectedObjectIndex = index;

        UpdateSelectedObject();
    }

    private void UpdateSelectedObject() {
        var objectsPreview = _objectsContainer.GetChildren();

        for (var i = 0; i < objectsPreview.Count; i++) {
            var objectPreview = objectsPreview[i];

            ((DockPreviewButton) objectPreview).ButtonPressed = i == _selectedObjectIndex;
        }

        TerraBrush?.SetObject(_selectedObjectIndex);
    }

    public void SetShiftPressed(bool pressed) {
        if (pressed) {
            if (_selectedTool == TerrainToolType.TerrainAdd || _selectedTool == TerrainToolType.TerrainRemove || _selectedTool == TerrainToolType.TerrainFlatten) {
                _temporaryTool = TerrainToolType.TerrainSmooth;
            } else if (_selectedTool == TerrainToolType.FoliageAdd) {
                _temporaryTool = TerrainToolType.FoliageRemove;
            } else if (_selectedTool == TerrainToolType.ObjectAdd) {
                _temporaryTool = TerrainToolType.ObjectRemove;
            } else if (_selectedTool == TerrainToolType.WaterAdd) {
                _temporaryTool = TerrainToolType.WaterRemove;
            } else if (_selectedTool == TerrainToolType.WaterFlowAdd) {
                _temporaryTool = TerrainToolType.WaterFlowRemove;
            } else if (_selectedTool == TerrainToolType.SnowAdd) {
                _temporaryTool = TerrainToolType.SnowRemove;
            } else if (_selectedTool == TerrainToolType.HoleAdd) {
                _temporaryTool = TerrainToolType.HoleRemove;
            } else {
                _temporaryTool = TerrainToolType.None;
            }
        } else {
            _temporaryTool = TerrainToolType.None;
        }

        TerraBrush.SetTerrainTool(_temporaryTool == TerrainToolType.None ? _selectedTool : _temporaryTool);
        UpdateSelectedTerrainTool();
    }
}
#endif
