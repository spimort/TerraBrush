#include "key_listen_dialog.h"
#include "../misc/keybind_manager.h"

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/label_settings.hpp>
#include <godot_cpp/classes/h_separator.hpp>
#include <godot_cpp/classes/style_box_empty.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

using namespace godot;

void KeyListenDialog::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onOkButton"), &KeyListenDialog::onOkButton);
    ClassDB::bind_method(D_METHOD("onCancelButton"), &KeyListenDialog::onCancelButton);
}

KeyListenDialog::KeyListenDialog() {}

KeyListenDialog::~KeyListenDialog() {}

void KeyListenDialog::init(std::function<void(Ref<InputEventKey>)> onAccept) {
    _onAccept = onAccept;
}

void KeyListenDialog::_ready() {
    set_title("Press a Key");
    set_initial_position(WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
    set_size(Vector2(300, 100));

    VBoxContainer *vboxContainer = memnew(VBoxContainer);
    vboxContainer->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    add_child(vboxContainer);

    Label *label = memnew(Label);
    label->set_text("Press a Key to Assign to Shortcut");
    label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    vboxContainer->add_child(label);

    Ref<LabelSettings> keyDisplayLabelSettings = memnew(LabelSettings);
    keyDisplayLabelSettings->set_font_size(14);
    keyDisplayLabelSettings->set_font_color(Color::html("#d0d0d0"));

    _keyDisplay = memnew(Label);
    _keyDisplay->set_text("Waiting for Key...");
    _keyDisplay->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _keyDisplay->set_label_settings(keyDisplayLabelSettings);
    vboxContainer->add_child(_keyDisplay);

    HSeparator *hSeparator = memnew(HSeparator);
    hSeparator->set("theme_override_constants/separation", 10);
    hSeparator->set("theme_override_styles/separator", memnew(StyleBoxEmpty));
    vboxContainer->add_child(hSeparator);

    HBoxContainer *hboxContainer = memnew(HBoxContainer);
    hboxContainer->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    vboxContainer->add_child(hboxContainer);

    _okButton = memnew(Button);
    _okButton->set_text("   OK   ");
    _okButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    _okButton->set_disabled(true);
    _okButton->connect("pressed", Callable(this, "onOkButton"));
    hboxContainer->add_child(_okButton);

    Button *cancelButton = memnew(Button);
    cancelButton->set_text("   Cancel   ");
    cancelButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    cancelButton->connect("pressed", Callable(this, "onCancelButton"));
    hboxContainer->add_child(cancelButton);
}

void KeyListenDialog::_input(const Ref<InputEvent> &event) {
    if (Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
        Ref<InputEventKey> iek = Object::cast_to<InputEventKey>(event.ptr());
        _eventKey = iek;
        _okButton->set_disabled(false);
        _keyDisplay->set_text(KeybindManager::describeInputKey(iek));
    }
}

void KeyListenDialog::onOkButton() {
    if (_eventKey.is_null()) {
        _okButton->set_disabled(true);
        return;
    }

    _onAccept(_eventKey);
}

void KeyListenDialog::onCancelButton() {
    queue_free();
}