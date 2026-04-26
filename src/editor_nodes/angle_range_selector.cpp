#include "angle_range_selector.h"
#include "../misc/setting_contants.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

using namespace godot;

void AngleRangeSelector::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_rangeValue"), &AngleRangeSelector::get_rangeValue);
    ClassDB::bind_method(D_METHOD("set_rangeValue", "value"), &AngleRangeSelector::set_rangeValue);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "value"), "set_rangeValue", "get_rangeValue");

    ADD_SIGNAL(MethodInfo("rangeValueChanged", PropertyInfo(Variant::VECTOR2, "rangeValue")));
}

void AngleRangeSelector::_notification(const int what) {
    if (what == NOTIFICATION_DRAW) {
        draw();
    }
}

AngleRangeSelector::AngleRangeSelector() {
    set_max(90.0);
    set_custom_minimum_size(Vector2(0, HandleHeight + (2 * LabelHeight)));
}

AngleRangeSelector::~AngleRangeSelector() {}

void AngleRangeSelector::_ready() {
    Vector2 size = get_size();
    float center = (size.y / 2.0) - (LabelHeight / 2.0);

    _lowValueLabel = memnew(Label);
    _lowValueLabel->set_custom_minimum_size(Vector2(LabelWidth, LabelHeight));
    _lowValueLabel->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_CENTER);
    _lowValueLabel->set_position(Vector2(-LabelHorizontalOffset, _lowValueLabel->get_position().y - LabelHeight));
    _lowValueLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _lowValueLabel->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    _lowValueLabel->set("theme_override_font_sizes/font_size", 10);
    add_child(_lowValueLabel);

    _highValueLabel = memnew(Label);
    _highValueLabel->set_custom_minimum_size(Vector2(LabelWidth, LabelHeight));
    _highValueLabel->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_CENTER);
    _highValueLabel->set_position(Vector2(-LabelHorizontalOffset, _highValueLabel->get_position().y + LabelHeight));
    _highValueLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _highValueLabel->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    _highValueLabel->set("theme_override_font_sizes/font_size", 10);
    add_child(_highValueLabel);

    updateLabels();
}

void AngleRangeSelector::_gui_input(const Ref<InputEvent> &event) {
    Ref<InputEventMouseButton> inputButton = Object::cast_to<InputEventMouseButton>(*event);
    if (!inputButton.is_null() && inputButton->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
        if (inputButton->is_pressed()) {
            setGrabbedHandle(getHandleForMousePosition(inputButton->get_position()));
        } else {
            setGrabbedHandle(AngleRangeHandle::None);
        }
    }

    Ref<InputEventMouseMotion> inputMouseMotion = Object::cast_to<InputEventMouseMotion>(*event);
    if (!inputMouseMotion.is_null()) {
        if (_grabbedHandle != AngleRangeHandle::None) {
            float newValue = (inputMouseMotion->get_position().x / get_size().x) * get_max();
            if (_grabbedHandle == AngleRangeHandle::Left) {
                set_rangeValue(Vector2(newValue, _rangeValue.y));
            } else {
                set_rangeValue(Vector2(_rangeValue.x, newValue));
            }
            emit_signal("rangeValueChanged", _rangeValue);

            setHoverHandle(AngleRangeHandle::None);
        } else {
            setHoverHandle(getHandleForMousePosition(inputMouseMotion->get_position()));
        }
    }
}

Vector2 AngleRangeSelector::get_rangeValue() const {
    return _rangeValue;
}
void AngleRangeSelector::set_rangeValue(const Vector2 value) {
    _rangeValue = Vector2(Math::min(Math::max(value.x, (float) get_min()), (float) get_max()), Math::max(Math::min(value.y, (float) get_max()), (float) get_min()));

    if (_rangeValue.x > _rangeValue.y) {
        _rangeValue = Vector2(_rangeValue.y, _rangeValue.x);
    }

    queue_redraw();
}

void AngleRangeSelector::draw() {
    Color iconsColor = ProjectSettings::get_singleton()->get_setting(SettingContants::IconsColor(), SettingContants::IconsColorDefaultValue());

    Vector2 size = get_size();
    float centerY = (size.y / 2.0);
    float lowValuePosition = getLowValuePosition();
    float highValuePosition = getHighValuePosition();

    // Background bar
    Color backgroundColor = Color(iconsColor);
    backgroundColor.a = 0.25;
    Ref<StyleBoxFlat> backgroundStyleBox = memnew(StyleBoxFlat);
    backgroundStyleBox->set_bg_color(Color(backgroundColor));
    backgroundStyleBox->set_corner_radius_all(RectRadius);
    draw_style_box(backgroundStyleBox, Rect2(Vector2(0, centerY - RectHeight / 2.0), Size2(size.x, RectHeight)));

    // Value bar
    Ref<StyleBoxFlat> valueStyleBox = memnew(StyleBoxFlat);
    valueStyleBox->set_bg_color(iconsColor);
    valueStyleBox->set_corner_radius_all(RectRadius);
    draw_style_box(valueStyleBox, Rect2(Vector2(lowValuePosition, centerY - RectHeight / 2.0), Size2(highValuePosition - lowValuePosition, RectHeight)));

    // Handles
    AngleRangeHandle currentHandle = _grabbedHandle == AngleRangeHandle::None ? _hoverHandle : _grabbedHandle;

    float handleSize = HandleHeight / 2.0;
    Color handleColor = Color::named("WHITE");
    Color handleOutlineColor = Color(handleColor);
    handleOutlineColor.a = 0.25;
    draw_circle(Vector2(lowValuePosition, centerY), handleSize / 2.0, handleColor);
    draw_circle(Vector2(lowValuePosition, centerY), handleSize, currentHandle == AngleRangeHandle::Left ? handleColor : handleOutlineColor, false, handleSize / 4.0);
    draw_circle(Vector2(highValuePosition, centerY), handleSize / 2.0, handleColor);
    draw_circle(Vector2(highValuePosition, centerY), handleSize, currentHandle == AngleRangeHandle::Right ? handleColor : handleOutlineColor, false, handleSize / 4.0);

    updateLabels();
}

AngleRangeSelector::AngleRangeHandle AngleRangeSelector::getHandleForMousePosition(Vector2 position) const {
    Vector2 size = get_size();
    float lowValuePosition = getLowValuePosition();
    float highValuePosition = getHighValuePosition();

    if (position.x >= lowValuePosition - HandleHeight && position.x <= lowValuePosition + HandleHeight) {
        return AngleRangeHandle::Left;
    } else if (position.x >= highValuePosition - HandleHeight && position.x <= highValuePosition + HandleHeight) {
        return AngleRangeHandle::Right;
    }

    return AngleRangeHandle::None;
}

void AngleRangeSelector::setGrabbedHandle(AngleRangeSelector::AngleRangeHandle handle) {
    if (handle != _grabbedHandle) {
        _grabbedHandle = handle;
        queue_redraw();
    }
}

void AngleRangeSelector::setHoverHandle(AngleRangeSelector::AngleRangeHandle handle) {
    if (handle != _hoverHandle) {
        _hoverHandle = handle;
        queue_redraw();
    }
}

float AngleRangeSelector::getLowValuePosition() const {
    return (_rangeValue.x / get_max()) * get_size().x;
}

float AngleRangeSelector::getHighValuePosition() const {
    return (_rangeValue.y / get_max()) * get_size().x;
}

void AngleRangeSelector::updateLabels() {
    _lowValueLabel->set_text(String::num(_rangeValue.x, 2) + String::utf8("°"));
    _lowValueLabel->set_position(Vector2(getLowValuePosition() - LabelHorizontalOffset, _lowValueLabel->get_position().y));

    _highValueLabel->set_text(String::num(_rangeValue.y, 2) + String::utf8("°"));
    _highValueLabel->set_position(Vector2(getHighValuePosition() - LabelHorizontalOffset, _highValueLabel->get_position().y));
}