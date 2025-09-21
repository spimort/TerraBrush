#include "import_image_row.h"
#include "../misc/setting_contants.h"
#include "../misc/dialog_utils.h"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/gradient_texture1_d.hpp>
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void ImportImageRow::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onMouseEnter"), &ImportImageRow::onMouseEnter);
    ClassDB::bind_method(D_METHOD("onMouseExit"), &ImportImageRow::onMouseExit);
    ClassDB::bind_method(D_METHOD("onGuiInput", "event"), &ImportImageRow::onGuiInput);
}

ImportImageRow::ImportImageRow() {}

ImportImageRow::~ImportImageRow() {}

void ImportImageRow::_ready() {
    Ref<GradientTexture1D> gradientTexture = memnew(GradientTexture1D);
    Ref<Gradient> gradient = memnew(Gradient);
    gradient->set_offsets(PackedFloat32Array(Array::make(1.0)));
    gradient->set_colors(PackedColorArray(Array::make(Color::html("#ffffff62"))));
    gradientTexture->set_gradient(gradient);
    _originalNormalTexture = gradientTexture;

    Color iconsColor = ProjectSettings::get_singleton()->get_setting(SettingContants::IconsColor());

    set_size(Vector2(93, 88));

    Ref<StyleBoxFlat> styleBoxFlat = memnew(StyleBoxFlat);
    styleBoxFlat->set_bg_color(iconsColor);
    styleBoxFlat->set_corner_radius_all(5);
    set("theme_override_styles/panel", styleBoxFlat);

    MarginContainer *marginContainer = memnew(MarginContainer);
    marginContainer->set("theme_override_constants/margin_left", 10);
    marginContainer->set("theme_override_constants/margin_right", 10);
    marginContainer->set("theme_override_constants/margin_top", 10);
    marginContainer->set("theme_override_constants/margin_bottom", 10);
    add_child(marginContainer);

    HBoxContainer *hbox = memnew(HBoxContainer);
    marginContainer->add_child(hbox);

    Label *label = memnew(Label);
    label->set_text(_imageTypeName);
    label->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    hbox->add_child(label);

    PanelContainer *panelContainer = memnew(PanelContainer);
    Ref<StyleBoxFlat> panelContainerStyleBoxFlat = memnew(StyleBoxFlat);
    panelContainerStyleBoxFlat->set_bg_color(Color::html("#999999"));
    panelContainerStyleBoxFlat->set_border_width_all(2);
    panelContainerStyleBoxFlat->set_border_color(Color::named("WHITE"));
    panelContainerStyleBoxFlat->set_corner_radius_all(5);
    panelContainer->set("theme_override_styles/panel", panelContainerStyleBoxFlat);
    hbox->add_child(panelContainer);

    _textureButton = memnew(TextureButton);
    _textureButton->set_ignore_texture_size(true);
    _textureButton->set_stretch_mode(TextureButton::StretchMode::STRETCH_SCALE);
    _textureButton->set_button_mask(MouseButtonMask::MOUSE_BUTTON_MASK_LEFT & MouseButtonMask::MOUSE_BUTTON_MASK_RIGHT);
    _textureButton->set_custom_minimum_size(Vector2(64, 64));
    _textureButton->set_default_cursor_shape(CursorShape::CURSOR_POINTING_HAND);
    panelContainer->add_child(_textureButton);

    if (_imageTexture.is_null()) {
        _textureButton->set_texture_normal(_originalNormalTexture);
    } else {
        _textureButton->set_texture_normal(_imageTexture);
    }

    _textureButton->connect("mouse_entered", Callable(this, "onMouseEnter"));
    _textureButton->connect("mouse_exited", Callable(this, "onMouseExit"));
    _textureButton->connect("gui_input", Callable(this, "onGuiInput"));
}

void ImportImageRow::set_imageTypeName(const String value) {
    _imageTypeName = value;
}

Ref<Texture2D> ImportImageRow::get_imageTexture() const {
    return _imageTexture;
}

void ImportImageRow::set_imageTexture(const Ref<Texture2D> &value) {
    _imageTexture = value;
}

void ImportImageRow::onMouseEnter() {
    Ref<Tween> tween = create_tween();
    tween->tween_property(_textureButton, "self_modulate", Color(1, 1, 1, 0.3f), 0.1);
}

void ImportImageRow::onMouseExit() {
    Ref<Tween> tween = create_tween();
    tween->tween_property(_textureButton, "self_modulate", Color::named("WHITE"), 0.1);
}

void ImportImageRow::onGuiInput(const Ref<InputEvent> &event) {
    if (Object::cast_to<InputEventMouseButton>(event.ptr()) != nullptr) {
        Ref<InputEventMouseButton> mouseEvent = Object::cast_to<InputEventMouseButton>(event.ptr());

        if (!mouseEvent->is_echo() && !_isSelectorOpen) {
            if (mouseEvent->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
                _isSelectorOpen = true;

                DialogUtils::showFileDialog(
                    this,
                    ([&](String file) {
                        _isSelectorOpen = false;

                        if (!file.is_empty()) {
                            Ref<Texture2D> imageResource = ResourceLoader::get_singleton()->load(file);
                            _textureButton->set_texture_normal(imageResource);
                            _imageTexture = imageResource;
                        }
                    }),
                    EditorFileDialog::ACCESS_FILESYSTEM,
                    EditorFileDialog::FILE_MODE_OPEN_FILE,
                    Array::make("*.png, *.jpg, *.jpeg, *.bmp, *.exr ; Supported Images", "*res, *.tres ; Resources"),
                    true
                );
            } else if (mouseEvent->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT) {
                _imageTexture = Ref<Texture2D>(nullptr);
                _textureButton->set_texture_normal(_originalNormalTexture);
            }
        }
    }
}