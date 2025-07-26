using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class TerrainControlDock : Control {
    private int _selectedBrushIndex = 0;
    private TerrainToolType _selectedTool = TerrainToolType.TerrainAdd;
    private TerrainToolType _temporaryTool = TerrainToolType.None;
    private int? _selectedTextureIndex = null;
    private int? _selectedFoliageIndex = null;
    private int? _selectedObjectIndex = null;
    private int? _selectedMetaInfoIndex = null;

    private FlowContainer _brushesContainer;
    private FlowContainer _toolTypesContainer;
    private FlowContainer _texturesContainer;
    private FlowContainer _foliagesContainer;
    private FlowContainer _objectsContainer;
    private FlowContainer _metaInfoLayersContainer;
    private Slider _brushSizeSlider;
    private Slider _brushStrengthSlider;

    public TerraBrush TerraBrush { get; set; }
    public BrushDecal BrushDecal { get; set; }
    public EditorResourcePreview EditorResourcePreview { get; set; }

    protected override void _Ready() {
        base._Ready();

        BuildLayout();

        if (TerraBrush != null) {
            _brushSizeSlider.Value = TerraBrush.BrushSize;
            _brushStrengthSlider.Value = TerraBrush.BrushStrength;
            _selectedTool = TerraBrush.TerrainTool;
            _selectedBrushIndex = TerraBrush.SelectedBrushIndex.GetValueOrDefault();
            _selectedTextureIndex = TerraBrush.TextureSetIndex;
            _selectedFoliageIndex = TerraBrush.FoliageIndex;
            _selectedObjectIndex = TerraBrush.ObjectIndex;
            _selectedMetaInfoIndex = TerraBrush.MetaInfoIndex;

            InitializeBrushes();
            InitializeToolPreview();
            InitializeTextures();
            InitializeFoliages();
            InitializeObjects();
            InitializeMetaInfoLayers();
        }

        _brushSizeSlider.ValueChanged += value => {
            SetBrushSize((int)value);
        };

        _brushStrengthSlider.ValueChanged += value => {
            SetBrushStrength((float)value);
        };
    }

    protected override void _ExitTree() {
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

            ((DockPreviewButton)brushPreview).ButtonPressed = i == _selectedBrushIndex;
        }

        var brushImage = ((DockPreviewButton)brushesPreview[_selectedBrushIndex]).ButtonImage.GetImage();
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

            ((DockPreviewButton)texturePreview).ButtonPressed = i == _selectedTextureIndex;
        }

        TerraBrush?.SetTextureSet(_selectedTextureIndex);
    }

    private void InitializeFoliages() {
        CustomContentLoader.AddFoliagesPreviewToParent(TerraBrush, _foliagesContainer, index => {
            SetSelectedFoliageIndex(index);
        });

        if (TerraBrush.Foliages?.Count > 0 && _selectedFoliageIndex == null) {
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

            ((DockPreviewButton)foliagePreview).ButtonPressed = i == _selectedFoliageIndex;
        }

        TerraBrush?.SetFoliage(_selectedFoliageIndex);
    }

    private void InitializeObjects() {
        CustomContentLoader.AddObjectsPreviewToParent(TerraBrush, _objectsContainer, index => {
            SetSelectedObjectIndex(index);
        });

        if (TerraBrush.Objects?.Count > 0 && _selectedObjectIndex == null) {
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

            ((DockPreviewButton)objectPreview).ButtonPressed = i == _selectedObjectIndex;
        }

        TerraBrush?.SetObject(_selectedObjectIndex);
    }

    private void InitializeMetaInfoLayers() {
        CustomContentLoader.AddMetaInfoLayersPreviewToParent(TerraBrush, _metaInfoLayersContainer, index => {
            SetSelectedMetaInfoIndex(index);
        });

        if (TerraBrush.MetaInfoLayers?.Count > 0 && _selectedMetaInfoIndex == null) {
            _selectedMetaInfoIndex = 0;
        }

        UpdateSelectedMetaInfo();
    }

    public void SetSelectedMetaInfoIndex(int index) {
        _selectedMetaInfoIndex = index;

        UpdateSelectedMetaInfo();
    }

    private void UpdateSelectedMetaInfo() {
        var metaInfoLayersPreview = _metaInfoLayersContainer.GetChildren();

        for (var i = 0; i < metaInfoLayersPreview.Count; i++) {
            var metaInfoPreview = metaInfoLayersPreview[i];

            ((DockPreviewButton)metaInfoPreview).ButtonPressed = i == _selectedMetaInfoIndex;
        }

        TerraBrush?.SetMetaInfo(_selectedMetaInfoIndex);
    }

    public void SetShiftPressed(bool pressed) {
        if (pressed) {
            if (_selectedTool == TerrainToolType.TerrainAdd || _selectedTool == TerrainToolType.TerrainRemove || _selectedTool == TerrainToolType.TerrainFlatten) {
                _temporaryTool = TerrainToolType.TerrainSmooth;
            }
            else if (_selectedTool == TerrainToolType.FoliageAdd) {
                _temporaryTool = TerrainToolType.FoliageRemove;
            }
            else if (_selectedTool == TerrainToolType.ObjectAdd) {
                _temporaryTool = TerrainToolType.ObjectRemove;
            }
            else if (_selectedTool == TerrainToolType.WaterAdd) {
                _temporaryTool = TerrainToolType.WaterRemove;
            }
            else if (_selectedTool == TerrainToolType.WaterFlowAdd) {
                _temporaryTool = TerrainToolType.WaterFlowRemove;
            }
            else if (_selectedTool == TerrainToolType.SnowAdd) {
                _temporaryTool = TerrainToolType.SnowRemove;
            }
            else if (_selectedTool == TerrainToolType.HoleAdd) {
                _temporaryTool = TerrainToolType.HoleRemove;
            }
            else if (_selectedTool == TerrainToolType.LockAdd) {
                _temporaryTool = TerrainToolType.LockRemove;
            }
            else if (_selectedTool == TerrainToolType.MetaInfoAdd) {
                _temporaryTool = TerrainToolType.MetaInfoRemove;
            }
            else {
                _temporaryTool = TerrainToolType.None;
            }
        }
        else {
            _temporaryTool = TerrainToolType.None;
        }

        TerraBrush.SetTerrainTool(_temporaryTool == TerrainToolType.None ? _selectedTool : _temporaryTool);
        UpdateSelectedTerrainTool();
    }

    // This component has some complexe ui some I create a kinda of structure using {} for better visualization
    private void BuildLayout() {
        var marginContainer = new MarginContainer();
        marginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
        marginContainer.Set((StringName)"theme_override_constants/margin_left", 5);
        marginContainer.Set((StringName)"theme_override_constants/margin_top", 5);
        marginContainer.Set((StringName)"theme_override_constants/margin_right", 5);
        marginContainer.Set((StringName)"theme_override_constants/margin_bottom", 5);
        AddChild(marginContainer);
        { // MarginContainer
            var vBoxContainer = new VBoxContainer();
            marginContainer.AddChild(vBoxContainer);
            { // VBoxContainer
                var tabContainer1 = new TabContainer {
                    CurrentTab = 0,
                    SizeFlagsVertical = SizeFlags.ExpandFill
                };
                vBoxContainer.AddChild(tabContainer1);
                { // TabContainer
                    var toolsTab = new Control {
                        Name = (StringName)"Tools"
                    };
                    tabContainer1.AddChild(toolsTab);
                    { //Tab
                        var toolsMarginContainer = new MarginContainer();
                        toolsMarginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
                        toolsMarginContainer.Set((StringName)"theme_override_constants/margin_left", 10);
                        toolsMarginContainer.Set((StringName)"theme_override_constants/margin_top", 10);
                        toolsMarginContainer.Set((StringName)"theme_override_constants/margin_right", 10);
                        toolsMarginContainer.Set((StringName)"theme_override_constants/margin_bottom", 10);
                        toolsTab.AddChild(toolsMarginContainer);
                        { // MarginContainer
                            var toolsScrollContainer = new ScrollContainer();
                            toolsMarginContainer.AddChild(toolsScrollContainer);
                            { // ScrollContainer
                                var toolsVBoxContainer = new VBoxContainer() {
                                    SizeFlagsHorizontal = SizeFlags.ExpandFill,
                                    SizeFlagsVertical = SizeFlags.ExpandFill
                                };
                                toolsScrollContainer.AddChild(toolsVBoxContainer);
                                { // VBoxContainer
                                    _toolTypesContainer = new HFlowContainer();
                                    toolsVBoxContainer.AddChild(_toolTypesContainer);
                                    { // HFlowContainer
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_add.png"),
                                            TooltipText = "Raise terrain"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_remove.png"),
                                            TooltipText = "Lower terrain"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainSmooth,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_smooth.png"),
                                            TooltipText = "Smooth terrain"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainFlatten,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_flatten.png"),
                                            TooltipText = "Flatten terrain"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainSetHeight,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_set_height.png"),
                                            TooltipText = "Set terrain height"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.TerrainSetAngle,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_set_angle.png"),
                                            TooltipText = "Set terrain angle"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.Paint,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/paint.png"),
                                            IconType = IconType.Circle,
                                            TooltipText = "Paint texture"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.FoliageAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/foliage_add.png"),
                                            TooltipText = "Add foliage"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.FoliageRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/foliage_remove.png"),
                                            TooltipText = "Remove foliage"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.ObjectAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/object_add.png"),
                                            TooltipText = "Add objects"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.ObjectRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/object_remove.png"),
                                            TooltipText = "Remove objects"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.WaterAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/water_add.png"),
                                            TooltipText = "Add water"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.WaterRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/water_remove.png"),
                                            TooltipText = "Remove water"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.WaterFlowAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/flow_add.png"),
                                            TooltipText = "Add water flow"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.WaterFlowRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/flow_remove.png"),
                                            TooltipText = "Remove water flow"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.SnowAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/snow_add.png"),
                                            TooltipText = "Add snow"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.SnowRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/snow_remove.png"),
                                            TooltipText = "Remove snow"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.HoleAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/holes_add.png"),
                                            TooltipText = "Add hole"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.HoleRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/holes_remove.png"),
                                            TooltipText = "Remove hole"
                                        });

                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.LockAdd,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/lock_add.png"),
                                            TooltipText = "Lock"
                                        });
                                        _toolTypesContainer.AddChild(new ToolPreview() {
                                            ToolType = TerrainToolType.LockRemove,
                                            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/lock_remove.png"),
                                            TooltipText = "Unlock"
                                        });
                                    }

                                    var toolsBrushSizeVBoxContainer = new VBoxContainer();
                                    toolsVBoxContainer.AddChild(toolsBrushSizeVBoxContainer);
                                    { // VBoxContainer
                                        toolsBrushSizeVBoxContainer.AddChild(new Label() {
                                            Text = "Brush size"
                                        });

                                        _brushSizeSlider = new HSlider() {
                                            MinValue = 1.0,
                                            MaxValue = 200.0,
                                            Value = 1.0
                                        };
                                        toolsBrushSizeVBoxContainer.AddChild(_brushSizeSlider);
                                    }

                                    var toolsBrushStrengthVBoxContainer = new VBoxContainer();
                                    toolsVBoxContainer.AddChild(toolsBrushStrengthVBoxContainer);
                                    { // VBoxContainer
                                        toolsBrushStrengthVBoxContainer.AddChild(new Label() {
                                            Text = "Brush strength"
                                        });

                                        _brushStrengthSlider = new HSlider() {
                                            MinValue = 0.01,
                                            MaxValue = 1.0,
                                            Value = 0.01,
                                            Step = 0.01
                                        };
                                        toolsBrushStrengthVBoxContainer.AddChild(_brushStrengthSlider);
                                    }

                                    var toolsBrushesVBoxContainer = new VBoxContainer();
                                    toolsVBoxContainer.AddChild(toolsBrushesVBoxContainer);
                                    { // VBoxContainer
                                        toolsBrushesVBoxContainer.AddChild(new Label() {
                                            Text = "Brushes"
                                        });

                                        _brushesContainer = new HFlowContainer();
                                        toolsBrushesVBoxContainer.AddChild(_brushesContainer);
                                    }
                                }
                            }
                        }
                    }
                }

                var tabContainer2 = new TabContainer() {
                    CurrentTab = 0
                };
                vBoxContainer.AddChild(tabContainer2);
                { // TabContainer
                    var texturesTab = new Control() {
                        Name = (StringName)"Textures"
                    };
                    tabContainer2.AddChild(texturesTab);
                    { // Tab
                        var texturesMarginContainer = new MarginContainer();
                        texturesMarginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
                        texturesTab.AddChild(texturesMarginContainer);
                        { // MarginContainer
                            _texturesContainer = new HFlowContainer();
                            texturesMarginContainer.AddChild(_texturesContainer);
                        }
                    }

                    var foliagesTab = new Control() {
                        Name = (StringName)"Foliages"
                    };
                    tabContainer2.AddChild(foliagesTab);
                    { // Tab
                        var foliagesMarginContainer = new MarginContainer();
                        foliagesMarginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
                        foliagesTab.AddChild(foliagesMarginContainer);
                        { // MarginContainer
                            _foliagesContainer = new HFlowContainer();
                            foliagesMarginContainer.AddChild(_foliagesContainer);
                        }
                    }

                    var objectsTab = new Control() {
                        Name = (StringName)"Objects"
                    };
                    tabContainer2.AddChild(objectsTab);
                    { // Tab
                        var objectsMarginContainer = new MarginContainer();
                        objectsMarginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
                        objectsTab.AddChild(objectsMarginContainer);
                        { // MarginContainer
                            _objectsContainer = new HFlowContainer();
                            objectsMarginContainer.AddChild(_objectsContainer);
                        }
                    }

                    var metaInfoTab = new Control() {
                        Name = (StringName)"MetaInfo"
                    };
                    tabContainer2.AddChild(metaInfoTab);
                    { // Tab
                        var metaInfoMarginContainer = new MarginContainer();
                        metaInfoMarginContainer.SetAnchorsPreset(LayoutPreset.FullRect);
                        metaInfoTab.AddChild(metaInfoMarginContainer);
                        { // MarginContainer
                            _metaInfoLayersContainer = new HFlowContainer();
                            metaInfoMarginContainer.AddChild(_metaInfoLayersContainer);
                        }
                    }
                }
            }
        }
    }
}