#include "terrain_control_dock.h"

#include "tool_preview.h"
#include "dock_preview_button.h"
#include "../terra_brush.h"
#include "../misc/custom_content_loader.h"
#include "../misc/enums.h"

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

    ADD_SIGNAL(MethodInfo("toolTypeSelected", PropertyInfo(Variant::INT, "toolType")));
    ADD_SIGNAL(MethodInfo("brushSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("brushSizeChanged", PropertyInfo(Variant::INT, "value")));
    ADD_SIGNAL(MethodInfo("brushStrengthChanged", PropertyInfo(Variant::FLOAT, "value")));
    ADD_SIGNAL(MethodInfo("textureSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("foliageSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("objectSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("metaInfoSelected", PropertyInfo(Variant::INT, "index")));
}

TerrainControlDock::TerrainControlDock() {
    _selectedBrushIndex = 0;
    _selectedTool = TerrainToolType::TERRAINTOOLTYPE_TERRAINADD;
    _selectedTextureIndex = -1;
    _selectedFoliageIndex = -1;
    _selectedObjectIndex = -1;
    _selectedMetaInfoIndex = -1;
}

TerrainControlDock::~TerrainControlDock() {}

void TerrainControlDock::_ready() {
    buildLayout();

    if (_terraBrush != nullptr) {
        initializeBrushes();
        initializeToolPreview();
        initializeTextures();
        initializeFoliages();
        initializeObjects();
        initializeMetaInfoLayers();
    }

    _brushSizeSlider->connect("value_changed", Callable(this, "onBrushSizeValueChange"));
    _brushStrengthSlider->connect("value_changed", Callable(this, "onBrushStrengthValueChange"));
}

void TerrainControlDock::set_terraBrush(TerraBrush *value) {
    _terraBrush = value;
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
}

void TerrainControlDock::initializeFoliages() {
    CustomContentLoader::addFoliagesPreviewToParent(_terraBrush, _foliagesContainer, Callable(this, "setSelectedFoliageIndex"));
}

void TerrainControlDock::initializeObjects() {
    CustomContentLoader::addObjectsPreviewToParent(_terraBrush, _objectsContainer, Callable(this, "setSelectedObjectIndex"));
}

void TerrainControlDock::initializeMetaInfoLayers() {
    CustomContentLoader::addMetaInfoLayersPreviewToParent(_terraBrush, _metaInfoLayersContainer, Callable(this, "setSelectedMetaInfoIndex"));
}

void TerrainControlDock::updateSelectedBrush() {
    for (int i = 0; i < _brushesContainer->get_child_count(); i++) {
        Node *childNode = _brushesContainer->get_child(i);
        DockPreviewButton *brushPreview = Object::cast_to<DockPreviewButton>(childNode);

        brushPreview->set_pressed(i == _selectedBrushIndex);
    }

    Node *selectedNode = _brushesContainer->get_child(_selectedBrushIndex);
    DockPreviewButton *selectedDockButton = Object::cast_to<DockPreviewButton>(selectedNode);

    emit_signal("brushSelected", _selectedBrushIndex);
}

void TerrainControlDock::updateSelectedTerrainTool() {
    for (int i = 0; i < _toolTypesContainer->get_child_count(); i++) {
        Node *childNode = _toolTypesContainer->get_child(i);
        ToolPreview *toolPreview = Object::cast_to<ToolPreview>(childNode);
        toolPreview->set_pressed(toolPreview->get_toolType() == _selectedTool);
    }
}

void TerrainControlDock::upateSelectedTextureSet() {
    for (int i = 0; i < _texturesContainer->get_child_count(); i++) {
        Node *childNode = _texturesContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedTextureIndex);
    }

    emit_signal("textureSelected", _selectedTextureIndex);
}

void TerrainControlDock::updateSelectedFoliage() {
    for (int i = 0; i < _foliagesContainer->get_child_count(); i++) {
        Node *childNode = _foliagesContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedFoliageIndex);
    }

    emit_signal("foliageSelected", _selectedFoliageIndex);
}

void TerrainControlDock::updateSelectedObject() {
    for (int i = 0; i < _objectsContainer->get_child_count(); i++) {
        Node *childNode = _objectsContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedObjectIndex);
    }

    emit_signal("objectSelected", _selectedObjectIndex);
}

void TerrainControlDock::updateSelectedMetaInfo() {
    for (int i = 0; i < _metaInfoLayersContainer->get_child_count(); i++) {
        Node *childNode = _metaInfoLayersContainer->get_child(i);
        DockPreviewButton *previewButton = Object::cast_to<DockPreviewButton>(childNode);

        previewButton->set_pressed(i == _selectedMetaInfoIndex);
    }

    emit_signal("metaInfoSelected", _selectedMetaInfoIndex);
}

void TerrainControlDock::onBrushSizeValueChange(const float value) {
    setBrushSize((int)value);
}

void TerrainControlDock::onBrushStrengthValueChange(const float value) {
    setBrushStrength((float)value);
}

void TerrainControlDock::setBrushSize(int value) {
    _brushSizeSlider->set_value(value);
    emit_signal("brushSizeChanged", value);
}

void TerrainControlDock::setBrushStrength(float value) {
    _brushStrengthSlider->set_value(value);
    emit_signal("brushStrengthChanged", value);
}

void TerrainControlDock::setSelectedBrushIndex(const int index) {
    _selectedBrushIndex = index;

    updateSelectedBrush();
}

void TerrainControlDock::selectToolType(const TerrainToolType toolType) {
    _selectedTool = toolType;
    updateSelectedTerrainTool();
    emit_signal("toolTypeSelected", toolType);
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
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_add.png"));
                                    toolPreview->set_tooltip_text("Raise terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_remove.png"));
                                    toolPreview->set_tooltip_text("Lower terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_smooth.png"));
                                    toolPreview->set_tooltip_text("Smooth terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_flatten.png"));
                                    toolPreview->set_tooltip_text("Flatten terrain");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSETHEIGHT);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_height.png"));
                                    toolPreview->set_tooltip_text("Set terrain height");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSETANGLE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_angle.png"));
                                    toolPreview->set_tooltip_text("Set terrain angle");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_PAINT);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/paint.png"));
                                    toolPreview->set_tooltip_text("Paint texture");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_add.png"));
                                    toolPreview->set_tooltip_text("Add foliage");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_remove.png"));
                                    toolPreview->set_tooltip_text("Remove foliage");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_OBJECTADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_add.png"));
                                    toolPreview->set_tooltip_text("Add objects");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_remove.png"));
                                    toolPreview->set_tooltip_text("Remove objects");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_add.png"));
                                    toolPreview->set_tooltip_text("Add water");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_remove.png"));
                                    toolPreview->set_tooltip_text("Remove water");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_add.png"));
                                    toolPreview->set_tooltip_text("Add water flow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERFLOWREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_remove.png"));
                                    toolPreview->set_tooltip_text("Remove water flow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_SNOWADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_add.png"));
                                    toolPreview->set_tooltip_text("Add snow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_SNOWREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_remove.png"));
                                    toolPreview->set_tooltip_text("Remove snow");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_HOLEADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_add.png"));
                                    toolPreview->set_tooltip_text("Add hole");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_HOLEREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_remove.png"));
                                    toolPreview->set_tooltip_text("Remove hole");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_LOCKADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_add.png"));
                                    toolPreview->set_tooltip_text("Lock");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_LOCKREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_remove.png"));
                                    toolPreview->set_tooltip_text("Unlock");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_METAINFOADD);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/tag_add.png"));
                                    toolPreview->set_tooltip_text("Add meta info");
                                    _toolTypesContainer->add_child(toolPreview);

                                    toolPreview = memnew(ToolPreview);
                                    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE);
                                    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/tag_remove.png"));
                                    toolPreview->set_tooltip_text("Remove meta info");
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