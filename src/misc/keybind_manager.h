#ifndef KEYBIND_MANAGER_H
#define KEYBIND_MANAGER_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class KeybindManager : public RefCounted {
    GDCLASS(KeybindManager, RefCounted);

private:
    static constexpr const char* SettingsGroup = "terrabrush";

    #pragma region InputMap Actions and associated Default Key Events
	Ref<InputEventKey> _toolPieKey = nullptr;
	Ref<InputEventKey> _brushPieKey = nullptr;
	Ref<InputEventKey> _toolContentKey = nullptr;
	Ref<InputEventKey> _brushSizeKey = nullptr;
	Ref<InputEventKey> _brushStrengthKey = nullptr;
	Ref<InputEventKey> _escapeKey = nullptr;
	Ref<InputEventKey> _toggleAutoAddZonesKey = nullptr;
	Ref<InputEventKey> _lockXAxis = nullptr;
	Ref<InputEventKey> _lockZAxis = nullptr;
    #pragma endregion

    TypedArray<StringName> _actionNames = TypedArray<StringName>();
    TypedDictionary<StringName, Ref<InputEventKey>> _defaultKeys = TypedDictionary<StringName, Ref<InputEventKey>>();

protected:
    static void _bind_methods();

public:
    #pragma region Public String Names
    class StringNames {
    public:
        static const StringName ToolPie();
        static const StringName BrushPie();
        static const StringName ToolContentPie();
        static const StringName BrushSizeSelector();
        static const StringName BrushStrengthSelector();
        static const StringName EscapeSelector();
        static const StringName ToggleAutoAddZones();
        static const StringName LockXAxis();
        static const StringName LockZAxis();
    };
    #pragma endregion

    static TypedArray<Ref<InputEvent>> getBinding(StringName actionName);
    static String describeInputKey(Ref<InputEventKey> key);

    KeybindManager();
    ~KeybindManager();

    TypedArray<StringName> get_actionNames() const;
    TypedDictionary<StringName, Ref<InputEventKey>> get_defaultKeys() const;

    void registerInputMap(bool registerDefaultKey = false);
    void loadEditorSettings();
    void saveEditorSettings();
    void updateKeybind(StringName action, Ref<InputEventKey> key);
    void resetKeybind(StringName action);
    String describeKey(StringName action);
};
#endif