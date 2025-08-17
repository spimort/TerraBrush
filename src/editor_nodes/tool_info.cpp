#include "tool_info.h"

#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

using namespace godot;

void ToolInfo::_bind_methods() {

}

ToolInfo::ToolInfo() {

}

ToolInfo::~ToolInfo() {}

void ToolInfo::_ready() {
    _panelContainer = memnew(PanelContainer);
    _panelContainer->set_size(Vector2(40, 41));
    _panelContainer->set_mouse_filter(MouseFilter::MOUSE_FILTER_IGNORE);
    _panelContainer->set_modulate(Color::html("#ffffff00"));

    Ref<StyleBoxFlat> styleBoxFlat = memnew(StyleBoxFlat);
    styleBoxFlat->set_bg_color(Color::html("#272727"));
    _panelContainer->set((StringName)"theme_override_styles/panel", styleBoxFlat);
    add_child(_panelContainer);

    MarginContainer *marginContainer = memnew(MarginContainer);
    marginContainer->set_mouse_filter(MouseFilter::MOUSE_FILTER_IGNORE);
    marginContainer->set((StringName)"theme_override_constants/margin_left", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_top", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_right", 10);
    marginContainer->set((StringName)"theme_override_constants/margin_bottom", 10);
    _panelContainer->add_child(marginContainer);

    _label = memnew(Label);
    marginContainer->add_child(_label);
}

void ToolInfo::setText(const String text) {
    if (_label->get_text() != text) {
        _label->set_text(text);

        if (!_tween.is_null()) {
            _tween->kill();
        }
        _tween = get_tree()->create_tween();

        if (!text.is_empty()) {
            _panelContainer->set_modulate(Color::html("#ffffff00"));
            _tween->tween_property(_panelContainer, (NodePath)"modulate", Color::html("#ffffff"), 0.1);
            _tween->tween_property(_panelContainer, (NodePath)"modulate", Color::html("#ffffff"), 5.0);
            _tween->tween_property(_panelContainer, (NodePath)"modulate", Color::html("#ffffff00"), 0.1);
        }
    }
}
