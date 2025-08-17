#include "terrain_control_dock.h"

#include "tool_preview.h"
#include "dock_preview_button.h"
#include "../terra_brush.h"
#include "../misc/custom_content_loader.h"

#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/tab_container.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/h_flow_container.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_slider.hpp>

using namespace godot;

void TerrainControlDock::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onBrushSizeValueChange", "value"), &TerrainControlDock::onBrushSizeValueChange);
    ClassDB::bind_method(D_METHOD("onBrushStrengthValueChange", "value"), &TerrainControlDock::onBrushStrengthValueChange);
    ClassDB::bind_method(D_METHOD("selectToolType", "toolType"), &TerrainControlDock::selectToolType);
    ClassDB::bind_method(D_METHOD("setSelectedBrushIndex", "index"), &TerrainControlDock::setSelectedBrushIndex);
    ClassDB::bind_method(D_METHOD("setSelectedTextureIndex", "index"), &TerrainControlDock::setSelectedTextureIndex);
    ClassDB::bind_method(D_METHOD("setSelectedFoliageIndex", "index"), &TerrainControlDock::setSelectedFoliageIndex);
    ClassDB::bind_method(D_METHOD("setSelectedObjectIndex", "index"), &TerrainControlDock::setSelectedObjectIndex);
    ClassDB::bind_method(D_METHOD("setSelectedMetaInfoIndex", "index"), &TerrainControlDock::setSelectedMetaInfoIndex);
}

TerrainControlDock::TerrainControlDock() {
    _selectedBrushIndex = 0;
    _selectedTool = TerraBrush::TerrainToolType::TerrainAdd;
    _temporaryTool = TerraBrush::TerrainToolType::None;
    _selectedTextureIndex = -1;
    _selectedFoliageIndex = -1;
    _selectedObjectIndex = -1;
    _selectedMetaInfoIndex = -1;
}

TerrainControlDock::~TerrainControlDock() {}

void TerrainControlDock::_ready() {
    buildLayout();

    // TODO : GDExtension
    // if (_terraBrush != nullptr) {
    //     _brushSizeSlider.Value = TerraBrush.BrushSize;
    //     _brushStrengthSlider.Value = TerraBrush.BrushStrength;
    //     _selectedTool = TerraBrush.TerrainTool;
    //     _selectedBrushIndex = TerraBrush.SelectedBrushIndex.GetValueOrDefault();
    //     _selectedTextureIndex = TerraBrush.TextureSetIndex;
    //     _selectedFoliageIndex = TerraBrush.FoliageIndex;
    //     _selectedObjectIndex = TerraBrush.ObjectIndex;
    //     _selectedMetaInfoIndex = TerraBrush.MetaInfoIndex;

        initializeBrushes();
        initializeToolPreview();
    //     initializeTextures();
    //     initializeFoliages();
    //     initializeObjects();
    //     initializeMetaInfoLayers();
    // }

    _brushSizeSlider->connect("value_changed", Callable(this, "onBrushSizeValueChange"));
    _brushStrengthSlider->connect("value_changed", Callable(this, "onBrushStrengthValueChange"));
}

void TerrainControlDock::set_terraBrush(TerraBrush *value) {
    _terraBrush = value;
}

void TerrainControlDock::set_brushDecal(BrushDecal *value) {
    _brushDecal = value;
}

void TerrainControlDock::set_editorResourcePreview(EditorResourcePreview *value) {
    _editorResourcePreview = value;
}

void TerrainControlDock::initializeBrushes() {
    CustomContentLoader::addBrushesPreviewToParent(_brushesContainer, Callable(this, "setSelectedBrushIndex"));

    updateSelectedBrush();
}

void TerrainControlDock::initializeToolPreview() {
    for (int i = 0; i < _toolTypesContainer->get_child_count(); i++) {
        Node *childNode = _toolTypesContainer->get_child(i);
        ToolPreview *toolPreview = Object::cast_to<ToolPreview>(childNode);
        toolPreview->set_onSelect(Callable(this, "selectToolType").bind(toolPreview->get_toolType()));
    }

    updateSelectedTerrainTool();
}

void TerrainControlDock::initializeTextures() {
    CustomContentLoader::addTexturesPreviewToParent(_terraBrush, _texturesContainer, Callable(this, "setSelectedTextureIndex"));

    if (_texturesContainer->get_child_count() > 0 && _selectedTextureIndex < 0) {
        _selectedTextureIndex = 0;
    }
    upateSelectedTextureSet();
}

void TerrainControlDock::initializeFoliages() {
    CustomContentLoader::addFoliagesPreviewToParent(_terraBrush, _foliagesContainer, Callable(this, "setSelectedFoliageIndex"));

    // TODO : GDExtension
    // if (_terraBrush->get_foliages().size() > 0 && _selectedFoliageIndex < 0) {
    //     _selectedFoliageIndex = 0;
    // }

    updateSelectedFoliage();
}

void TerrainControlDock::initializeObjects() {
    CustomContentLoader::addObjectsPreviewToParent(_terraBrush, _objectsContainer, Callable(this, "setSelectedObjectIndex"));

    // TODO : GDExtension
    // if (_terraBrush->get_objects().size() > 0 && _selectedObjectIndex < 0) {
    //     _selectedObjectIndex = 0;
    // }

    updateSelectedObject();
}

void TerrainControlDock::initializeMetaInfoLayers() {
    CustomContentLoader::addMetaInfoLayersPreviewToParent(_terraBrush, _metaInfoLayersContainer, Callable(this, "setSelectedMetaInfoIndex"));

    // TODO : GDExtension
    // if (_terraBrush->get_metaInfoLayers().size() > 0 && _selectedMetaInfoIndex < 0) {
    //     _selectedMetaInfoIndex = 0;
    // }

    updateSelectedMetaInfo();
}

void TerrainControlDock::updateSelectedBrush() {
    for (int i = 0; i < _brushesContainer->get_child_count(); i++) {
        Node *childNode = _brushesContainer->get_child(i);
        DockPreviewButton *brushPreview = Object::cast_to<DockPreviewButton>(childNode);

        brushPreview->set_pressed(i == _selectedBrushIndex);
    }

    Node *selectedNode = _brushesContainer->get_child(_selectedBrushIndex);
    DockPreviewButton *selectedDockButton = Object::cast_to<DockPreviewButton>(selectedNode);

    Ref<Image> brushImage = selectedDockButton->get_buttonImage()->get_image();
    // TODO : GDExtension
    // _terraBrush?.SetCurrentBrush(_selectedBrushIndex, brushImage);
    // _brushDecal?.SetBrushImage(brushImage);
}

void TerrainControlDock::updateSelectedTerrainTool() {
    for (int i = 0; i < _toolTypesContainer->get_child_count(); i++) {
        Node *childNode = _toolTypesContainer->get_child(i);
        ToolPreview *toolPreview = Object::cast_to<ToolPreview>(childNode);
        toolPreview->set_pressed(toolPreview->get_toolType() == (_temporaryTool == TerraBrush::TerrainToolType::None ? _selectedTool : _temporaryTool));
    }
}

void TerrainControlDock::upateSelectedTextureSet() {
    for (int i = 0; i < _texturesContainer->get_child_count(); i++) {
        Node *childNode = _texturesContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedTextureIndex);
    }

    // TODO : GDExtension
    // _terraBrush->set_textureSet(_selectedTextureIndex);
}

void TerrainControlDock::updateSelectedFoliage() {
    for (int i = 0; i < _foliagesContainer->get_child_count(); i++) {
        Node *childNode = _foliagesContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedFoliageIndex);
    }

    // TODO : GDExtension
    // _terraBrush->set_foliage(_selectedFoliageIndex);
}

void TerrainControlDock::updateSelectedObject() {
    for (int i = 0; i < _objectsContainer->get_child_count(); i++) {
        Node *childNode = _objectsContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedFoliageIndex);
    }

    // TODO : GDExtension
    // _terraBrush->set_object(_selectedObjectIndex);
}

void TerrainControlDock::updateSelectedMetaInfo() {
    for (int i = 0; i < _metaInfoLayersContainer->get_child_count(); i++) {
        Node *childNode = _metaInfoLayersContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedMetaInfoIndex);
    }

    // TODO : GDExtension
    // _terraBrush->set_metaInfo(_selectedMetaInfoIndex);
}

void TerrainControlDock::onBrushSizeValueChange(const float value) {
    setBrushSize((int)value);
}

void TerrainControlDock::onBrushStrengthValueChange(const float value) {
    setBrushStrength((float)value);
}

void TerrainControlDock::setBrushSize(int value) {
    _brushSizeSlider->set_value(value);
    // TODO : GDExtension
    // _terraBrush->setBrushSize(value);
    // _brushDecal->setSize(value);
}

void TerrainControlDock::setBrushStrength(float value) {
    _brushStrengthSlider->set_value(value);
    // TODO : GDExtension
    // _terraBrush->setBrushStrength(value);
}

void TerrainControlDock::setSelectedBrushIndex(const int index) {
    _selectedBrushIndex = index;

    updateSelectedBrush();
}

void TerrainControlDock::selectToolType(const TerraBrush::TerrainToolType toolType) {
    _selectedTool = toolType;
    updateSelectedTerrainTool();
    // TODO : GDExtension
    // _terraBrush->set_terrainTool(_selectedTool);
}

void TerrainControlDock::setSelectedTextureIndex(const int index) {
    _selectedTextureIndex = index;

    upateSelectedTextureSet();
}

void TerrainControlDock::setSelectedFoliageIndex(const int index) {
    _selectedFoliageIndex = index;

    updateSelectedFoliage();
}

void TerrainControlDock::setSelectedObjectIndex(const int index) {
    _selectedObjectIndex = index;

    updateSelectedObject();
}

void TerrainControlDock::setSelectedMetaInfoIndex(const int index) {
    _selectedMetaInfoIndex = index;

    updateSelectedMetaInfo();
}

void TerrainControlDock::setShiftPressed(bool pressed) {
    if (pressed) {
        if (_selectedTool == TerraBrush::TerrainToolType::TerrainAdd || _selectedTool == TerraBrush::TerrainToolType::TerrainRemove || _selectedTool == TerraBrush::TerrainToolType::TerrainFlatten) {
            _temporaryTool = TerraBrush::TerrainToolType::TerrainSmooth;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::FoliageAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::FoliageRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::ObjectAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::ObjectRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::WaterAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::WaterRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::WaterFlowAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::WaterFlowRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::SnowAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::SnowRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::HoleAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::HoleRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::LockAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::LockRemove;
        }
        else if (_selectedTool == TerraBrush::TerrainToolType::MetaInfoAdd) {
            _temporaryTool = TerraBrush::TerrainToolType::MetaInfoRemove;
        }
        else {
            _temporaryTool = TerraBrush::TerrainToolType::None;
        }
    }
    else {
        _temporaryTool = TerraBrush::TerrainToolType::None;
    }

    // TODO : GDExtension
    // _terraBrush->set_terrainTool(_temporaryTool == TerraBrush::TerrainToolType::None ? _selectedTool : _temporaryTool);
    updateSelectedTerrainTool();
}

// This component has some complexe ui so I create a kinda of structure using {} for better visualization
void TerrainControlDock::buildLayout() {
    set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);

    MarginContainer *marginContainer = memnew(MarginContainer);
    marginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
    marginContainer->set((StringName)"theme_override_constants/margin_left", 5);
    marginContainer->set((StringName)"theme_override_constants/margin_top", 5);
    marginContainer->set((StringName)"theme_override_constants/margin_right", 5);
    marginContainer->set((StringName)"theme_override_constants/margin_bottom", 5);
    add_child(marginContainer);
    { // MarginContainer
        VBoxContainer *vBoxContainer = memnew(VBoxContainer);
        marginContainer->add_child(vBoxContainer);
        { // VBoxContainer
            TabContainer *tabContainer1 = memnew(TabContainer);
            tabContainer1->set_current_tab(0);
            tabContainer1->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
            vBoxContainer->add_child(tabContainer1);
            { // TabContainer
                Control *toolsTab = memnew(Control);
                toolsTab->set_name("Tools");
                tabContainer1->add_child(toolsTab);
                { //Tab
                    MarginContainer *toolsMarginContainer = memnew(MarginContainer);
                    toolsMarginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
                    toolsMarginContainer->set((StringName)"theme_override_constants/margin_left", 10);
                    toolsMarginContainer->set((StringName)"theme_override_constants/margin_top", 10);
                    toolsMarginContainer->set((StringName)"theme_override_constants/margin_right", 10);
                    toolsMarginContainer->set((StringName)"theme_override_constants/margin_bottom", 10);
                    toolsTab->add_child(toolsMarginContainer);
                    { // MarginContainer
                        ScrollContainer *toolsScrollContainer = memnew(ScrollContainer);
                        toolsMarginContainer->add_child(toolsScrollContainer);
                        { // ScrollContainer
                            VBoxContainer *toolsVBoxContainer = memnew(VBoxContainer);
                            toolsVBoxContainer->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
                            toolsVBoxContainer->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
                            toolsScrollContainer->add_child(toolsVBoxContainer);
                            { // VBoxContainer
                                _toolTypesContainer = memnew(HFlowContainer);
                                toolsVBoxContainer->add_child(_toolTypesContainer);
                                { // HFlowContainer
                                    ToolPreview *toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_add.png"));
                                    toolPreview->set_tooltip_text("Raise terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_remove.png"));
                                    toolPreview->set_tooltip_text("Lower terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainSmooth);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_smooth.png"));
                                    toolPreview->set_tooltip_text("Smooth terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainFlatten);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_flatten.png"));
                                    toolPreview->set_tooltip_text("Flatten terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainSetHeight);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_height.png"));
                                    toolPreview->set_tooltip_text("Set terrain height");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::TerrainSetAngle);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_angle.png"));
                                    toolPreview->set_tooltip_text("Set terrain angle");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::Paint);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/paint.png"));
                                    toolPreview->set_tooltip_text("Paint texture");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::FoliageAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_add.png"));
                                    toolPreview->set_tooltip_text("Add foliage");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::FoliageRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_remove.png"));
                                    toolPreview->set_tooltip_text("Remove foliage");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::ObjectAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_add.png"));
                                    toolPreview->set_tooltip_text("Add objects");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::ObjectRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_remove.png"));
                                    toolPreview->set_tooltip_text("Remove objects");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::WaterAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_add.png"));
                                    toolPreview->set_tooltip_text("Add water");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::WaterRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_remove.png"));
                                    toolPreview->set_tooltip_text("Remove water");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::WaterFlowAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_add.png"));
                                    toolPreview->set_tooltip_text("Add water flow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::WaterFlowRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_remove.png"));
                                    toolPreview->set_tooltip_text("Remove water flow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::SnowAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_add.png"));
                                    toolPreview->set_tooltip_text("Add snow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::SnowRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_remove.png"));
                                    toolPreview->set_tooltip_text("Remove snow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::HoleAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_add.png"));
                                    toolPreview->set_tooltip_text("Add hole");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::HoleRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_remove.png"));
                                    toolPreview->set_tooltip_text("Remove hole");
                                    _toolTypesContainer->add_child(toolPreview);


                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::LockAdd);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_add.png"));
                                    toolPreview->set_tooltip_text("Lock");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerraBrush::TerrainToolType::LockRemove);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_remove.png"));
                                    toolPreview->set_tooltip_text("Unlock");
                                    _toolTypesContainer->add_child(toolPreview);
                                }

                                VBoxContainer *toolsBrushSizeVBoxContainer = memnew(VBoxContainer);
                                toolsVBoxContainer->add_child(toolsBrushSizeVBoxContainer);
                                { // VBoxContainer
                                    Label *label = memnew(Label);
                                    label->set_text("Brush size");
                                    toolsBrushSizeVBoxContainer->add_child(label);

                                    _brushSizeSlider = memnew(HSlider);
                                    _brushSizeSlider->set_min(1.0);
                                    _brushSizeSlider->set_max(200.0);
                                    _brushSizeSlider->set_value(1.0);
                                    toolsBrushSizeVBoxContainer->add_child(_brushSizeSlider);
                                }

                                VBoxContainer *toolsBrushStrengthVBoxContainer = memnew(VBoxContainer);
                                toolsVBoxContainer->add_child(toolsBrushStrengthVBoxContainer);
                                { // VBoxContainer
                                    Label *label = memnew(Label);
                                    label->set_text("Brush strength");
                                    toolsBrushStrengthVBoxContainer->add_child(label);

                                    _brushStrengthSlider = memnew(HSlider);
                                    _brushStrengthSlider->set_min(0.01);
                                    _brushStrengthSlider->set_max(1.0);
                                    _brushStrengthSlider->set_value(0.01);
                                    _brushStrengthSlider->set_step(0.01);
                                    toolsBrushStrengthVBoxContainer->add_child(_brushStrengthSlider);
                                }

                                VBoxContainer *toolsBrushesVBoxContainer = memnew(VBoxContainer);
                                toolsVBoxContainer->add_child(toolsBrushesVBoxContainer);
                                { // VBoxContainer
                                    Label *label = memnew(Label);
                                    label->set_text("Brushes");
                                    toolsBrushesVBoxContainer->add_child(label);

                                    _brushesContainer = memnew(HFlowContainer);
                                    toolsBrushesVBoxContainer->add_child(_brushesContainer);
                                }
                            }
                        }
                    }
                }
            }

            TabContainer *tabContainer2 = memnew(TabContainer);
            tabContainer2->set_current_tab(0);
            tabContainer2->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
            vBoxContainer->add_child(tabContainer2);
            { // TabContainer
                Control *texturesTab = memnew(Control);
                texturesTab->set_name("Textures");
                tabContainer2->add_child(texturesTab);
                { // Tab
                    MarginContainer *texturesMarginContainer = memnew(MarginContainer);
                    texturesMarginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
                    texturesTab->add_child(texturesMarginContainer);
                    { // MarginContainer
                        _texturesContainer = memnew(HFlowContainer);
                        texturesMarginContainer->add_child(_texturesContainer);
                    }
                }

                Control *foliagesTab = memnew(Control);
                foliagesTab->set_name("Foliages");
                tabContainer2->add_child(foliagesTab);
                { // Tab
                    MarginContainer *foliagesMarginContainer = memnew(MarginContainer);
                    foliagesMarginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
                    foliagesTab->add_child(foliagesMarginContainer);
                    { // MarginContainer
                        _foliagesContainer = memnew(HFlowContainer);
                        foliagesMarginContainer->add_child(_foliagesContainer);
                    }
                }

                Control *objectsTab = memnew(Control);
                objectsTab->set_name("Objects");
                tabContainer2->add_child(objectsTab);
                { // Tab
                    MarginContainer *objectsMarginContainer = memnew(MarginContainer);
                    objectsMarginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
                    objectsTab->add_child(objectsMarginContainer);
                    { // MarginContainer
                        _objectsContainer = memnew(HFlowContainer);
                        objectsMarginContainer->add_child(_objectsContainer);
                    }
                }

                Control *metaInfoTab = memnew(Control);
                metaInfoTab->set_name("MetaInfo");
                tabContainer2->add_child(metaInfoTab);
                { // Tab
                    MarginContainer *metaInfoMarginContainer = memnew(MarginContainer);
                    metaInfoMarginContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
                    metaInfoTab->add_child(metaInfoMarginContainer);
                    { // MarginContainer
                        _metaInfoLayersContainer = memnew(HFlowContainer);
                        metaInfoMarginContainer->add_child(_metaInfoLayersContainer);
                    }
                }
            }
        }
    }
}