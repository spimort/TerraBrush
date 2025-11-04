#include "keybind_settings.h"
#include "key_listen_dialog.h"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void KeybindSettings::_bind_methods() {
    ClassDB::bind_method(D_METHOD("keybindListOnButtonClicked", "item", "column", "id", "mouseButtonIndex"), &KeybindSettings::keybindListOnButtonClicked);
    ClassDB::bind_method(D_METHOD("onConfirm"), &KeybindSettings::onConfirm);
}

KeybindSettings::KeybindSettings() {}

KeybindSettings::~KeybindSettings() {}

void KeybindSettings::_ready() {
    set_ok_button_text("Close");
    set_title("TerraBrush Keybindings");
    set_initial_position(WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
    set_size(Vector2(400, 300));
    set_visible(true);
    connect("confirmed", Callable(this, "onConfirm"));

    VBoxContainer *vboxContainer = memnew(VBoxContainer);
    vboxContainer->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    add_child(vboxContainer);

    Tree *tree = memnew(Tree);
    tree->set_columns(2);
    tree->set_column_titles_visible(true);
    tree->set_hide_folding(true);
    tree->set_hide_root(true);
    tree->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
    vboxContainer->add_child(tree);

    Ref<Theme> iconTheme = EditorInterface::get_singleton()->get_editor_theme();

    _keybindManager = Ref<KeybindManager>(memnew(KeybindManager));

    TreeItem *root = tree->create_item();
    tree->set_column_title(0,"Name");
    tree->set_column_title(1, "Binding");
    tree->set_hide_root(true);
    for (String action : _keybindManager->get_actionNames()) {
        TreeItem *iter = tree->create_item(root);
        iter->set_text(0, properCase(action));
        iter->set_text(1, _keybindManager->describeKey(action));

        iter->add_button(1, iconTheme->get_icon("Edit", "EditorIcons"), (int)ShortcutType::SHORTCUTTYPE_ADD);
        iter->add_button(1, iconTheme->get_icon("Close", "EditorIcons"), (int)ShortcutType::SHORTCUTTYPE_ERASE);
        iter->set_metadata(0, action);
    }
    tree->connect("button_clicked", Callable(this, "keybindListOnButtonClicked"));
}

String KeybindSettings::properCase(StringName name) {
    PackedStringArray parts = name.split("_");
    String newString = "";
    for (String part : parts) {
        if (part != "terrabrush") {
            newString += part.substr(0, 1).to_upper() + part.substr(1, part.length() - 1);
            newString += " ";
        }
    }

    return newString;
}

void KeybindSettings::keybindListOnButtonClicked(const TreeItem *item, const int column, const int id, const int mouseButtonIndex) {
    if (column != 1) return;
    if ((MouseButton)mouseButtonIndex != MouseButton::MOUSE_BUTTON_LEFT) return;

    switch ((ShortcutType)id) {
        case ShortcutType::SHORTCUTTYPE_ADD: {
            KeyListenDialog *dlg = memnew(KeyListenDialog);
            dlg->init(([dlg, item, &_keybindManager = _keybindManager](Ref<InputEventKey> key) {
                String action = item->get_metadata(0);
                const_cast<TreeItem*>(item)->set_text(1, KeybindManager::describeInputKey(key));
                _keybindManager->updateKeybind(action, key);
                dlg->queue_free();
            }));

            get_tree()->get_root()->add_child(dlg);
            dlg->popup_centered();
            break;
        }
        case ShortcutType::SHORTCUTTYPE_ERASE:
            String action = item->get_metadata(0);
            _keybindManager->resetKeybind(action);
            const_cast<TreeItem*>(item)->set_text(1, _keybindManager->describeKey(action));
            break;
    }
}

void KeybindSettings::onConfirm() {
    _keybindManager->saveEditorSettings();
    queue_free();
}