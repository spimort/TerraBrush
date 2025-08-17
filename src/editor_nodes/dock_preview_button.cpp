#include "dock_preview_button.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void DockPreviewButton::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onItemSelect"), &DockPreviewButton::onItemSelect);
    ClassDB::bind_method(D_METHOD("onPreviewThumbnailReady", "path", "preview", "thumbnailPreview", "resourceName"), &DockPreviewButton::onPreviewThumbnailReady);
}

void DockPreviewButton::_notification(int what) {
    if (what == NOTIFICATION_DRAW) {
        setTextureColor(get_draw_mode());
    }
}

DockPreviewButton::DockPreviewButton() {
    _iconType = IconType::Square;
}

DockPreviewButton::~DockPreviewButton() {}

void DockPreviewButton::_ready() {
    set_custom_minimum_size(Vector2(48, 48));
    set_ignore_texture_size(true);
    set_stretch_mode(TextureButton::StretchMode::STRETCH_SCALE);
    set_toggle_mode(true);
    set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    set_anchor(Side::SIDE_RIGHT, 1.0);
    set_anchor(Side::SIDE_BOTTOM, 1.0);
    set_h_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    set_v_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);

    _marginContainer = memnew(MarginContainer);
    _marginContainer->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    _marginContainer->set_anchor(Side::SIDE_RIGHT, 1.0);
    _marginContainer->set_anchor(Side::SIDE_BOTTOM, 1.0);
    _marginContainer->set_h_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    _marginContainer->set_v_grow_direction(GrowDirection::GROW_DIRECTION_BOTH);
    add_child(_marginContainer);

    _textureRect = memnew(TextureRect);
    _textureRect->set_expand_mode(TextureRect::ExpandMode::EXPAND_IGNORE_SIZE);
    _marginContainer->add_child(_textureRect);

    _label = memnew(Label);
    _label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    _label->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    _label->set((StringName)"theme_override_font_sizes/font_size", 10);
    _marginContainer->add_child(_label);

    _textureSquare = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Buttons/square_white.png");
    _textureCircle = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Buttons/circle_white.png");
    _textureCircleWithSign = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Buttons/circle_sign_white.png");

    connect("pressed", Callable(this, "onItemSelect"));

    if (!_buttonImage.is_null()) {
        setTextureImage(_buttonImage);
    }

    calculateTextures();

    Color iconsColor = ProjectSettings::get_singleton()->get_setting(SettingContants::IconsColor());

    _normalColor = iconsColor;
    _hoverColor = _normalColor.lightened(0.4f);
    _pressedColor = _normalColor.lightened(0.5f);

    if (_marginContainer != nullptr && _margin > 0) {
        _marginContainer->set((StringName)"theme_override_constants/margin_left", _margin);
        _marginContainer->set((StringName)"theme_override_constants/margin_right", _margin);
        _marginContainer->set((StringName)"theme_override_constants/margin_top", _margin);
        _marginContainer->set((StringName)"theme_override_constants/margin_bottom", _margin);
    }

    if (_label != nullptr && !_text.is_empty()) {
        _label->set_text(_text);
    }

    _marginContainer->set("theme_override_constants/margin_left", 0);
    _marginContainer->set("theme_override_constants/margin_top", 0);
    _marginContainer->set("theme_override_constants/margin_right", 0);
    _marginContainer->set("theme_override_constants/margin_bottom", 0);
}

void DockPreviewButton::set_buttonImage(const Ref<Texture2D> &value) {
    _buttonImage = value;
}

void DockPreviewButton::set_iconType(const IconType value) {
    _iconType = value;
}

void DockPreviewButton::set_margin(const int value) {
    _margin = value;
}

void DockPreviewButton::set_text(const String value) {
    _text = value;
}

void DockPreviewButton::set_onSelect(const Callable value) {
    _onSelect = value;
}

void DockPreviewButton::calculateTextures() {
    switch (_iconType) {
        case IconType::Square:
            set_texture_normal(_textureSquare);
            break;
        case IconType::Circle:
            set_texture_normal(_textureCircle);
            break;
        case IconType::CircleWithSign:
            set_texture_normal(_textureCircleWithSign);
            break;
    }
}

void DockPreviewButton::setTextureColor(DrawMode drawMode) {
    Color newColor = _normalColor;

    switch(drawMode) {
        case DrawMode::DRAW_NORMAL:
            newColor = _normalColor;
            break;
        case DrawMode::DRAW_HOVER:
            newColor = _hoverColor;
            break;
        case DrawMode::DRAW_PRESSED:
            newColor = _pressedColor;
            break;
        case DrawMode::DRAW_HOVER_PRESSED:
            newColor = _pressedColor;
            break;
    }

    Ref<Tween> tween = get_tree()->create_tween();
    tween->tween_property(this, "self_modulate", newColor, 0.1);
}

void DockPreviewButton::onPreviewThumbnailReady(const String path, const Ref<Texture2D> &preview, const Ref<Texture2D> &thumbnailPreview, const String resourceName) {
    if (preview == nullptr) {
        _label->set_text(resourceName);
    } else {
        _label->set_text("");
        setTextureImage(preview);
    }
}

void DockPreviewButton::onItemSelect() {
    if (!_onSelect.is_null()) {
        _onSelect.call();
    }
}

void DockPreviewButton::setTextureImage(const Ref<Texture2D> &toolImage) {
    _textureRect->set_texture(toolImage);
}

void DockPreviewButton::showMouseOver() {
    setTextureColor(DrawMode::DRAW_HOVER);
}

void DockPreviewButton::clearMouseOver() {
    setTextureColor(DrawMode::DRAW_NORMAL);
}

void DockPreviewButton::loadResourcePreview(const Ref<Resource> &resource) {
    EditorInterface::get_singleton()->get_resource_previewer()->queue_edited_resource_preview(resource, this, "onPreviewThumbnailReady", resource->get_path().get_file());
}
