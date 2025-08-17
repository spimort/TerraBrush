#include "pie_menu.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/style_box_empty.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

using namespace godot;

void PieMenu::_bind_methods() {

}

PieMenu::PieMenu() {
    _buttonsRadiusRatio = 10;
    _buttonsMinRadius = 150;
}

PieMenu::~PieMenu() {}

void PieMenu::_ready() {
    set_anchors_and_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);

    _controlsContainer = memnew(Control);
    _controlsContainer->set_mouse_filter(MouseFilter::MOUSE_FILTER_PASS);
    _controlsContainer->set_anchors_and_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);
    _controlsContainer->set_offset(Side::SIDE_BOTTOM, 40.0);
    add_child(_controlsContainer);

    _pieBackground = memnew(TextureRect);
    _pieBackground->set_texture(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/white_circle.png"));
    _pieBackground->set_expand_mode(TextureRect::ExpandMode::EXPAND_IGNORE_SIZE);
    _pieBackground->set_modulate(Color::html("#3a3a3a4b"));
    _pieBackground->set_anchors_and_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);
    _pieBackground->set_offset(Side::SIDE_RIGHT, 496);
    _pieBackground->set_offset(Side::SIDE_BOTTOM, 496);
    _controlsContainer->add_child(_pieBackground);

    _menuLabel = memnew(Label);
    _menuLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _menuLabel->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    _menuLabel->set_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);
    _menuLabel->set_offset(Side::SIDE_LEFT, -75.0);
    _menuLabel->set_offset(Side::SIDE_TOP, 30.0);
    _menuLabel->set_offset(Side::SIDE_RIGHT, 75.0);
    _menuLabel->set_offset(Side::SIDE_BOTTOM, 58.0);
    _menuLabel->set_autowrap_mode(TextServer::AUTOWRAP_WORD);
    _menuLabel->set_custom_minimum_size(Vector2(100, 20));
    _menuLabel->set((StringName)"theme_override_colors/font_color", Color::named("WHITE"));
    _menuLabel->set((StringName)"theme_override_colors/font_outline_color", Color::html("#00151f"));
    _menuLabel->set((StringName)"theme_override_constants/outline_size", 3);
    _menuLabel->set((StringName)"theme_override_font_sizes/font_size", 20);

    Color iconsColor = ProjectSettings::get_singleton()->get_setting(SettingContants::IconsColor());
    Ref<StyleBoxFlat> normalStyle = memnew(StyleBoxFlat);
    normalStyle->set_bg_color(iconsColor);
    normalStyle->set_corner_radius_all(5);
    normalStyle->set_expand_margin_all(5);
    _menuLabel->set((StringName)"theme_override_styles/normal", normalStyle);
    _menuLabel->set((StringName) "theme_override_colors/font_outline_color", iconsColor);
    _controlsContainer->add_child(_menuLabel);

    _currentAnglePanel = memnew(Panel);
    _currentAnglePanel->set_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);
    _currentAnglePanel->set_offset(Side::SIDE_LEFT, -20);
    _currentAnglePanel->set_offset(Side::SIDE_TOP, -20);
    _currentAnglePanel->set_offset(Side::SIDE_RIGHT, 20);
    _currentAnglePanel->set_offset(Side::SIDE_BOTTOM, 20);
    _currentAnglePanel->set_pivot_offset(Vector2(20, 20));
    _currentAnglePanel->set((StringName)"theme_override_styles/panel", memnew(StyleBoxEmpty));
    _controlsContainer->add_child(_currentAnglePanel);

    Panel *panel = memnew(Panel);
    panel->set_anchors_and_offsets_preset(LayoutPreset::PRESET_CENTER_TOP);
    panel->set_anchor(Side::SIDE_LEFT, 0.5);
    panel->set_anchor(Side::SIDE_RIGHT, 0.5);
    panel->set_offset(Side::SIDE_LEFT, -5.0);
    panel->set_offset(Side::SIDE_RIGHT, 5.0);
    panel->set_offset(Side::SIDE_BOTTOM, 10.0);
    panel->set_h_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    Ref<StyleBoxFlat> panelStyle = memnew(StyleBoxFlat);
    panelStyle->set_bg_color(Color::html("#007fc6"));
    panelStyle->set_corner_radius_all(5);
    panel->set((StringName)"theme_override_styles/panel", panelStyle);
    _currentAnglePanel->add_child(panel);

    updateContent();

    set_scale(Vector2(0, 0));
    Ref<Tween> tween = get_tree()->create_tween();
    tween->tween_property(this, "scale", Vector2(1.0, 1.0), 0.1);
}

void PieMenu::_process(double delta) {
    int mouseOverItemIndex = getMouseOverItemIndex();
    if (mouseOverItemIndex > get_child_count() - 1) {
        return;
    }
    Node *itemForAngle = get_child(mouseOverItemIndex);

    DockPreviewButton *dockPreviewItem = Object::cast_to<DockPreviewButton>(itemForAngle);
    if (dockPreviewItem != nullptr) {
        if (_previewDockPreviewOverItem != nullptr) {
            _previewDockPreviewOverItem->clearMouseOver();
        }

        dockPreviewItem->showMouseOver();
        _previewDockPreviewOverItem = dockPreviewItem;

        if (!dockPreviewItem->get_tooltip_text().is_empty()) {
            _menuLabel->set_text(dockPreviewItem->get_tooltip_text());
        }
    }

    updateCurrentAnglePanel();
}

void PieMenu::_gui_input(const Ref<InputEvent> &event) {
    Ref<InputEventMouseButton> inputButton = Object::cast_to<InputEventMouseButton>(*event);

    if (!inputButton.is_null()) {
        if (inputButton->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            Node *itemForAngle = get_child((int) getMouseOverItemIndex());
            DockPreviewButton *dockPreviewItem = Object::cast_to<DockPreviewButton>(itemForAngle);
            if (dockPreviewItem != nullptr) {
                dockPreviewItem->onItemSelect();
            }
        }
    }
}

void PieMenu::set_buttonsRadiusRatio(const int value) {
    _buttonsRadiusRatio = value;
}

void PieMenu::set_buttonsMinRadius(const int value) {
    _buttonsMinRadius = value;
}

void PieMenu::set_label(const String value) {
    _label = value;
}

TypedArray<Control> PieMenu::getButtons() {
    TypedArray<Control> result = TypedArray<Control>();

    for (int i = 0; i < get_child_count(); i++) {
        Node* node = get_child(i);
        if (node != _controlsContainer) {
            result.append(node);
        }
    }

    return result;
}

void PieMenu::updateContent() {
    TypedArray<Control> buttons = getButtons();
    _buttonCount = buttons.size();

    if (_buttonCount > 0) {
        _startingButtonsIndex = get_children().find(buttons[0]);
        Control *firstButton = Object::cast_to<Control>(get_child(_startingButtonsIndex));
        _angleOffset = (float) (2 * Math_PI / _buttonCount);

        int buttonsRadius = Math::max(_buttonsMinRadius, _buttonsRadiusRatio * _buttonCount);
        Vector2 buttonSize = firstButton->get_size();

        float currentAngle = Math::deg_to_rad(90.0);
        for (int i = 0; i < _buttonCount; i++) {
            float xPosition = (float) Math::cos(currentAngle) * buttonsRadius;
            float yPosition = (float) Math::sin(currentAngle) * buttonsRadius;

            int buttonIndex = get_children().find(buttons[i]);
            Control *button = Object::cast_to<Control>(get_child(buttonIndex));
            button->set_position(Vector2(xPosition - (buttonSize.x / 2.0f), -yPosition - (buttonSize.y / 2.0f)));
            currentAngle += _angleOffset;
        }

        int backgroundSize = (int) (buttonsRadius * 2 + buttonSize.x * 2);
        _pieBackground->set_size(Vector2(backgroundSize, backgroundSize));
        _pieBackground->set_position(Vector2(-(backgroundSize / 2), -(backgroundSize / 2)));
    }

    _menuLabel->set_text(_label);
}

float PieMenu::getMouseCurrentAngle() {
    float angle = get_global_position().angle_to_point(get_global_mouse_position()) + Math::deg_to_rad(90.0);
    if (angle < 0) {
        angle += 2 * Math_PI;
    }
    angle = (2 * Math_PI) - angle;

    return angle;
}

int PieMenu::getMouseOverItemIndex() {
    float angle = getMouseCurrentAngle();

    float index = angle / _angleOffset;
    float offsetIndex = index - Math::floor(index);

    if (offsetIndex >= 0.5f) {
        index += 1;
    }
    index = Math::floor(index);

    if (index > _buttonCount - 1) {
        index = 0;
    }

    index += _startingButtonsIndex;

    return (int) index;
}

void PieMenu::updateCurrentAnglePanel() {
    if (_currentAnglePanel != nullptr) {
        float angle = get_global_position().angle_to_point(get_global_mouse_position()) + Math::deg_to_rad(90.0);
        if (angle < 0) {
            angle += 2 * Math_PI;
        }
        _currentAnglePanel->set_rotation(angle);
    }
}