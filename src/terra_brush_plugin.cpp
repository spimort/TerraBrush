#include "terra_brush_plugin.h"
#include "misc/keybind_manager.h"
#include "misc/setting_contants.h"
#include "misc/zone_info.h"
#include "misc/zone_utils.h"
#include "misc/custom_content_loader.h"
#include "misc/utils.h"
#include "editor_nodes/brush_numeric_selector.h"
#include "editor_nodes/tools_pie_menu.h"
#include "editor_nodes/custom_content_pie_menu.h"
#include "editor_resources/zone_resource.h"
#include "editor_tools/sculpt_tool.h"
#include "editor_tools/set_height_tool.h"
#include "editor_tools/set_angle_tool.h"
#include "editor_tools/texture_tool.h"

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
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void TerraBrushPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("handleKeyBindings"), &TerraBrushPlugin::handleKeyBindings);
    ClassDB::bind_method(D_METHOD("onPieMenuBrushSelected", "customContentPieMenu"), &TerraBrushPlugin::onPieMenuBrushSelected);
    ClassDB::bind_method(D_METHOD("onPieMenuBrushIndexSelected", "brushIndex"), &TerraBrushPlugin::onPieMenuBrushIndexSelected);
    ClassDB::bind_method(D_METHOD("onBrushSizeSelected", "value"), &TerraBrushPlugin::onBrushSizeSelected);
    ClassDB::bind_method(D_METHOD("onBrushStrengthSelected", "value"), &TerraBrushPlugin::onBrushStrengthSelected);
    ClassDB::bind_method(D_METHOD("onUndoRedo"), &TerraBrushPlugin::onUndoRedo);
    ClassDB::bind_method(D_METHOD("updateTerrainSettings"), &TerraBrushPlugin::updateTerrainSettings);
    ClassDB::bind_method(D_METHOD("updateAutoAddZonesSetting"), &TerraBrushPlugin::updateAutoAddZonesSetting);
    ClassDB::bind_method(D_METHOD("onToolSelected", "value"), &TerraBrushPlugin::onToolSelected);
    ClassDB::bind_method(D_METHOD("hideOverlaySelector"), &TerraBrushPlugin::hideOverlaySelector);

    ClassDB::bind_method(D_METHOD("onDockToolTypeSelected", "toolType"), &TerraBrushPlugin::onDockToolTypeSelected);
    ClassDB::bind_method(D_METHOD("onDockBrushSelected", "index", "image"), &TerraBrushPlugin::onDockBrushSelected);
    ClassDB::bind_method(D_METHOD("onDockBrushSizeChanged", "value"), &TerraBrushPlugin::onDockBrushSizeChanged);
    ClassDB::bind_method(D_METHOD("onDockBrushStrengthChanged", "value"), &TerraBrushPlugin::onDockBrushStrengthChanged);
    ClassDB::bind_method(D_METHOD("onDockTextureSelected", "index"), &TerraBrushPlugin::onDockTextureSelected);
    ClassDB::bind_method(D_METHOD("onDockFoliageSelected", "index"), &TerraBrushPlugin::onDockFoliageSelected);
    ClassDB::bind_method(D_METHOD("onDockObjectSelected", "index"), &TerraBrushPlugin::onDockObjectSelected);
    ClassDB::bind_method(D_METHOD("onDockMetaInfoSelected", "index"), &TerraBrushPlugin::onDockMetaInfoSelected);
}

TerraBrushPlugin::TerraBrushPlugin() {
    _updateTime = 0;
    _autoAddZones = false;
    _preventInitialDo = false;
    _currentTool = Ref<ToolBase>(nullptr);
    _currentToolType = TerrainToolType::TERRAINTOOLTYPE_TERRAINADD;
    _brushImage = Ref<Image>(nullptr);
    _brushSize = 100;
    _brushStrength = 0.1;
    _brushIndex = 0;
    _textureIndex = -1;
    _foliageIndex = -1;
    _objectIndex = -1;
    _metaInfoLayerIndex = -1;
}

TerraBrushPlugin::~TerraBrushPlugin() {}

void TerraBrushPlugin::_physics_process(double delta) {
    if (!_isMousePressed) {
        _updateTime = 0;
    } else if (_updateTime > 0) {
        _updateTime -= (float) delta;
    } else if (_isMousePressed && _mouseHitPosition != Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue) && !_currentTool.is_null()) {
        int zoneSize = _currentTerraBrushNode->get_zonesSize();
        Vector3 meshToImagePosition = _mouseHitPosition + Vector3(zoneSize / 2, 0, zoneSize / 2);
        Vector2 imagePosition = Vector2(meshToImagePosition.x, meshToImagePosition.z);

        _currentTool->paint(_currentToolType, _brushImage, _brushSize, _brushStrength, imagePosition);

        _updateTime = UpdateDelay;
    }
}

void TerraBrushPlugin::_enter_tree() {
    Ref<KeybindManager> keybindManager = memnew(KeybindManager);

    createCustomSetting(SettingContants::DecalColor(), Color(1.0f, 0, 0, 0.5f), Variant::Type::COLOR);
    createCustomSetting(SettingContants::CustomBrushesFolder(), "res://TerraBrush_CustomBrushes", Variant::Type::STRING);
    createCustomSetting(SettingContants::SculptingMultiplier(), 10, Variant::Type::INT);
    createCustomSetting(SettingContants::IconsColor(), Color::html("#00151F"), Variant::Type::COLOR);

    _terrainDockContainer = memnew(Control);
    _terrainDockContainer->set_name("Terrain Editor");
    add_control_to_dock(DockSlot::DOCK_SLOT_RIGHT_BL, _terrainDockContainer);

    _editorViewportsContainer = getEditorViewportsContainer();

    _editorViewports = Array();
    for (int i = 0; i < _editorViewportsContainer->get_child_count(); i++) {
        _editorViewports.append(_editorViewportsContainer->get_child(i));
    }

    keybindManager->registerInputMap();
    keybindManager->loadEditorSettings();
    add_tool_menu_item("TerraBrush Key bindings", Callable(this, "handleKeyBindings"));
}

void TerraBrushPlugin::_exit_tree() {
    if (_terrainDockContainer != nullptr) {
        remove_control_from_docks(_terrainDockContainer);
        _terrainDockContainer->queue_free();

        _terrainDockContainer = nullptr;
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
    bool preventGuiInput = false;

    if (_toolInfo != nullptr) {
        _toolInfo->set_position(viewportCamera->get_viewport()->get_mouse_position() + Object::cast_to<Control>(viewportCamera->get_viewport()->get_parent())->get_global_position() + Vector2i(ToolInfoOffset, ToolInfoOffset));

        if (!_currentTool.is_null()) {
            _toolInfo->setText(_currentTool->getToolInfo(_currentToolType));
        }
    }

    if (Object::cast_to<InputEventMouseMotion>(event.ptr()) != nullptr) {
        Ref<InputEventMouseMotion> inputMotion = Object::cast_to<InputEventMouseMotion>(event.ptr());
        Vector3 meshPosition = getRayCastWithTerrain(viewportCamera);
        if (meshPosition == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
            _brushDecal->set_visible(false);
        } else {
            _brushDecal->set_visible(true);
            _brushDecal->set_position(Vector3(meshPosition.x, 0, meshPosition.z));
        }

        _mouseHitPosition = meshPosition - _currentTerraBrushNode->get_global_position();
    }

    if (Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
        Ref<InputEventKey> inputEvent = Object::cast_to<InputEventKey>(event.ptr());
        _terrainControlDock->setShiftPressed(Input::get_singleton()->is_key_pressed(Key::KEY_SHIFT));

        if (!inputEvent->is_pressed() || inputEvent->is_echo()) return EditorPlugin::_forward_3d_gui_input(viewportCamera, event);

        if (inputEvent->is_action(KeybindManager::StringNames::ToolPie())) {
            showToolPieMenu(KeybindManager::StringNames::ToolPie());
            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::BrushPie())) {
            showCustomContentPieMenu("Brushes", Callable(this, "onPieMenuBrushSelected"));
            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::ToolContentPie())) {
            showCurrentToolMenu();
            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::BrushSizeSelector())) {
            showBrushNumericSelector(1, 200, Color::named("LIME_GREEN"), _brushSize, Callable(this, "onBrushSizeSelected"), KeybindManager::StringNames::BrushSizeSelector());

            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::BrushStrengthSelector())) {
            showBrushNumericSelector(1, 100, Color::named("CRIMSON"), (int) (_brushStrength * 100), Callable(this, "onBrushStrengthSelected"), KeybindManager::StringNames::BrushStrengthSelector());

            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::EscapeSelector()) && _overlaySelector != nullptr) {
            hideOverlaySelector();
            return AFTER_GUI_INPUT_STOP;
        }

        if (inputEvent->is_action(KeybindManager::StringNames::ToggleAutoAddZones())) {
            _autoAddZonesCheckbox->set_pressed(!_autoAddZonesCheckbox->is_pressed());
            updateAutoAddZonesSetting();
            return AFTER_GUI_INPUT_STOP;
        }
    }

    if (Object::cast_to<InputEventMouseButton>(event.ptr()) != nullptr) {
        Ref<InputEventMouseButton> inputButton = Object::cast_to<InputEventMouseButton>(event.ptr());
        if (_overlaySelector != nullptr) {
            if (Object::cast_to<BrushNumericSelector>(_overlaySelector) != nullptr) {
                BrushNumericSelector *brushNumericSelector = Object::cast_to<BrushNumericSelector>(_overlaySelector);
                brushNumericSelector->requestSelectValue();
            }

            hideOverlaySelector();
            return AFTER_GUI_INPUT_STOP;
        }

        if (inputButton->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            if (inputButton->is_pressed()) {
                if (_mouseHitPosition != Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
                    _undoRedo->create_action("Modify terrain");

                    // Trigger a dirty state
                    _undoRedo->add_undo_property(_currentTerraBrushNode, "zonesSize", _currentTerraBrushNode->get_zonesSize());

                    _isMousePressed = true;
                    preventGuiInput = true;

                    if (!_currentTool.is_null()) {
                        _currentTool->beginPaint();
                    }
                }
            } else if (_isMousePressed) {
                _currentTerraBrushNode->get_terrain()->terrainUpdated();
                _isMousePressed = false;

                // Trigger a dirty state
                _undoRedo->add_do_property(_currentTerraBrushNode, "zonesSize", _currentTerraBrushNode->get_zonesSize());

                if (!_currentTool.is_null()) {
                    _currentTool->endPaint();
                }

                _undoRedo->add_undo_method(this, "onUndoRedo");
                _undoRedo->add_do_method(this, "onUndoRedo");

                _preventInitialDo = true;
                _undoRedo->commit_action();
                _preventInitialDo = false;
            }
        }
    }

    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->updateCameraPosition(viewportCamera);
    }

    if (preventGuiInput) {
        return AFTER_GUI_INPUT_STOP;
    } else {
        if (!_currentTool.is_null() && _currentTool->handleInput(_currentToolType, event)) {
            return AFTER_GUI_INPUT_STOP;
        }

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
    // TODO : GDExtension

    // var dlg = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/KeybindSettings.tscn")
    //     .Instantiate<KeybindSettings>();
    // dlg.Confirmed += () => dlg.QueueFree();
    // GetTree().Root.AddChild(dlg);
    // dlg.PopupCentered();
}

void TerraBrushPlugin::onUndoTexture(Ref<ImageTexture> imageTexture, PackedByteArray previousImageData) {
    if (_preventInitialDo) {
        return;
    }

    Ref<Image> image = memnew(Image);
    image->set_data(imageTexture->get_width(), imageTexture->get_height(), imageTexture->get_image()->has_mipmaps(), imageTexture->get_format(), previousImageData);
    imageTexture->update(image);
}

void TerraBrushPlugin::onUndoRedo() {
    if (_preventInitialDo) {
        return;
    }

    _currentTerraBrushNode->get_terrain()->terrainUpdated();
    if (!_currentTerraBrushNode->get_terrainZones().is_null()) {
        _currentTerraBrushNode->get_terrainZones()->updateImageTextures(_currentTerraBrushNode->get_zonesSize());
    }

    _currentTerraBrushNode->clearObjects();
    _currentTerraBrushNode->createObjects();
}

Vector3 TerraBrushPlugin::getRayCastWithTerrain(Camera3D *editorCamera) {
    PhysicsDirectSpaceState3D *spaceState = _currentTerraBrushNode->get_world_3d()->get_direct_space_state();

    if (Object::cast_to<SubViewport>(editorCamera->get_viewport())) {
        SubViewport *viewport = Object::cast_to<SubViewport>(editorCamera->get_viewport());
        if (Object::cast_to<SubViewportContainer>(viewport->get_parent())) {
            SubViewportContainer *viewportContainer = Object::cast_to<SubViewportContainer>(viewport->get_parent());
            Vector2 screenPosition = editorCamera->get_viewport()->get_mouse_position();

            Vector3 from = editorCamera->project_ray_origin(screenPosition);
            Vector3 dir = editorCamera->project_ray_normal(screenPosition);

            float distance = 2000;
            Ref<PhysicsRayQueryParameters3D> query = memnew(PhysicsRayQueryParameters3D);
            query->set_from(from);
            query->set_to(from + dir * distance);
            Dictionary result = spaceState->intersect_ray(query);

            if (_currentTerraBrushNode->get_terrain() != nullptr && result.size() > 0) {
                CollisionObject3D *collider = Object::cast_to<CollisionObject3D>(result["collider"]);
                if (collider == _currentTerraBrushNode->get_terrain()->get_terrainCollider()) {
                    return (Vector3)result["position"] + Vector3(0, 0.1f, 0);
                }
            }

            return getMouseClickToZoneHeight(from, dir);
        }
    }

    return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
}

Vector3 TerraBrushPlugin::getMouseClickToZoneHeight(Vector3 from, Vector3 direction) {
    TypedDictionary<Ref<ImageTexture>, Ref<Image>> heightmapsCache = TypedDictionary<Ref<ImageTexture>, Ref<Image>>();

    for (int i = 0; i < 20000; i++) {
        Vector3 position = from + (direction * i * 0.1f) - _currentTerraBrushNode->get_global_position();

        ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(position.x + (_currentTerraBrushNode->get_zonesSize() / 2), position.z + (_currentTerraBrushNode->get_zonesSize() / 2), _currentTerraBrushNode->get_zonesSize(), _currentTerraBrushNode->get_resolution());
        Ref<ZoneResource> zone;
        if (!_currentTerraBrushNode->get_terrainZones().is_null()) {
            zone = _currentTerraBrushNode->get_terrainZones()->getZoneForZoneInfo(zoneInfo);
        }

        if (!zone.is_null() && !zone->get_heightMapTexture().is_null()) {
            Ref<Image> heightMapImage;
            if (heightmapsCache.has(zone->get_heightMapTexture())) {
                heightMapImage = heightmapsCache[zone->get_heightMapTexture()];
            }

            if (heightMapImage.is_null()) {
                heightMapImage = zone->get_heightMapTexture()->get_image();
                heightmapsCache[zone->get_heightMapTexture()] = heightMapImage;
            }

            float zoneHeight = heightMapImage->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y).r;

            if (zoneHeight >= position.y) {
                return Vector3(position.x, zoneHeight, position.z) + _currentTerraBrushNode->get_global_position();
            }
        }

    }

    return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
}

void TerraBrushPlugin::removeDock() {
    if (_terrainControlDock != nullptr) {
        if (UtilityFunctions::is_instance_id_valid(_terrainControlDock->get_instance_id())) {
            _terrainControlDock->queue_free();
        }
        _terrainControlDock = nullptr;
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
    if (_brushDecal != nullptr) {
        _brushDecal->queue_free();
    }

    _brushDecal = memnew(BrushDecal);
    _brushDecal->set_name("BrushDecal");
    add_child(_brushDecal);

    _brushDecal->setSize(_brushSize);
    if (!_brushImage.is_null()) {
        _brushDecal->setBrushImage(_brushImage);
    }

    _currentTerraBrushNode = terraBrush;
    // TODO : GDExtension
    // _currentTerraBrushNode.TerrainSettingsUpdated += () => {
    //     RemoveDock();
    //     AddDock();
    // };
    _undoRedo = get_undo_redo();
    updateCurrentTool();
    // TODO : GDExtension
    // _currentTerraBrushNode.UndoRedo = _undoRedo;

    if (_toolInfo != nullptr) {
        _toolInfo->queue_free();
    }

    _toolInfo = memnew(ToolInfo);
    _toolInfo->set_name("ToolInfo");
    add_child(_toolInfo);

    if (!_currentTerraBrushNode->get_textureSets().is_null() && _currentTerraBrushNode->get_textureSets()->get_textureSets().size() > 0 && _textureIndex < 0) {
        _textureIndex = 0;
    }

    if (_currentTerraBrushNode->get_foliages().size() > 0 && _foliageIndex < 0) {
        _foliageIndex = 0;
    }

    if (_currentTerraBrushNode->get_objects().size() > 0 && _objectIndex < 0) {
        _objectIndex = 0;
    }

    if (_currentTerraBrushNode->get_metaInfoLayers().size() > 0 && _metaInfoLayerIndex < 0) {
        _metaInfoLayerIndex = 0;
    }

    addDock();

    terraBrush->set_meta("_edit_lock_", true);
}

void TerraBrushPlugin::addDock() {
    _terrainControlDock = memnew(TerrainControlDock);
    _terrainControlDock->set_terraBrush(_currentTerraBrushNode);
    _terrainControlDock->set_brushDecal(_brushDecal);
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

    _terrainControlDock->setBrushSize(_brushSize);
    _terrainControlDock->setBrushStrength(_brushStrength);
    _terrainControlDock->setSelectedBrushIndex(_brushIndex);
    _terrainControlDock->selectToolType(_currentToolType);
    _terrainControlDock->setSelectedTextureIndex(_textureIndex);
    _terrainControlDock->setSelectedFoliageIndex(_foliageIndex);
    _terrainControlDock->setSelectedObjectIndex(_objectIndex);
    _terrainControlDock->setSelectedMetaInfoIndex(_metaInfoLayerIndex);

    _updateTerrainSettingsButton = memnew(Button);
    _updateTerrainSettingsButton->set_text("Update terrain");
    _updateTerrainSettingsButton->connect("pressed", Callable(this, "updateTerrainSettings"));
    add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _updateTerrainSettingsButton);

    _autoAddZonesCheckbox = memnew(CheckBox);
    _autoAddZonesCheckbox->set_text("Auto add zones");
    _autoAddZonesCheckbox->set_pressed(_autoAddZones);
    _autoAddZonesCheckbox->connect("pressed", Callable(this, "updateAutoAddZonesSetting"));
    add_control_to_container(CustomControlContainer::CONTAINER_SPATIAL_EDITOR_MENU, _autoAddZonesCheckbox);
}

void TerraBrushPlugin::onExitEditTerrainNode() {
    removeDock();
    hideOverlaySelector();

    if (_brushDecal != nullptr) {
        _brushDecal->queue_free();
        _brushDecal = nullptr;
    }

    if (_toolInfo != nullptr) {
        _toolInfo->queue_free();
        _toolInfo = nullptr;
    }

    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->set_meta("_edit_lock_", false);
        _currentTerraBrushNode = nullptr;
    }
}

Node *TerraBrushPlugin::getEditorViewportsContainer() {
    return getEditorViewportsContainerRecursive(EditorInterface::get_singleton()->get_base_control());
}

Node *TerraBrushPlugin::getEditorViewportsContainerRecursive(Node *node) {
    for (int i = 0; i < node->get_child_count(); i++) {
        Node *child = node->get_child(i);
        if (child->get_class() == "Node3DEditorViewportContainer") {
            return child;
        }

        Node *subChild = getEditorViewportsContainerRecursive(child);
        if (subChild != nullptr) {
            return subChild;
        }
    }

    return nullptr;
}

Node *TerraBrushPlugin::getActiveViewport() {
    for (int i = 0; i < _editorViewports.size(); i++) {
        Control *viewport = Object::cast_to<Control>(_editorViewports[i]);
        if (viewport->get_rect().has_point(viewport->get_local_mouse_position())) {
            return viewport;
        }
    }

    return nullptr;
}

StringName TerraBrushPlugin::hideOverlaySelector() {
    if (_overlaySelector != nullptr) {
        StringName overlayActionName = _overlaySelector->get_meta(OverlayActionNameKey, "");

        _overlaySelector->queue_free();
        _overlaySelector = nullptr;

        return overlayActionName;
    }

    return "";
}

void TerraBrushPlugin::showToolPieMenu(StringName actionName) {
    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != actionName) {
        Node *activeViewport = getActiveViewport();
        if (activeViewport != nullptr) {
            ToolsPieMenu *pieMenu = memnew(ToolsPieMenu);
            pieMenu->set_onToolSelected(Callable(this, "onToolSelected"));

            _overlaySelector = pieMenu;

            _overlaySelector->set_position(Object::cast_to<Control>(activeViewport)->get_global_mouse_position());
            _overlaySelector->set_meta(OverlayActionNameKey, actionName);

            EditorInterface::get_singleton()->get_base_control()->add_child(_overlaySelector);
        }
    }
}

void TerraBrushPlugin::showCustomContentPieMenu(String label, Callable addItems) {
    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != label) {
        Node *activeViewport = getActiveViewport();
        if (activeViewport != nullptr) {
            CustomContentPieMenu *customContentPieMenu = memnew(CustomContentPieMenu);

            _overlaySelector = customContentPieMenu;
            _overlaySelector->set_position(Object::cast_to<Control>(activeViewport)->get_global_mouse_position());
            _overlaySelector->set_meta((StringName)OverlayActionNameKey, label);

            EditorInterface::get_singleton()->get_base_control()->add_child(_overlaySelector);

            addItems.call(customContentPieMenu);

            customContentPieMenu->get_pieMenu()->set_label(label);
            customContentPieMenu->get_pieMenu()->updateContent();
        }
    }
}

void TerraBrushPlugin::showCurrentToolMenu() {
    // TODO : GDExtension
    switch (_currentToolType) {
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETHEIGHT:
            // var setHeightTool = (SetHeightTool)_currentTerraBrushNode.CurrentTool;
            // var heightValue = await DialogUtils.ShowNumericSelector(this, setHeightTool.GetSetHeightValue());
            // if (heightValue.HasValue) {
            //     setHeightTool.UpdateSetHeightValue(heightValue.Value);
            // }
            break;
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETANGLE:
            // var setAngleTool = (SetAngleTool)_currentTerraBrushNode.CurrentTool;
            // var angleValue = await DialogUtils.ShowNumericSelector(this, setAngleTool.GetSetAngleValue(), -89.9f, 89.9f);
            // if (angleValue.HasValue) {
            //     setAngleTool.UpdateSetAngleValue(angleValue.Value);
            // }
            break;
        case TerrainToolType::TERRAINTOOLTYPE_PAINT:
            // ShowCustomContentPieMenu("Textures", customContentPieMenu => {
            //     CustomContentLoader.AddTexturesPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
            //         _terrainControlDock.SetSelectedTextureIndex(index);
            //         HideOverlaySelector();
            //     }, true);
            // });

            break;
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD:
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE:
            // ShowCustomContentPieMenu("Foliages", customContentPieMenu => {
            //     CustomContentLoader.AddFoliagesPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
            //         _terrainControlDock.SetSelectedFoliageIndex(index);
            //         HideOverlaySelector();
            //     }, true);
            // });

            break;
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTADD:
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE:
            // ShowCustomContentPieMenu("Objects", customContentPieMenu => {
            //     CustomContentLoader.AddObjectsPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
            //         _terrainControlDock.SetSelectedObjectIndex(index);
            //         HideOverlaySelector();
            //     }, true);
            // });

            break;
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOADD:
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE:
            // ShowCustomContentPieMenu("MetaInfo", customContentPieMenu => {
            //     CustomContentLoader.AddMetaInfoLayersPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
            //         _terrainControlDock.SetSelectedMetaInfoIndex(index);
            //         HideOverlaySelector();
            //     }, true);
            // });

            break;
    }
}

void TerraBrushPlugin::showBrushNumericSelector(int minVale, int maxValue, Color widgetColor, int initialValue, Callable onValueSelected, StringName actionName) {
    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != actionName) {
        Node *activeViewport = getActiveViewport();
        if (activeViewport != nullptr) {
            BrushNumericSelector *selector = memnew(BrushNumericSelector);

            selector->set_minValue(minVale);
            selector->set_maxValue(maxValue);
            selector->set_widgetColor(widgetColor);

            _overlaySelector = selector;
            _overlaySelector->set_position(Object::cast_to<Control>(activeViewport)->get_global_mouse_position());
            _overlaySelector->set_meta(OverlayActionNameKey, actionName);

            EditorInterface::get_singleton()->get_base_control()->add_child(_overlaySelector);

            selector->setInitialValue(initialValue);

            selector->set_onValueSelected(onValueSelected);
            selector->set_onCancel(Callable(this, "hideOverlaySelector"));
        }
    }
}

void TerraBrushPlugin::updateTerrainSettings() {
    if (_currentTerraBrushNode != nullptr) {
        _currentTerraBrushNode->onUpdateTerrainSettings();
    }
}

void TerraBrushPlugin::updateAutoAddZonesSetting() {
    _autoAddZones = _autoAddZonesCheckbox->is_pressed();
    if (!_currentTool.is_null()) {
        _currentTool->set_autoAddZones(_autoAddZones);
    }
}

void TerraBrushPlugin::onPieMenuBrushSelected(const CustomContentPieMenu *customContentPieMenu) {
    CustomContentLoader::addBrushesPreviewToParent(customContentPieMenu->get_pieMenu(), Callable(this, "onPieMenuBrushIndexSelected"), true);
}

void TerraBrushPlugin::onPieMenuBrushIndexSelected(const int brushIndex) {
    _terrainControlDock->setSelectedBrushIndex(brushIndex);
    hideOverlaySelector();
}

void TerraBrushPlugin::onBrushSizeSelected(const int value) {
    _terrainControlDock->setBrushSize(value);
    hideOverlaySelector();
}

void TerraBrushPlugin::onBrushStrengthSelected(const int value) {
    _terrainControlDock->setBrushStrength(value / 100.0f);
    hideOverlaySelector();
}

void TerraBrushPlugin::onToolSelected(const TerrainToolType value) {
    _terrainControlDock->selectToolType(value);

    hideOverlaySelector();
}

void TerraBrushPlugin::onDockToolTypeSelected(const TerrainToolType toolType) {
    _currentToolType = toolType;
    updateCurrentTool();
}

void TerraBrushPlugin::onDockBrushSelected(const int index, const Ref<Image> &image) {
    _brushIndex = index;
    _originalBrushImage = image;

    // This is to trigger the resize of the brush image
    onDockBrushSizeChanged(_brushSize);
}

void TerraBrushPlugin::onDockBrushSizeChanged(const int value) {
    _brushSize = value;

    if (!_originalBrushImage.is_null()) {
        _brushImage = Ref<Image>(memnew(Image));
        _brushImage->copy_from(_originalBrushImage);
        _brushImage->resize(_brushSize, _brushSize);
    }
}

void TerraBrushPlugin::onDockBrushStrengthChanged(const float value) {
    _brushStrength = value;
}

void TerraBrushPlugin::onDockTextureSelected(const int index) {
    _textureIndex = index;

    if (!_currentTool.is_null() && Object::cast_to<TextureTool>(_currentTool.ptr()) != nullptr) {
        Ref<TextureTool> textureTool = Object::cast_to<TextureTool>(_currentTool.ptr());
        textureTool->updateSelectedTextureIndex(index);
    }
}

void TerraBrushPlugin::onDockFoliageSelected(const int index) {
    _foliageIndex = index;
}

void TerraBrushPlugin::onDockObjectSelected(const int index) {
    _objectIndex = index;
}

void TerraBrushPlugin::onDockMetaInfoSelected(const int index) {
    _metaInfoLayerIndex = index;
}

void TerraBrushPlugin::updateCurrentTool() {
    Ref<ToolBase> newTool = getToolForType(_currentToolType);
    if (newTool.is_null()) {
        if (!_currentTool.is_null()) {
            beforeDeselectTool();
        }
        _currentTool = Ref<ToolBase>(nullptr);
    } else if (_currentTool.is_null() || _currentTool->get_class() != newTool->get_class()) {
        if (!_currentTool.is_null()) {
            beforeDeselectTool();
        }
        _currentTool = newTool;

        _currentTool->init(_currentTerraBrushNode, _undoRedo, _autoAddZones);
    }
}

void TerraBrushPlugin::beforeDeselectTool() {
    if (Object::cast_to<SetHeightTool>(_currentTool.ptr()) != nullptr) {
        Ref<SetHeightTool> setHeightTool = Object::cast_to<SetHeightTool>(_currentTool.ptr());
        _selectedSetHeight = setHeightTool->getSetHeightValue();
    } else if (Object::cast_to<SetAngleTool>(_currentTool.ptr()) != nullptr) {
        Ref<SetAngleTool> setAngleTool = Object::cast_to<SetAngleTool>(_currentTool.ptr());
        _selectedSetAngle = setAngleTool->getSetAngleValue();
        _selectedSetAngleInitialPoint = setAngleTool->getSetAngleInitialPoint();
    }

    _currentTool->beforeDeselect();
}

Ref<ToolBase> TerraBrushPlugin::getToolForType(TerrainToolType toolType) {
    switch (toolType) {
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINADD:
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE:
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH:
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN:
            return memnew(SculptTool);
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETHEIGHT: {
            Ref<SetHeightTool> setHeightTool = memnew(SetHeightTool);
            setHeightTool->updateSetHeightValue(_selectedSetHeight);
            return setHeightTool;
        }
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETANGLE: {
            Ref<SetAngleTool> setAngleTool = memnew(SetAngleTool);
            setAngleTool->updateSetAngleValue(_selectedSetAngle);
            setAngleTool->updateSetAngleInitialPoint(_selectedSetAngleInitialPoint);
            return setAngleTool;
        }
        case TerrainToolType::TERRAINTOOLTYPE_PAINT: {
            Ref<TextureTool> textureTool = memnew(TextureTool);
            textureTool->updateSelectedTextureIndex(_textureIndex);
            return textureTool;
        }
        // case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD:
        // case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE:
        //     return memnew(FoliageTool);
        // case TerrainToolType::TERRAINTOOLTYPE_OBJECTADD:
        // case TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE:
        //     return memnew(ObjectTool);
        // case TerrainToolType::TERRAINTOOLTYPE_WATERADD:
        // case TerrainToolType::TERRAINTOOLTYPE_WATERREMOVE:
        //     return memnew(WaterTool);
        // case TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD:
        // case TerrainToolType::TERRAINTOOLTYPE_WATERFLOWREMOVE:
        //     return memnew(WaterFlowTool);
        // case TerrainToolType::TERRAINTOOLTYPE_SNOWADD:
        // case TerrainToolType::TERRAINTOOLTYPE_SNOWREMOVE:
        //     return memnew(SnowTool);
        // case TerrainToolType::TERRAINTOOLTYPE_HOLEADD:
        // case TerrainToolType::TERRAINTOOLTYPE_HOLEREMOVE:
        //     return memnew(HoleTool);
        // case TerrainToolType::TERRAINTOOLTYPE_LOCKADD:
        // case TerrainToolType::TERRAINTOOLTYPE_LOCKREMOVE:
        //     return memnew(LockTool);
        // case TerrainToolType::TERRAINTOOLTYPE_METAINFOADD:
        // case TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE:
        //     return memnew(MetaInfoTool);
    }

    return nullptr;
}