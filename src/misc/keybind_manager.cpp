#include "keybind_manager.h"

#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/os.hpp>

using namespace godot;

void KeybindManager::_bind_methods() {

}

TypedArray<Ref<InputEvent>> KeybindManager::getBinding(StringName actionName) {
    return InputMap::get_singleton()->action_get_events(actionName);
}

String KeybindManager::describeInputKey(Ref<InputEventKey> key) {
    String desc = "";
    if (key->is_ctrl_pressed()) desc += "Ctrl + ";
    if (key->is_alt_pressed()) desc += "Alt + ";
    desc += OS::get_singleton()->get_keycode_string(key->get_keycode());
    return desc;
}

const StringName KeybindManager::StringNames::ToolPie() { return StringName("terrabrush_tool_pie"); }
const StringName KeybindManager::StringNames::BrushPie() { return StringName("terrabrush_brush_pie"); }
const StringName KeybindManager::StringNames::ToolContentPie() { return StringName("terrabrush_tool_content_pie"); }
const StringName KeybindManager::StringNames::BrushSizeSelector() { return StringName("terrabrush_brush_size_selector"); }
const StringName KeybindManager::StringNames::BrushStrengthSelector() { return StringName("terrabrush_brush_strength_selector"); }
const StringName KeybindManager::StringNames::EscapeSelector() { return StringName("terrabrush_brush_escape_selector"); }
const StringName KeybindManager::StringNames::ToggleAutoAddZones() { return StringName("terrabrush_toggle_auto_add_zones"); }
const StringName KeybindManager::StringNames::LockXAxis() { return StringName("terrabrush_lock_x_axis"); }
const StringName KeybindManager::StringNames::LockZAxis() { return StringName("terrabrush_lock_z_axis"); }

KeybindManager::KeybindManager() {
    _toolPieKey = Ref<InputEventKey>(memnew(InputEventKey));
    _toolPieKey->set_keycode(Key::KEY_V);

    _brushPieKey = Ref<InputEventKey>(memnew(InputEventKey));
    _brushPieKey->set_keycode(Key::KEY_B);

    _toolContentKey = Ref<InputEventKey>(memnew(InputEventKey));
    _toolContentKey->set_keycode(Key::KEY_N);

    _brushSizeKey = Ref<InputEventKey>(memnew(InputEventKey));
    _brushSizeKey->set_keycode(Key::KEY_G);

    _brushStrengthKey = Ref<InputEventKey>(memnew(InputEventKey));
    _brushStrengthKey->set_keycode(Key::KEY_H);

    _escapeKey = Ref<InputEventKey>(memnew(InputEventKey));
    _escapeKey->set_keycode(Key::KEY_ESCAPE);

    _toggleAutoAddZonesKey = Ref<InputEventKey>(memnew(InputEventKey));
    _toggleAutoAddZonesKey->set_keycode(Key::KEY_K);

    _lockXAxis = Ref<InputEventKey>(memnew(InputEventKey));
    _lockXAxis->set_keycode(Key::KEY_X);

    _lockZAxis = Ref<InputEventKey>(memnew(InputEventKey));
    _lockZAxis->set_keycode(Key::KEY_Z);

    _actionNames = TypedArray<StringName>({
        StringNames::ToolPie(),
        StringNames::BrushPie(),
        StringNames::ToolContentPie(),
        StringNames::BrushSizeSelector(),
        StringNames::BrushStrengthSelector(),
        StringNames::EscapeSelector(),
        StringNames::ToggleAutoAddZones(),
        StringNames::LockXAxis(),
        StringNames::LockZAxis(),
    });
    _defaultKeys = TypedDictionary<StringName, InputEventKey>();
    _defaultKeys[StringNames::ToolPie()] = _toolPieKey;
    _defaultKeys[StringNames::BrushPie()] = _brushPieKey;
    _defaultKeys[StringNames::ToolContentPie()] = _toolContentKey;
    _defaultKeys[StringNames::BrushSizeSelector()] = _brushSizeKey;
    _defaultKeys[StringNames::BrushStrengthSelector()] = _brushStrengthKey;
    _defaultKeys[StringNames::EscapeSelector()] = _escapeKey;
    _defaultKeys[StringNames::ToggleAutoAddZones()] = _toggleAutoAddZonesKey;
    _defaultKeys[StringNames::LockXAxis()] = _lockXAxis;
    _defaultKeys[StringNames::LockZAxis()] = _lockZAxis;
}

KeybindManager::~KeybindManager() {}

TypedArray<StringName> KeybindManager::get_actionNames() const {
    return _actionNames;
}

TypedDictionary<StringName, Ref<InputEventKey>> KeybindManager::get_defaultKeys() const {
    return _defaultKeys;
}

/// <summary>
/// Register's the Action Names with the Godot Editor InputMap.
/// </summary>
void KeybindManager::registerInputMap(bool registerDefaultKey) {
    InputMap *im = InputMap::get_singleton();
    for (StringName action : _actionNames) {
        if (!im->has_action(action)) {
            im->add_action(action); // Register the Action, as we don't have the Action.
            if (registerDefaultKey) {
                im->action_add_event(action, _defaultKeys[action]);
            }
        }
    }
}

/// <summary>
/// Load Keybinds from Godot Editor's EditorSettings File, for association with the InputMap.Actions.
/// </summary>
void KeybindManager::loadEditorSettings() {
    Ref<EditorSettings> settings = EditorInterface::get_singleton()->get_editor_settings();
    InputMap *im = InputMap::get_singleton();
    TypedArray<Dictionary> shortcuts = settings->get_setting("shortcuts");
    for (StringName action : _actionNames) {
        String binding = String(SettingsGroup) + "/" + action;

        Dictionary keyEvents;

        for (Dictionary shortcut : shortcuts) {
            if (shortcut["name"] == binding) {
                keyEvents = shortcut;
                break;
            }
        }

        if (!keyEvents.is_empty()) {
            // Register Saved event in EditorSettings.
            TypedArray<Ref<InputEventKey>> keys = keyEvents["shortcuts"];
            for (Ref<InputEventKey> key : keys) {
                im->action_add_event(action, key);
            }
        }
        else {
            // Register Default Event
            im->action_add_event(action, _defaultKeys[action]);
        }
    }
}

void KeybindManager::saveEditorSettings() {
    Ref<EditorSettings> settings = EditorInterface::get_singleton()->get_editor_settings();
    InputMap *im = InputMap::get_singleton();
    TypedArray<Dictionary> shortcuts = settings->get_setting("shortcuts");
    for (StringName action : _actionNames) {
        String binding = String(SettingsGroup) + "/" + action;

        Dictionary keyEvents;

        for (Dictionary shortcut : shortcuts) {
            if (shortcut["name"] == binding) {
                keyEvents = shortcut;
                break;
            }
        }

        if (keyEvents.is_empty()) {
            TypedDictionary<String, Variant> keybind = TypedDictionary<String, Variant>();
            keybind["name"] = binding;
            keybind["shortcuts"] = im->action_get_events(action);
            shortcuts.append(keybind);
        }
        else {
            keyEvents["shortcuts"] = im->action_get_events(action);
        }
    }

    settings->set_setting("shortcuts", shortcuts);
}

void KeybindManager::updateKeybind(StringName action, Ref<InputEventKey> key) {
    InputMap *im = InputMap::get_singleton();
    im->action_erase_events(action);
    im->action_add_event(action, key);
}

void KeybindManager::resetKeybind(StringName action) {
    if (!_actionNames.has(action)) return;
    InputMap *im = InputMap::get_singleton();
    im->action_erase_events(action);
    im->action_add_event(action, _defaultKeys[action]);
}

String KeybindManager::describeKey(StringName action) {
    TypedArray<Ref<InputEvent>> key = InputMap::get_singleton()->action_get_events(action);
    return describeInputKey(key[0]);
}
