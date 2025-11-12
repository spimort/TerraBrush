#include "terra_brush_editor.h"
#include "editor_nodes/brush_numeric_selector.h"
#include "editor_nodes/tools_pie_menu.h"
#include "editor_nodes/custom_content_pie_menu.h"

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

#include "misc/zone_utils.h"
#include "misc/custom_content_loader.h"
#include "misc/dialog_utils.h"
#include "misc/keybind_manager.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/sub_viewport_container.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void TerraBrushEditor::_bind_methods() {
    ADD_SIGNAL(MethodInfo("toolTypeSelected", PropertyInfo(Variant::INT, "toolType")));
    ADD_SIGNAL(MethodInfo("brushSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("brushSizeChanged", PropertyInfo(Variant::INT, "value")));
    ADD_SIGNAL(MethodInfo("brushStrengthChanged", PropertyInfo(Variant::FLOAT, "value")));
    ADD_SIGNAL(MethodInfo("textureSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("foliageSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("objectSelected", PropertyInfo(Variant::INT, "index")));
    ADD_SIGNAL(MethodInfo("metaInfoSelected", PropertyInfo(Variant::INT, "index")));

    ClassDB::bind_method(D_METHOD("onPieMenuBrushSelected", "customContentPieMenu"), &TerraBrushEditor::onPieMenuBrushSelected);
    ClassDB::bind_method(D_METHOD("onPieMenuBrushIndexSelected", "brushIndex"), &TerraBrushEditor::onPieMenuBrushIndexSelected);
    ClassDB::bind_method(D_METHOD("onBrushSizeSelected", "value"), &TerraBrushEditor::onBrushSizeSelected);
    ClassDB::bind_method(D_METHOD("onBrushStrengthSelected", "value"), &TerraBrushEditor::onBrushStrengthSelected);
    ClassDB::bind_method(D_METHOD("onUndoRedo"), &TerraBrushEditor::onUndoRedo);
    ClassDB::bind_method(D_METHOD("onToolSelected", "value"), &TerraBrushEditor::onToolSelected);
    ClassDB::bind_method(D_METHOD("hideOverlaySelector"), &TerraBrushEditor::hideOverlaySelector);

    ClassDB::bind_method(D_METHOD("onCustomContentSelectorTextureSelected", "index"), &TerraBrushEditor::onCustomContentSelectorTextureSelected);
    ClassDB::bind_method(D_METHOD("onCustomContentSelectorFoliageSelected", "index"), &TerraBrushEditor::onCustomContentSelectorFoliageSelected);
    ClassDB::bind_method(D_METHOD("onCustomContentSelectorObjectSelected", "index"), &TerraBrushEditor::onCustomContentSelectorObjectSelected);
    ClassDB::bind_method(D_METHOD("onCustomContentSelectorMetaInfoSelected", "index"), &TerraBrushEditor::onCustomContentSelectorMetaInfoSelected);

    ClassDB::bind_method(D_METHOD("get_enabled"), &TerraBrushEditor::get_enabled);
    ClassDB::bind_method(D_METHOD("set_enabled", "value"), &TerraBrushEditor::set_enabled);

    ClassDB::bind_method(D_METHOD("get_brushIndex"), &TerraBrushEditor::get_brushIndex);
    ClassDB::bind_method(D_METHOD("set_brushIndex", "value"), &TerraBrushEditor::set_brushIndex);

    ClassDB::bind_method(D_METHOD("get_brushSize"), &TerraBrushEditor::get_brushSize);
    ClassDB::bind_method(D_METHOD("set_brushSize", "value"), &TerraBrushEditor::set_brushSize);

    ClassDB::bind_method(D_METHOD("get_brushStrength"), &TerraBrushEditor::get_brushStrength);
    ClassDB::bind_method(D_METHOD("set_brushStrength", "value"), &TerraBrushEditor::set_brushStrength);

    ClassDB::bind_method(D_METHOD("get_selectedToolType"), &TerraBrushEditor::get_selectedToolType);
    ClassDB::bind_method(D_METHOD("set_selectedToolType", "value"), &TerraBrushEditor::set_selectedToolType);

    ClassDB::bind_method(D_METHOD("get_selectedTextureIndex"), &TerraBrushEditor::get_selectedTextureIndex);
    ClassDB::bind_method(D_METHOD("set_selectedTextureIndex", "value"), &TerraBrushEditor::set_selectedTextureIndex);

    ClassDB::bind_method(D_METHOD("get_selectedFoliageIndex"), &TerraBrushEditor::get_selectedFoliageIndex);
    ClassDB::bind_method(D_METHOD("set_selectedFoliageIndex", "value"), &TerraBrushEditor::set_selectedFoliageIndex);

    ClassDB::bind_method(D_METHOD("get_selectedObjectIndex"), &TerraBrushEditor::get_selectedObjectIndex);
    ClassDB::bind_method(D_METHOD("set_selectedObjectIndex", "value"), &TerraBrushEditor::set_selectedObjectIndex);

    ClassDB::bind_method(D_METHOD("get_selectedMetaInfoIndex"), &TerraBrushEditor::get_selectedMetaInfoIndex);
    ClassDB::bind_method(D_METHOD("set_selectedMetaInfoIndex", "value"), &TerraBrushEditor::set_selectedMetaInfoIndex);

    ClassDB::bind_method(D_METHOD("get_autoAddZones"), &TerraBrushEditor::get_autoAddZones);
    ClassDB::bind_method(D_METHOD("set_autoAddZones", "value"), &TerraBrushEditor::set_autoAddZones);

    ClassDB::bind_method(D_METHOD("get_terraBrushNode"), &TerraBrushEditor::get_terraBrushNode);
    ClassDB::bind_method(D_METHOD("set_terraBrushNode", "value"), &TerraBrushEditor::set_terraBrushNode);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "terraBrushNode", PROPERTY_HINT_NODE_TYPE, "TerraBrush"), "set_terraBrushNode", "get_terraBrushNode");

    ClassDB::bind_method(D_METHOD("get_enableOnReady"), &TerraBrushEditor::get_enableOnReady);
    ClassDB::bind_method(D_METHOD("set_enableOnReady", "value"), &TerraBrushEditor::set_enableOnReady);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enableOnReady"), "set_enableOnReady", "get_enableOnReady");

    ClassDB::bind_method(D_METHOD("get_allowBuiltInToolSelectors"), &TerraBrushEditor::get_allowBuiltInToolSelectors);
    ClassDB::bind_method(D_METHOD("set_allowBuiltInToolSelectors", "value"), &TerraBrushEditor::set_allowBuiltInToolSelectors);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allowBuiltInToolSelectors"), "set_allowBuiltInToolSelectors", "get_allowBuiltInToolSelectors");

    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_NONE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINSMOOTH);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINFLATTEN);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINSETHEIGHT);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_TERRAINSETANGLE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_PAINT);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_FOLIAGEADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_FOLIAGEREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_OBJECTADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_OBJECTREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_WATERADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_WATERREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_WATERFLOWADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_WATERFLOWREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_SNOWADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_SNOWREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_HOLEADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_HOLEREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_LOCKADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_LOCKREMOVE);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_METAINFOADD);
    BIND_ENUM_CONSTANT(TERRAINTOOLTYPE_METAINFOREMOVE);
}

TerraBrushEditor::TerraBrushEditor() {
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

TerraBrushEditor::~TerraBrushEditor() {}

void TerraBrushEditor::_ready() {
    set_brushIndex(0);

    if (!Engine::get_singleton()->is_editor_hint()) {
        Ref<KeybindManager> keybindManager = memnew(KeybindManager);
        keybindManager->registerInputMap(true);

        _containerNode = this;

        if (_enableOnReady) {
            set_enabled(true);
        }
    }
}

void TerraBrushEditor::_physics_process(double delta) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    if (!_isMousePressed) {
        _updateTime = 0;
    } else if (_updateTime > 0) {
        _updateTime -= (float) delta;
    } else if (_isMousePressed && _mouseHitPosition != Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue) && !_currentTool.is_null()) {
        int zoneSize = _terraBrushNode->get_zonesSize();
        Vector3 meshToImagePosition = _mouseHitPosition + Vector3(zoneSize / 2, 0, zoneSize / 2);
        Vector2 imagePosition = Vector2(meshToImagePosition.x, meshToImagePosition.z);

        _currentTool->paint(get_selectedToolType(), _brushImage, _brushSize, _brushStrength, imagePosition);

        _updateTime = UpdateDelay;
    }
}

void TerraBrushEditor::_unhandled_input(const Ref<InputEvent> &event) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    if (!Engine::get_singleton()->is_editor_hint()) {
        Camera3D *camera = get_viewport()->get_camera_3d();
        onGuiInput(camera, event);
    }
};

bool TerraBrushEditor::onGuiInput(Camera3D *camera, const Ref<InputEvent> &event) {
    if (!_enabled ||_terraBrushNode == nullptr || _containerNode == nullptr) {
        return false;
    }

    bool preventGuiInput = false;

    if (Object::cast_to<InputEventMouseMotion>(event.ptr()) != nullptr) {
        Ref<InputEventMouseMotion> inputMotion = Object::cast_to<InputEventMouseMotion>(event.ptr());
        Vector3 meshPosition = getRayCastWithTerrain(camera);
        if (meshPosition == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
            _brushDecal->set_visible(false);
        } else {
            _brushDecal->set_visible(true);
            _brushDecal->set_position(Vector3(meshPosition.x, meshPosition.y, meshPosition.z));
        }

        _mouseHitPosition = meshPosition - _terraBrushNode->get_global_position();
    }

    if (_allowBuiltInToolSelectors) {
        if (_toolInfo != nullptr) {
            Vector2 globalMousePosition = Vector2(0, 0);
            if (camera->get_viewport()->get_parent() != nullptr) {
                globalMousePosition =  Object::cast_to<Control>(camera->get_viewport()->get_parent())->get_global_position();
            }
            _toolInfo->set_position(camera->get_viewport()->get_mouse_position() + globalMousePosition + Vector2i(ToolInfoOffset, ToolInfoOffset));

            if (!_currentTool.is_null()) {
                _toolInfo->setText(_currentTool->getToolInfo(get_selectedToolType()));
            }
        }

        if (Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
            Ref<InputEventKey> inputEvent = Object::cast_to<InputEventKey>(event.ptr());

            if (inputEvent->get_keycode() == Key::KEY_SHIFT && !inputEvent->is_echo()) {
                if (inputEvent->is_pressed()) {
                    setShiftPressed(true);
                } else {
                    setShiftPressed(false);
                }
            }

            if (!inputEvent->is_pressed() || inputEvent->is_echo()) return false;

            if (inputEvent->is_action(KeybindManager::StringNames::ToolPie())) {
                showToolPieMenu(camera->get_viewport(), KeybindManager::StringNames::ToolPie());
                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::BrushPie())) {
                showCustomContentPieMenu(camera->get_viewport(), "Brushes", ([&](CustomContentPieMenu *customContentPieMenu) {
                    onPieMenuBrushSelected(customContentPieMenu);
                }));
                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::ToolContentPie())) {
                showCurrentToolMenu(camera->get_viewport());
                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::BrushSizeSelector())) {
                showBrushNumericSelector(camera->get_viewport(), 1, 200, Color::named("LIME_GREEN"), _brushSize, Callable(this, "onBrushSizeSelected"), KeybindManager::StringNames::BrushSizeSelector());

                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::BrushStrengthSelector())) {
                showBrushNumericSelector(camera->get_viewport(), 1, 100, Color::named("CRIMSON"), (int) (_brushStrength * 100), Callable(this, "onBrushStrengthSelected"), KeybindManager::StringNames::BrushStrengthSelector());

                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::EscapeSelector()) && _overlaySelector != nullptr) {
                hideOverlaySelector();
                return true;
            }

            if (inputEvent->is_action(KeybindManager::StringNames::ToggleAutoAddZones())) {
                _autoAddZones = !_autoAddZones;
                updateAutoAddZonesSetting();
                return true;
            }
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
            return true;
        }

        if (inputButton->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            if (inputButton->is_pressed()) {
                if (_mouseHitPosition != Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
                    if (_undoRedo != nullptr) {
                        _undoRedo->create_action("Modify terrain");

                        // Trigger a dirty state
                        _undoRedo->add_undo_property(_terraBrushNode, "zonesSize", _terraBrushNode->get_zonesSize());
                    }

                    _isMousePressed = true;
                    preventGuiInput = true;

                    if (!_currentTool.is_null()) {
                        _currentTool->beginPaint();
                    }
                }
            } else if (_isMousePressed) {
                _terraBrushNode->get_terrain()->terrainUpdated();
                _isMousePressed = false;

                // Trigger a dirty state
                if (_undoRedo != nullptr) {
                    _undoRedo->add_do_property(_terraBrushNode, "zonesSize", _terraBrushNode->get_zonesSize());
                }

                if (!_currentTool.is_null()) {
                    _currentTool->endPaint();
                }

                if (_undoRedo != nullptr) {
                    _undoRedo->add_undo_method(this, "onUndoRedo");
                    _undoRedo->add_do_method(this, "onUndoRedo");

                    _preventInitialDo = true;
                    _undoRedo->commit_action();
                    _preventInitialDo = false;
                }

            }
        }
    }

    if (preventGuiInput) {
        return true;
    } else {
        if (!_currentTool.is_null() && _currentTool->handleInput(get_selectedToolType(), event)) {
            return true;
        }

        return false;
    }
}

void TerraBrushEditor::onUndoRedo() {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr || _preventInitialDo) {
        return;
    }

    _terraBrushNode->get_terrain()->terrainUpdated();
    if (!_terraBrushNode->get_terrainZones().is_null()) {
        _terraBrushNode->get_terrainZones()->updateImageImages(_terraBrushNode->get_zonesSize());
    }

    _terraBrushNode->clearObjects();
    _terraBrushNode->createObjects();
}

Vector3 TerraBrushEditor::getRayCastWithTerrain(Camera3D *camera) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
    }

    PhysicsDirectSpaceState3D *spaceState = _terraBrushNode->get_world_3d()->get_direct_space_state();

    Vector2 screenPosition = camera->get_viewport()->get_mouse_position();

    Vector3 from = camera->project_ray_origin(screenPosition);
    Vector3 dir = camera->project_ray_normal(screenPosition);

    return getMouseClickToZoneHeight(from, dir);
}

Vector3 TerraBrushEditor::getMouseClickToZoneHeight(Vector3 from, Vector3 direction) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
    }

    for (int i = 0; i < 20000; i++) {
        Vector3 position = from + (direction * i * 0.1f) - _terraBrushNode->get_global_position();

        ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(position.x + (_terraBrushNode->get_zonesSize() / 2), position.z + (_terraBrushNode->get_zonesSize() / 2), _terraBrushNode->get_zonesSize(), _terraBrushNode->get_resolution());
        Ref<ZoneResource> zone;
        if (!_terraBrushNode->get_terrainZones().is_null()) {
            zone = _terraBrushNode->get_terrainZones()->getZoneForZoneInfo(zoneInfo);
        }

        if (!zone.is_null() && !zone->get_heightMapImage().is_null()) {
            Ref<Image> heightMapImage = zone->get_heightMapImage();
            float zoneHeight = heightMapImage->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y).r;

            if (zoneHeight >= position.y) {
                return Vector3(position.x, zoneHeight, position.z) + _terraBrushNode->get_global_position();
            }
        }

    }

    return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
}

StringName TerraBrushEditor::hideOverlaySelector() {
    if (_overlaySelector != nullptr) {
        StringName overlayActionName = _overlaySelector->get_meta(OverlayActionNameKey, "");

        _overlaySelector->queue_free();
        _overlaySelector = nullptr;

        return overlayActionName;
    }

    return "";
}

void TerraBrushEditor::showToolPieMenu(Viewport *viewport, StringName actionName) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != actionName && viewport != nullptr) {
        ToolsPieMenu *pieMenu = memnew(ToolsPieMenu);
        pieMenu->set_onToolSelected(Callable(this, "onToolSelected"));

        _overlaySelector = pieMenu;

        Vector2 mousePosition = viewport->get_mouse_position();
        if (viewport->get_parent() != nullptr) {
            mousePosition = Object::cast_to<Control>(viewport->get_parent())->get_global_mouse_position();
        }
        _overlaySelector->set_position(mousePosition);
        _overlaySelector->set_meta(OverlayActionNameKey, actionName);

        _containerNode->add_child(_overlaySelector);
    }
}

void TerraBrushEditor::showCustomContentPieMenu(Viewport *viewport, String label, std::function<void(CustomContentPieMenu*)> addItems) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != label && viewport != nullptr) {
        CustomContentPieMenu *customContentPieMenu = memnew(CustomContentPieMenu);

        _overlaySelector = customContentPieMenu;

        Vector2 mousePosition = viewport->get_mouse_position();
        if (viewport->get_parent() != nullptr) {
            mousePosition = Object::cast_to<Control>(viewport->get_parent())->get_global_mouse_position();
        }
        _overlaySelector->set_position(mousePosition);
        _overlaySelector->set_meta((StringName)OverlayActionNameKey, label);

        _containerNode->add_child(_overlaySelector);

        addItems(customContentPieMenu);

        customContentPieMenu->get_pieMenu()->set_label(label);
        customContentPieMenu->get_pieMenu()->updateContent();
    }
}

void TerraBrushEditor::showCurrentToolMenu(Viewport *viewport) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    switch (get_selectedToolType()) {
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETHEIGHT:
            DialogUtils::showNumericSelector(
                this,
                ([&](float value) {
                    _selectedSetHeight = value;
                    Ref<SetHeightTool> setHeightTool = Object::cast_to<SetHeightTool>(_currentTool.ptr());
                    setHeightTool->updateSetHeightValue(_selectedSetHeight);
                }),
                _selectedSetHeight
            );
            break;
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSETANGLE:
            DialogUtils::showNumericSelector(
                this,
                ([&](float value) {
                    _selectedSetAngle = value;
                    Ref<SetAngleTool> setAngleTool = Object::cast_to<SetAngleTool>(_currentTool.ptr());
                    setAngleTool->updateSetAngleValue(_selectedSetAngle);
                }),
                _selectedSetAngle
            );
            break;
        case TerrainToolType::TERRAINTOOLTYPE_PAINT:
            showCustomContentPieMenu(viewport, "Textures", ([&](CustomContentPieMenu *customContentPieMenu) {
                CustomContentLoader::addTexturesPreviewToParent(_terraBrushNode, customContentPieMenu->get_pieMenu(), Callable(this, "onCustomContentSelectorTextureSelected"), true);
            }));

            break;
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD:
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE:
            showCustomContentPieMenu(viewport, "Foliages", ([&](CustomContentPieMenu *customContentPieMenu) {
                CustomContentLoader::addFoliagesPreviewToParent(_terraBrushNode, customContentPieMenu->get_pieMenu(), Callable(this, "onCustomContentSelectorFoliageSelected"), true);
            }));

            break;
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTADD:
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE:
            showCustomContentPieMenu(viewport, "Objects", ([&](CustomContentPieMenu *customContentPieMenu) {
                CustomContentLoader::addObjectsPreviewToParent(_terraBrushNode, customContentPieMenu->get_pieMenu(), Callable(this, "onCustomContentSelectorObjectSelected"), true);
            }));

            break;
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOADD:
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE:
            showCustomContentPieMenu(viewport, "MetaInfo", ([&](CustomContentPieMenu *customContentPieMenu) {
                CustomContentLoader::addMetaInfoLayersPreviewToParent(_terraBrushNode, customContentPieMenu->get_pieMenu(), Callable(this, "onCustomContentSelectorMetaInfoSelected"), true);
            }));

            break;
        default:
            break;
    }
}

void TerraBrushEditor::showBrushNumericSelector(Viewport *viewport, int minVale, int maxValue, Color widgetColor, int initialValue, Callable onValueSelected, StringName actionName) {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    StringName previewActionName = hideOverlaySelector();

    if (previewActionName != actionName && viewport != nullptr) {
        BrushNumericSelector *selector = memnew(BrushNumericSelector);

        selector->set_minValue(minVale);
        selector->set_maxValue(maxValue);
        selector->set_widgetColor(widgetColor);

        _overlaySelector = selector;

        Vector2 mousePosition = viewport->get_mouse_position();
        if (viewport->get_parent() != nullptr) {
            mousePosition = Object::cast_to<Control>(viewport->get_parent())->get_global_mouse_position();
        }
        _overlaySelector->set_position(mousePosition);
        _overlaySelector->set_meta(OverlayActionNameKey, actionName);

        _containerNode->add_child(_overlaySelector);

        selector->setInitialValue(initialValue);

        selector->set_onValueSelected(onValueSelected);
        selector->set_onCancel(Callable(this, "hideOverlaySelector"));
    }
}

void TerraBrushEditor::onPieMenuBrushSelected(const CustomContentPieMenu *customContentPieMenu) {
    CustomContentLoader::addBrushesPreviewToParent(customContentPieMenu->get_pieMenu(), Callable(this, "onPieMenuBrushIndexSelected"), true);
}

void TerraBrushEditor::onPieMenuBrushIndexSelected(const int brushIndex) {
    set_brushIndex(brushIndex);
    emit_signal("brushSelected", brushIndex);

    hideOverlaySelector();
}

void TerraBrushEditor::onBrushSizeSelected(const int value) {
    set_brushSize(value);
    emit_signal("brushSizeChanged", value);

    hideOverlaySelector();
}

void TerraBrushEditor::onBrushStrengthSelected(const int value) {
    set_brushStrength(value / 100.0f);
    emit_signal("brushStrengthChanged", value / 100.0f);

    hideOverlaySelector();
}

void TerraBrushEditor::onToolSelected(const TerrainToolType value) {
    set_selectedToolType(value);
    emit_signal("toolTypeSelected", value);

    hideOverlaySelector();
}

void TerraBrushEditor::onCustomContentSelectorTextureSelected(const int index) {
    set_selectedTextureIndex(index);
    emit_signal("textureSelected", index);

    hideOverlaySelector();
}

void TerraBrushEditor::onCustomContentSelectorFoliageSelected(const int index) {
    set_selectedFoliageIndex(index);
    emit_signal("foliageSelected", index);

    hideOverlaySelector();
}

void TerraBrushEditor::onCustomContentSelectorObjectSelected(const int index) {
    set_selectedObjectIndex(index);
    emit_signal("objectSelected", index);

    hideOverlaySelector();
}

void TerraBrushEditor::onCustomContentSelectorMetaInfoSelected(const int index) {
    set_selectedMetaInfoIndex(index);
    emit_signal("metaInfoSelected", index);

    hideOverlaySelector();
}

void TerraBrushEditor::updateCurrentTool() {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    TerrainToolType toolType = get_selectedToolType();

    Ref<ToolBase> newTool = getToolForType(toolType);
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

        _currentTool->init(_terraBrushNode, _undoRedo, _autoAddZones);
    }
}

void TerraBrushEditor::beforeDeselectTool() {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

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

Ref<ToolBase> TerraBrushEditor::getToolForType(TerrainToolType toolType) {
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
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD:
        case TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE: {
            Ref<FoliageTool> foliageTool = memnew(FoliageTool);
            foliageTool->updateSelectedFoliageIndex(_foliageIndex);
            return foliageTool;
        }
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTADD:
        case TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE: {
            Ref<ObjectTool> objectTool = memnew(ObjectTool);
            objectTool->updateSelectedObjectIndex(_objectIndex);
            return objectTool;
        }
        case TerrainToolType::TERRAINTOOLTYPE_WATERADD:
        case TerrainToolType::TERRAINTOOLTYPE_WATERREMOVE:
            return memnew(WaterTool);
        case TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD:
        case TerrainToolType::TERRAINTOOLTYPE_WATERFLOWREMOVE:
            return memnew(WaterFlowTool);
        case TerrainToolType::TERRAINTOOLTYPE_SNOWADD:
        case TerrainToolType::TERRAINTOOLTYPE_SNOWREMOVE:
            return memnew(SnowTool);
        case TerrainToolType::TERRAINTOOLTYPE_HOLEADD:
        case TerrainToolType::TERRAINTOOLTYPE_HOLEREMOVE:
            return memnew(HoleTool);
        case TerrainToolType::TERRAINTOOLTYPE_LOCKADD:
        case TerrainToolType::TERRAINTOOLTYPE_LOCKREMOVE:
            return memnew(LockTool);
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOADD:
        case TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE: {
            Ref<MetaInfoTool> metaInfoTool = memnew(MetaInfoTool);
            metaInfoTool->updateSelectedMetaInfoIndex(_metaInfoLayerIndex);
            return metaInfoTool;
        }
        case TerrainToolType::TERRAINTOOLTYPE_NONE: {
            return nullptr;
        }
    }

    return nullptr;
}

void TerraBrushEditor::updateAutoAddZonesSetting() {
    if (!_enabled || _terraBrushNode == nullptr || _containerNode == nullptr) {
        return;
    }

    if (!_currentTool.is_null()) {
        _currentTool->set_autoAddZones(_autoAddZones);
    }
}

void TerraBrushEditor::updateBrushImage() {
    if (!_originalBrushImage.is_null()) {
        _brushImage = Ref<Image>(memnew(Image));
        _brushImage->copy_from(_originalBrushImage);
        _brushImage->resize(_brushSize, _brushSize);

        if (_brushDecal != nullptr && !_brushImage.is_null()) {
            _brushDecal->setBrushImage(_brushImage);
        }
    }
}

void TerraBrushEditor::setShiftPressed(bool pressed) {
    if (pressed) {
        if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD || _currentToolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE || _currentToolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_OBJECTADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_WATERADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_WATERREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_WATERFLOWREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_SNOWADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_SNOWREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_HOLEADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_HOLEREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_LOCKADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_LOCKREMOVE;
        }
        else if (_currentToolType == TerrainToolType::TERRAINTOOLTYPE_METAINFOADD) {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_METAINFOREMOVE;
        }
        else {
            _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_NONE;
        }
    } else {
        _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_NONE;
    }

    TerrainToolType toolType = get_selectedToolType();
    updateCurrentTool();
    emit_signal("toolTypeSelected", toolType);
}

void TerraBrushEditor::set_containerNode(Node *containerNode) {
    _containerNode = containerNode;
}

TerraBrush *TerraBrushEditor::get_terraBrushNode() const {
    return _terraBrushNode;
}
void TerraBrushEditor::set_terraBrushNode(const TerraBrush *terraBrushNode) {
    if (_originalBrushImage.is_null()) {
        set_brushIndex(0);
    }

    _terraBrushNode = const_cast<TerraBrush*>(terraBrushNode);
}

void TerraBrushEditor::set_undoRedo(EditorUndoRedoManager *undoRedo) {
    _undoRedo = undoRedo;
}

bool TerraBrushEditor::get_enabled() const {
    return _enabled;
}

void TerraBrushEditor::set_enabled(const bool value) {
    _enabled = value;

    if (_brushDecal != nullptr) {
        _brushDecal->queue_free();
        _brushDecal = nullptr;
    }

    if (_toolInfo != nullptr) {
        _toolInfo->queue_free();
        _toolInfo = nullptr;
    }

    _currentTool = Ref<ToolBase>(nullptr);

    if (value) {
        _brushDecal = memnew(BrushDecal);
        _brushDecal->set_name("BrushDecal");
        add_child(_brushDecal);

        _brushDecal->setSize(_brushSize);
        if (!_brushImage.is_null()) {
            _brushDecal->setBrushImage(_brushImage);
        }

        _toolInfo = memnew(ToolInfo);
        _toolInfo->set_name("ToolInfo");
        add_child(_toolInfo);

        updateCurrentTool();

        if (!_terraBrushNode->get_textureSets().is_null() && _terraBrushNode->get_textureSets()->get_textureSets().size() > 0 && _textureIndex < 0) {
            _textureIndex = 0;
        }

        if (_terraBrushNode->get_foliages().size() > 0 && _foliageIndex < 0) {
            _foliageIndex = 0;
        }

        if (_terraBrushNode->get_objects().size() > 0 && _objectIndex < 0) {
            _objectIndex = 0;
        }

        if (_terraBrushNode->get_metaInfoLayers().size() > 0 && _metaInfoLayerIndex < 0) {
            _metaInfoLayerIndex = 0;
        }
    }
}

bool TerraBrushEditor::get_enableOnReady() const {
    return _enableOnReady;
}
void TerraBrushEditor::set_enableOnReady(const bool value) {
    _enableOnReady = value;
}

bool TerraBrushEditor::get_allowBuiltInToolSelectors() const {
    return _allowBuiltInToolSelectors;
}
void TerraBrushEditor::set_allowBuiltInToolSelectors(const bool value) {
    _allowBuiltInToolSelectors = value;
}

int TerraBrushEditor::get_brushIndex() const {
    return _brushIndex;
}
void TerraBrushEditor::set_brushIndex(const int value) {
    _brushIndex = value;
    Ref<Image> newBrushImage = Utils::getBrushImageForIndex(value);

    if (!newBrushImage.is_null()) {
        _originalBrushImage = newBrushImage;
    }

    updateBrushImage();
}

int TerraBrushEditor::get_brushSize() const {
    return _brushSize;
}
void TerraBrushEditor::set_brushSize(const int value) {
    _brushSize = value;
    _brushDecal->setSize(value);

    updateBrushImage();
}

float TerraBrushEditor::get_brushStrength() const {
    return _brushStrength;
}
void TerraBrushEditor::set_brushStrength(const float value) {
    _brushStrength = value;
}

TerrainToolType TerraBrushEditor::get_selectedToolType() const {
    return _temporaryTool == TerrainToolType::TERRAINTOOLTYPE_NONE ? _currentToolType : _temporaryTool;
;
}
void TerraBrushEditor::set_selectedToolType(const TerrainToolType value) {
    if (_temporaryTool == TerrainToolType::TERRAINTOOLTYPE_NONE) {
        _currentToolType = value;

        updateCurrentTool();
    }
}

int TerraBrushEditor::get_selectedTextureIndex() const {
    return _textureIndex;
}
void TerraBrushEditor::set_selectedTextureIndex(const int value) {
    _textureIndex = value;

    if (!_currentTool.is_null() && Object::cast_to<TextureTool>(_currentTool.ptr()) != nullptr) {
        Ref<TextureTool> textureTool = Object::cast_to<TextureTool>(_currentTool.ptr());
        textureTool->updateSelectedTextureIndex(value);
    }
}

int TerraBrushEditor::get_selectedFoliageIndex() const {
    return _foliageIndex;
}
void TerraBrushEditor::set_selectedFoliageIndex(const int value) {
    _foliageIndex = value;

    if (!_currentTool.is_null() && Object::cast_to<FoliageTool>(_currentTool.ptr()) != nullptr) {
        Ref<FoliageTool> foliageTool = Object::cast_to<FoliageTool>(_currentTool.ptr());
        foliageTool->updateSelectedFoliageIndex(value);
    }
}

int TerraBrushEditor::get_selectedObjectIndex() const {
    return _objectIndex;
}
void TerraBrushEditor::set_selectedObjectIndex(const int value) {
    if (!_currentTool.is_null() && Object::cast_to<ObjectTool>(_currentTool.ptr()) != nullptr) {
        Ref<ObjectTool> objectTool = Object::cast_to<ObjectTool>(_currentTool.ptr());
        objectTool->updateSelectedObjectIndex(value);
    }
}

int TerraBrushEditor::get_selectedMetaInfoIndex() const {
    return _metaInfoLayerIndex;
}
void TerraBrushEditor::set_selectedMetaInfoIndex(const int value) {
    _metaInfoLayerIndex = value;

    if (!_currentTool.is_null() && Object::cast_to<MetaInfoTool>(_currentTool.ptr()) != nullptr) {
        Ref<MetaInfoTool> metaInfoTool = Object::cast_to<MetaInfoTool>(_currentTool.ptr());
        metaInfoTool->updateSelectedMetaInfoIndex(value);
    }
}

bool TerraBrushEditor::get_autoAddZones() const {
    return _autoAddZones;
}
void TerraBrushEditor::set_autoAddZones(const bool value) {
    _autoAddZones = value;
    updateAutoAddZonesSetting();
}
