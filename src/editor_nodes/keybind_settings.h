#ifndef KEYBIND_SETTINGS_H
#define KEYBIND_SETTINGS_H

#include "../misc/keybind_manager.h"

#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/tree_item.hpp>

using namespace godot;

class KeybindSettings : public AcceptDialog {
    GDCLASS(KeybindSettings, AcceptDialog);

    enum ShortcutType {
        SHORTCUTTYPE_ADD = 1,
        SHORTCUTTYPE_ERASE  = 2
    };

private:
    Ref<KeybindManager> _keybindManager = nullptr;

    String properCase(StringName name);
    void keybindListOnButtonClicked(const TreeItem *item, const int column, const int id, const int mouseButtonIndex);
    void onConfirm();

protected:
    static void _bind_methods();

public:
    KeybindSettings();
    ~KeybindSettings();

    void _ready() override;
};
#endif