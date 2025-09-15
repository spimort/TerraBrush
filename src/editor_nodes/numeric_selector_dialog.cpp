#include "numeric_selector_dialog.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/line_edit.hpp>

using namespace godot;

void NumericSelectorDialog::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onOkButton"), &NumericSelectorDialog::onOkButton);
    ClassDB::bind_method(D_METHOD("onCancelButton"), &NumericSelectorDialog::onCancelButton);
}

NumericSelectorDialog::NumericSelectorDialog() {}

NumericSelectorDialog::~NumericSelectorDialog() {}

void NumericSelectorDialog::_unhandled_input(const Ref<InputEvent> &event) {
    if (Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
        Ref<InputEventKey> inputEvent = Object::cast_to<InputEventKey>(event.ptr());

        if (inputEvent->get_keycode() == Key::KEY_ENTER  || inputEvent->get_keycode() == Key::KEY_KP_ENTER) {
            onOkButton();
        }
    }
}

void NumericSelectorDialog::_ready() {
    set_title("Select a value");
    set_initial_position(WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
    set_size(Vector2i(300, 90));
    set_exclusive(true);
    set_flag(Window::Flags::FLAG_RESIZE_DISABLED, true);
    connect("close_requested", Callable(this, "onCancelButton"));

    VBoxContainer *vbox = memnew(VBoxContainer);
    vbox->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    add_child(vbox);

    MarginContainer *marginContainer = memnew(MarginContainer);
    marginContainer->set((StringName)"theme_override_constants/margin_left", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_top", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_right", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_bottom", 10);
    vbox->add_child(marginContainer);

    _spinBox = memnew(SpinBox);
    _spinBox->set_select_all_on_focus(true);
    _spinBox->set_min(_minValue);
    _spinBox->set_max(_maxValue);
    _spinBox->set_step(0.01);
    _spinBox->set_focus_mode(Control::FocusMode::FOCUS_ALL);
    marginContainer->add_child(_spinBox);

    HBoxContainer *hbox = memnew(HBoxContainer);
    hbox->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    vbox->add_child(hbox);

    Button *okButton = memnew(Button);
    okButton->set_text("   OK   ");
    okButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    okButton->connect("pressed", Callable(this, "onOkButton"));
    hbox->add_child(okButton);

    Button *cancelButton = memnew(Button);
    cancelButton->set_text("   Cancel   ");
    cancelButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
    cancelButton->connect("pressed", Callable(this, "onCancelButton"));
    hbox->add_child(cancelButton);

    _spinBox->get_line_edit()->grab_focus();
}

void NumericSelectorDialog::init(float minValue, float maxValue, std::function<void(float)> onAccept) {
    _minValue = minValue;
    _maxValue = maxValue;
    _onAccept = onAccept;
}

void NumericSelectorDialog::setValue(float value) {
    _spinBox->set_value(value);
}

void NumericSelectorDialog::sendValue() {
    float value = Utils::roundToDecimalPlace(_spinBox->get_value(), 2);
    _onAccept(value);
}

void NumericSelectorDialog::onOkButton() {
    sendValue();
    queue_free();
}

void NumericSelectorDialog::onCancelButton() {
    queue_free();
}

