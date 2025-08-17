#ifndef KEYBIND_MANAGER_H
#define KEYBIND_MANAGER_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class KeybindManager : public RefCounted {
    GDCLASS(KeybindManager, RefCounted)

private:
    static constexpr const char* SettingsGroup = "terrabrush";

    #pragma region InputMap Actions and associated Default Key Events
	Ref<InputEventKey> _toolPieKey;
	Ref<InputEventKey> _brushPieKey;
	Ref<InputEventKey> _toolContentKey;
	Ref<InputEventKey> _brushSizeKey;
	Ref<InputEventKey> _brushStrengthKey;
	Ref<InputEventKey> _escapeKey;
	Ref<InputEventKey> _toggleAutoAddZonesKey;
	Ref<InputEventKey> _lockXAxis;
	Ref<InputEventKey> _lockZAxis;
    #pragma endregion

    TypedArray<StringName> _actionNames;
    TypedDictionary<StringName, Ref<InputEventKey>> _defaultKeys;

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

    void registerInputMap();
    void loadEditorSettings();
    void saveEditorSettings();
    void updateKeybind(StringName action, Ref<InputEventKey> key);
    void resetKeybind(StringName action);
    String describeKey(StringName action);
};
#endif