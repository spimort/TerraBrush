#include "terra_brush_plugin.h"
#include "misc/keybind_manager.h"
#include "misc/setting_contants.h"
#include "misc/zone_info.h"
#include "misc/zone_utils.h"
#include "misc/custom_content_loader.h"
#include "misc/utils.h"
#include "misc/dialog_utils.h"
#include "misc/importer_engine.h"
#include "misc/exporter_engine.h"

#include "editor_nodes/brush_numeric_selector.h"
#include "editor_nodes/tools_pie_menu.h"
#include "editor_nodes/custom_content_pie_menu.h"
#include "editor_nodes/keybind_settings.h"

#include "editor_resources/zone_resource.h"

#include "editor_tools/sculpt_tool.h"
#include "editor_tools/set_height_tool.h"
#include "editor_tools/set_angle_tool.h"
#include "editor_tools/texture_tool.h"
#include "editor_tools/foliage_tool.h"
#include "editor_tools/object_tool.h"
#include "editor_tools/water_tool.h"
#include "editor_tools/water_flow_tool.h"
#include "editor_tools/snow_tool.h"
#include "editor_tools/hole_tool.h"
#include "editor_tools/lock_tool.h"
#include "editor_tools/meta_info_tool.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/sub_viewport_container.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void TerraBrushPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("handleKeyBindings"), &TerraBrushPlugin::handleKeyBindings);
    ClassDB::bind_method(D_METHOD("updateTerrainSettings"), &TerraBrushPlugin::updateTerrainSettings);
    ClassDB::bind_method(D_METHOD("updateAutoAddZonesSetting"), &TerraBrushPlugin::updateAutoAddZonesSetting);
    ClassDB::bind_method(D_METHOD("importTerrain"), &TerraBrushPlugin::importTerrain);
    ClassDB::bind_method(D_METHOD("exportTerrain"), &TerraBrushPlugin::exportTerrain);

    ClassDB::bind_method(D_METHOD("onDockToolTypeSelected", "toolType"), &TerraBrushPlugin::onDockToolTypeSelected);
    ClassDB::bind_method(D_METHOD("onDockBrushSelected", "index"), &TerraBrushPlugin::onDockBrushSelected);
    ClassDB::bind_method(D_METHOD("onDockBrushSizeChanged", "value"), &TerraBrushPlugin::onDockBrushSizeChanged);
    ClassDB::bind_method(D_METHOD("onDockBrushStrengthChanged", "value"), &TerraBrushPlugin::onDockBrushStrengthChanged);
    ClassDB::bind_method(D_METHOD("onDockTextureSelected", "index"), &TerraBrushPlugin::onDockTextureSelected);
    ClassDB::bind_method(D_METHOD("onDockFoliageSelected", "index"), &TerraBrushPlugin::onDockFoliageSelected);
    ClassDB::bind_method(D_METHOD("onDockObjectSelected", "index"), &TerraBrushPlugin::onDockObjectSelected);
    ClassDB::bind_method(D_METHOD("onDockMetaInfoSelected", "index"), &TerraBrushPlugin::onDockMetaInfoSelected);
    ClassDB::bind_method(D_METHOD("onDockColorSelected", "value"), &TerraBrushPlugin::onDockColorSelected);

    ClassDB::bind_method(D_METHOD("onTerraBrushEditorToolTypeSelected", "toolType"), &TerraBrushPlugin::onTerraBrushEditorToolTypeSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorBrushSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorBrushSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorBrushSizeChanged", "value"), &TerraBrushPlugin::onTerraBrushEditorBrushSizeChanged);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorBrushStrengthChanged", "value"), &TerraBrushPlugin::onTerraBrushEditorBrushStrengthChanged);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorTextureSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorTextureSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorFoliageSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorFoliageSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorObjectSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorObjectSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorMetaInfoSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorMetaInfoSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorColorSelected", "index"), &TerraBrushPlugin::onTerraBrushEditorColorSelected);
    ClassDB::bind_method(D_METHOD("onTerraBrushEditorAutoAddZoneChanged", "value"), &TerraBrushPlugin::onTerraBrushEditorAutoAddZoneChanged);

    ClassDB::bind_method(D_METHOD("onTerrainMenuItemPressed", "id"), &TerraBrushPlugin::onTerrainMenuItemPressed);
}

TerraBrushPlugin::TerraBrushPlugin() {
}

TerraBrushPlugin::~TerraBrushPlugin() {}

void TerraBrushPlugin::_enter_tree() {
    Ref<KeybindManager> keybindManager = memnew(KeybindManager);

    createCustomSetting(SettingContants::ShowDecalBubble(), SettingContants::ShowDecalBubbleDefaultValue(), Variant::Type::BOOL);
    createCustomSetting(SettingContants::DecalColor(), SettingContants::DecalColorDefaultValue(), Variant::Type::COLOR);
    createCustomSetting(SettingContants::CustomBrushesFolder(), SettingContants::CustomBrushesFolderDefaultValue(), Variant::Type::STRING);
    createCustomSetting(SettingContants::SculptingMultiplier(), SettingContants::SculptingMultiplierDefaultValue(), Variant::Type::INT);
    createCustomSetting(SettingContants::IconsColor(), SettingContants::IconsColorDefaultValue(), Variant::Type::COLOR);
    createCustomSetting(SettingContants::AutoDetectTextures(), SettingContants::AutoDetectTexturesDefaultValue(), Variant::Type::BOOL);

    _terrainDockContainer = memnew(Control);
    _terrainDockContainer->set_name("Terrain Editor");
    add_control_to_dock(DockSlot::DOCK_SLOT_RIGHT_BL, _terrainDockContainer);

    keybindManager->registerInputMap();
    keybindManager->loadEditorSettings();
    add_tool_menu_item("TerraBrush Key bindings", Callable(this, "handleKeyBindings"));

    _terraBrushEditor = memnew(TerraBrushEditor);
    _terraBrushEditor->set_containerNode(EditorInterface::get_singleton()->get_base_control());
    _terraBrushEditor->connect("toolTypeSelected", Callable(this, "onTerraBrushEditorToolTypeSelected"));
    _terraBrushEditor->connect("brushSelected", Callable(this, "onTerraBrushEditorBrushSelected"));
    _terraBrushEditor->connect("brushSizeChanged", Callable(this, "onTerraBrushEditorBrushSizeChanged"));
    _terraBrushEditor->connect("brushStrengthChanged", Callable(this, "onTerraBrushEditorBrushStrengthChanged"));
    _terraBrushEditor->connect("textureSelected", Callable(this, "onTerraBrushEditorTextureSelected"));
    _terraBrushEditor->connect("foliageSelected", Callable(this, "onTerraBrushEditorFoliageSelected"));
    _terraBrushEditor->connect("objectSelected", Callable(this, "onTerraBrushEditorObjectSelected"));
    _terraBrushEditor->connect("metaInfoSelected", Callable(this, "onTerraBrushEditorMetaInfoSelected"));
    _terraBrushEditor->connect("colorSelected", Callable(this, "onTerraBrushEditorColorSelected"));
    _terraBrushEditor->connect("autoAddZoneChanged", Callable(this, "onTerraBrushEditorAutoAddZoneChanged"));
    EditorInterface::get_singleton()->get_base_control()->add_child(_terraBrushEditor);
}

void TerraBrushPlugin::_exit_tree() {
    if (_terrainDockContainer != nullptr) {
        remove_control_from_docks(_terrainDockContainer);
        _terrainDockContainer->queue_free();

        _terrainDockContainer = nullptr;
    }

    if (_terraBrushEditor != nullptr) {
        _terraBrushEditor->queue_free();
        _terraBrushEditor = nullptr;
    }

    remove_tool_menu_item("TerraBrush Key bindings");

    onExitEditTerrainNode();
}

void TerraBrushPlugin::_edit(Object *object) {
    if (_handles(object)) {
        onEditTerrainNode(Object::cast_to<TerraBrush>(object));
    } else {
        onExitEditTerrainNode();
    }
}

bool TerraBrushPlugin::_handles(Object *object) const {
    return Object::cast_to<TerraBrush>(object) != nullptr;
}

void TerraBrushPlugin::_save_external_data() {
    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->saveResources();
    }
}

int TerraBrushPlugin::_forward_3d_gui_input(Camera3D *viewportCamera, const Ref<InputEvent> &event) {
    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->updateCameraPosition(viewportCamera);
    }

    bool preventGuiInput = _terraBrushEditor->onGuiInput(viewportCamera, event);
    if (preventGuiInput) {
        return AFTER_GUI_INPUT_STOP;
    } else {
        return EditorPlugin::_forward_3d_gui_input(viewportCamera, event);
    }
}

void TerraBrushPlugin::createCustomSetting(String name, Variant defaultValue, Variant::Type type, PropertyHint hint, String hintString) {
    if (ProjectSettings::get_singleton()->has_setting(name)) {
        return;
    }

    Dictionary propertyInfo = Dictionary();
    propertyInfo["name"] = name;
    propertyInfo["type"] = (int)type;
    propertyInfo["hint"] = (int)hint;
    propertyInfo["hint_string"] = hintString;

    ProjectSettings::get_singleton()->set_setting(name, defaultValue);
    ProjectSettings::get_singleton()->add_property_info(propertyInfo);
    ProjectSettings::get_singleton()->set_initial_value(name, defaultValue);
}

void TerraBrushPlugin::handleKeyBindings() {
    KeybindSettings *dlg = memnew(KeybindSettings);
    get_tree()->get_root()->add_child(dlg);
    dlg->popup_centered();
}

void TerraBrushPlugin::removeDock() {
    if (_terrainControlDock != nullptr) {
        if (UtilityFunctions::is_instance_id_valid(_terrainControlDock->get_instance_id())) {
            _terrainControlDock->queue_free();
        }
        _terrainControlDock = nullptr;
    }

    if (_terrainMenuButton != nullptr) {
        remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _terrainMenuButton);
        _terrainMenuButton->queue_free();

        _terrainMenuButton = nullptr;
    }

    if (_updateTerrainSettingsButton != nullptr) {
        remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _updateTerrainSettingsButton);
        _updateTerrainSettingsButton->queue_free();

        _updateTerrainSettingsButton = nullptr;
    }

    if (_autoAddZonesCheckbox != nullptr) {
        remove_control_from_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _autoAddZonesCheckbox);
        _autoAddZonesCheckbox->queue_free();

        _autoAddZonesCheckbox = nullptr;
    }
}

void TerraBrushPlugin::onEditTerrainNode(TerraBrush *terraBrush) {
    removeDock();

    _currentTerraBrushNode = terraBrush;
    if (_terraBrushEditor != nullptr) {
        _terraBrushEditor->set_terraBrushNode(terraBrush);
        _terraBrushEditor->set_editorUndoRedo(get_undo_redo());
        _terraBrushEditor->set_enabled(true);
    }

    addDock();

    terraBrush->set_meta("_edit_lock_", true);
}

void TerraBrushPlugin::addDock() {
    _terrainControlDock = memnew(TerrainControlDock);
    _terrainControlDock->set_terraBrush(_currentTerraBrushNode);
    _terrainControlDock->set_editorResourcePreview(EditorInterface::get_singleton()->get_resource_previewer());
    _terrainDockContainer->add_child(_terrainControlDock);

    _terrainControlDock->connect("toolTypeSelected", Callable(this, "onDockToolTypeSelected"));
    _terrainControlDock->connect("brushSelected", Callable(this, "onDockBrushSelected"));
    _terrainControlDock->connect("brushSizeChanged", Callable(this, "onDockBrushSizeChanged"));
    _terrainControlDock->connect("brushStrengthChanged", Callable(this, "onDockBrushStrengthChanged"));
    _terrainControlDock->connect("textureSelected", Callable(this, "onDockTextureSelected"));
    _terrainControlDock->connect("foliageSelected", Callable(this, "onDockFoliageSelected"));
    _terrainControlDock->connect("objectSelected", Callable(this, "onDockObjectSelected"));
    _terrainControlDock->connect("metaInfoSelected", Callable(this, "onDockMetaInfoSelected"));
    _terrainControlDock->connect("colorSelected", Callable(this, "onDockColorSelected"));

    _terrainControlDock->setBrushSize(_terraBrushEditor->get_brushSize());
    _terrainControlDock->setBrushStrength(_terraBrushEditor->get_brushStrength());
    _terrainControlDock->setSelectedBrushIndex(_terraBrushEditor->get_brushIndex());
    _terrainControlDock->selectToolType(_terraBrushEditor->get_selectedToolType());
    _terrainControlDock->setSelectedTextureIndex(_terraBrushEditor->get_selectedTextureIndex());
    _terrainControlDock->setSelectedFoliageIndex(_terraBrushEditor->get_selectedFoliageIndex());
    _terrainControlDock->setSelectedObjectIndex(_terraBrushEditor->get_selectedObjectIndex());
    _terrainControlDock->setSelectedMetaInfoIndex(_terraBrushEditor->get_selectedMetaInfoIndex());
    _terrainControlDock->setSelectedColor(_terraBrushEditor->get_selectedColor());

    Ref<Theme> iconTheme = EditorInterface::get_singleton()->get_editor_theme();

    _terrainMenuButton = memnew(MenuButton);
    _terrainMenuButton->set_text("Terrain");
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Add", "EditorIcons"), "Create terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_CREATETERRAIN);
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Remove", "EditorIcons"), "Remove terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_REMOVETERRAIN);
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Reload", "EditorIcons"), "Update terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_UPDATETERRAIN);
    _terrainMenuButton->get_popup()->add_separator();
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("ImageTexture3D", "EditorIcons"), "Import terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_IMPORTTERRAIN);
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Image", "EditorIcons"), "Export terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_EXPORTTERRAIN);
    _terrainMenuButton->get_popup()->add_separator();
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Lock", "EditorIcons"), "Lock all terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_LOCKALLTERRAIN);
    _terrainMenuButton->get_popup()->add_icon_item(iconTheme->get_icon("Unlock", "EditorIcons"), "Unlock all terrain", TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_UNLOCKALLTERRAIN);
    _terrainMenuButton->get_popup()->connect("id_pressed", Callable(this, "onTerrainMenuItemPressed"));
    add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _terrainMenuButton);

    _updateTerrainSettingsButton = memnew(Button);
    _updateTerrainSettingsButton->set_text("Update terrain");
    _updateTerrainSettingsButton->connect("pressed", Callable(this, "updateTerrainSettings"));
    add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _updateTerrainSettingsButton);

    _autoAddZonesCheckbox = memnew(CheckBox);
    _autoAddZonesCheckbox->set_text("Auto add zones");
    _autoAddZonesCheckbox->set_pressed(_terraBrushEditor->get_autoAddZones());
    _autoAddZonesCheckbox->connect("pressed", Callable(this, "updateAutoAddZonesSetting"));
    add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _autoAddZonesCheckbox);
}

void TerraBrushPlugin::onExitEditTerrainNode() {
    removeDock();

    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->set_meta("_edit_lock_", false);
        _currentTerraBrushNode = nullptr;
    }

    if (_terraBrushEditor != nullptr) {
        _terraBrushEditor->set_enabled(false);
        _terraBrushEditor->set_terraBrushNode(nullptr);
    }
}

void TerraBrushPlugin::updateTerrainSettings() {
    _currentTerraBrushNode->onUpdateTerrainSettings();
    removeDock();
    addDock();
}

void TerraBrushPlugin::onDockToolTypeSelected(const TerrainToolType toolType) {
    _terraBrushEditor->set_selectedToolType(toolType);
}

void TerraBrushPlugin::onDockBrushSelected(const int index) {
    _terraBrushEditor->set_brushIndex(index);
}

void TerraBrushPlugin::onDockBrushSizeChanged(const int value) {
    _terraBrushEditor->set_brushSize(value);
}

void TerraBrushPlugin::onDockBrushStrengthChanged(const float value) {
    _terraBrushEditor->set_brushStrength(value);
}

void TerraBrushPlugin::onDockTextureSelected(const int index) {
    _terraBrushEditor->set_selectedTextureIndex(index);
}

void TerraBrushPlugin::onDockFoliageSelected(const int index) {
    _terraBrushEditor->set_selectedFoliageIndex(index);
}

void TerraBrushPlugin::onDockObjectSelected(const int index) {
    _terraBrushEditor->set_selectedObjectIndex(index);
}

void TerraBrushPlugin::onDockMetaInfoSelected(const int index) {
    _terraBrushEditor->set_selectedMetaInfoIndex(index);
}

void TerraBrushPlugin::onDockColorSelected(const Color value) {
    _terraBrushEditor->set_selectedColor(value);
}

void TerraBrushPlugin::onTerraBrushEditorToolTypeSelected(const TerrainToolType toolType) {
    _terrainControlDock->selectToolType(toolType);
}

void TerraBrushPlugin::onTerraBrushEditorBrushSelected(const int index) {
    _terrainControlDock->setSelectedBrushIndex(index);
}

void TerraBrushPlugin::onTerraBrushEditorBrushSizeChanged(const int value) {
    _terrainControlDock->setBrushSize(value);
}

void TerraBrushPlugin::onTerraBrushEditorBrushStrengthChanged(const float value) {
    _terrainControlDock->setBrushStrength(value);
}

void TerraBrushPlugin::onTerraBrushEditorTextureSelected(const int index) {
    _terrainControlDock->setSelectedTextureIndex(index);
}

void TerraBrushPlugin::onTerraBrushEditorFoliageSelected(const int index) {
    _terrainControlDock->setSelectedFoliageIndex(index);
}

void TerraBrushPlugin::onTerraBrushEditorObjectSelected(const int index) {
    _terrainControlDock->setSelectedObjectIndex(index);
}

void TerraBrushPlugin::onTerraBrushEditorMetaInfoSelected(const int index) {
    _terrainControlDock->setSelectedMetaInfoIndex(index);
}

void TerraBrushPlugin::onTerraBrushEditorColorSelected(const Color value) {
    _terrainControlDock->setSelectedColor(value);
}

void TerraBrushPlugin::onTerraBrushEditorAutoAddZoneChanged(const bool value) {
    _autoAddZonesCheckbox->set_pressed(value);
}

void TerraBrushPlugin::importTerrain() {
    DialogUtils::showImportDialog(this, _currentTerraBrushNode, ([&](ImporterSettings settings) {
        ImporterEngine::importTerrain(_currentTerraBrushNode, settings);
        _currentTerraBrushNode->onUpdateTerrainSettings();
    }));
}

void TerraBrushPlugin::exportTerrain() {
    DialogUtils::showFileDialog(
        this,
        ([&](String folder) {
            ExporterEngine::exportTerrain(_currentTerraBrushNode, folder);
        }),
        EditorFileDialog::Access::ACCESS_FILESYSTEM,
        EditorFileDialog::FileMode::FILE_MODE_OPEN_DIR
    );
}

void TerraBrushPlugin::onTerrainMenuItemPressed(const int id) {
    switch (id) {
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_CREATETERRAIN:
            _currentTerraBrushNode->onCreateTerrain();
            removeDock();
            addDock();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_REMOVETERRAIN:
            _currentTerraBrushNode->onRemoveTerrain();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_UPDATETERRAIN:
            updateTerrainSettings();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_IMPORTTERRAIN:
            importTerrain();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_EXPORTTERRAIN:
            exportTerrain();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_LOCKALLTERRAIN:
            _currentTerraBrushNode->onLockTerrain();
            break;
        case TerrainMenuButtonAction::TERRAINMENUBUTTONACTION_UNLOCKALLTERRAIN:
            _currentTerraBrushNode->onUnlockTerrain();
            break;
    }
}

void TerraBrushPlugin::updateAutoAddZonesSetting() {
    _terraBrushEditor->set_autoAddZones(_autoAddZonesCheckbox->is_pressed());
}