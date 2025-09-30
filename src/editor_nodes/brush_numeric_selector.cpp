#include "brush_numeric_selector.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

using namespace godot;

void BrushNumericSelector::_bind_methods() {}

BrushNumericSelector::BrushNumericSelector() {
    _brushSizeFactor = 2;
    _widgetColor = Color::named("WHITE");
    _minValue = -1;
    _maxValue = 1;
}

BrushNumericSelector::~BrushNumericSelector() {}

void BrushNumericSelector::_ready() {
    _background = memnew(TextureRect);
    _background->set_expand_mode(TextureRect::ExpandMode::EXPAND_IGNORE_SIZE);
    _background->set_texture(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/white_circle.png"));
    _background->set_modulate(Color::html("#3a3a3a4b"));
    add_child(_background);

    _brushPreview = memnew(TextureRect);
    _brushPreview->set_expand_mode(TextureRect::ExpandMode::EXPAND_IGNORE_SIZE);
    _brushPreview->set_texture(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/brush_preview.png"));
    _brushPreview->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_CENTER);
    _brushPreview->set_modulate(_widgetColor);
    add_child(_brushPreview);

    _valueLabel = memnew(Label);
    _valueLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _valueLabel->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    _valueLabel->set_h_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    _valueLabel->set_v_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    _valueLabel->set((StringName)"theme_override_colors/font_color", Color::named("WHITE"));
    _valueLabel->set((StringName)"theme_override_colors/font_outline_color", Color::html("#00151f"));
    _valueLabel->set((StringName)"theme_override_constants/outline_size", 3);
    _valueLabel->set((StringName)"theme_override_font_sizes/font_size", 20);

    Ref<StyleBoxFlat> valueLabelNormalStyle = memnew(StyleBoxFlat);
    valueLabelNormalStyle->set_bg_color(Color::html("#00151f"));
    valueLabelNormalStyle->set_corner_radius_all(5);
    valueLabelNormalStyle->set_expand_margin_all(5);

    _valueLabel->set((StringName)"theme_override_styles/normal", valueLabelNormalStyle);
    add_child(_valueLabel);

    Color iconsColor = ProjectSettings::get_singleton()->get_setting(SettingContants::IconsColor());
    _valueLabel->set((StringName)"theme_override_colors/font_outline_color", iconsColor);
    Ref<StyleBoxFlat> normalStyle = _valueLabel->get((StringName)"theme_override_styles/normal");
    normalStyle->set_bg_color(iconsColor);
}

void BrushNumericSelector::_process(double delta) {
    updateValue(getMouseDistance());
}

void BrushNumericSelector::_gui_input(const Ref<InputEvent> &event) {
    Ref<InputEventMouseButton> inputButton = Object::cast_to<InputEventMouseButton>(*event);

    if (!inputButton.is_null()) {
        if (inputButton->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            auto distance = getMouseDistance();
            if (!_onValueSelected.is_null()) {
                _onValueSelected.call(distance);
            }
        } else if (!_onCancel.is_null()) {
            _onCancel.call();
        }
    }
}

void BrushNumericSelector::set_brushSizeFactor(const int value) {
    _brushSizeFactor = value;
}
void BrushNumericSelector::set_widgetColor(const Color value) {
    _widgetColor = value;
}
void BrushNumericSelector::set_minValue(const int value) {
    _minValue = value;
}
void BrushNumericSelector::set_maxValue(const int value) {
    _maxValue = value;
}

void BrushNumericSelector::set_onValueSelected(const Callable value) {
    _onValueSelected = value;
}
void BrushNumericSelector::set_onCancel(const Callable value) {
    _onCancel = value;
}

int BrushNumericSelector::getMouseDistance() {
    float distance = get_position().distance_to(get_global_mouse_position());

    if (_minValue >= 0) {
        distance = Math::max(distance, (float)_minValue);
    }

    if (_maxValue >= 0) {
        distance = Math::min(distance, (float)_maxValue);
    }

    return (int) distance;
}

void BrushNumericSelector::setInitialValue(float value) {
    set_position(get_position() - Vector2(value * _brushSizeFactor / 2, 0));
    updateValue(value);
}

void BrushNumericSelector::updateValue(float value) {
    auto size = value * _brushSizeFactor;
    _background->set_size(Vector2(size + BackgroundMargin, size + BackgroundMargin));
    _background->set_position(Vector2(-((size / 2) + BackgroundMargin / 2), -((size / 2) + BackgroundMargin / 2)));

    _brushPreview->set_size(Vector2(size, size));
    _brushPreview->set_position(Vector2(-(size / 2), -(size / 2)));

    _valueLabel->set_text(String::num(value, 0));
}

void BrushNumericSelector::requestSelectValue() {
    auto distance = getMouseDistance();
    if (!_onValueSelected.is_null()) {
        _onValueSelected.call(distance);
    }
}
