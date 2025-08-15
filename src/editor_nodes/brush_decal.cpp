#include "brush_decal.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void BrushDecal::_bind_methods() {}

BrushDecal::BrushDecal() {}

BrushDecal::~BrushDecal() {}

void BrushDecal::_ready() {
    _decal = memnew(Decal);
    add_child(_decal);
}

void BrushDecal::setBrushImage(const Ref<Image> image) {
    Ref<Image> imageCopy = memnew(Image);
    imageCopy->copy_from(image);

    for (int x = 0; x < imageCopy->get_width(); x++) {
        for (int y = 0; y < imageCopy->get_height(); y++) {
            auto currentPixel = imageCopy->get_pixel(x, y);
            imageCopy->set_pixel(x, y, Color(1, 1, 1, currentPixel.a));
        }
    }

    _decal->set_texture(Decal::DecalTexture::TEXTURE_ALBEDO, ImageTexture::create_from_image(imageCopy));
    _decal->set_modulate(ProjectSettings::get_singleton()->get_setting(SettingContants::DecalColor()));
}