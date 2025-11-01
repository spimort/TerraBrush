#include "brush_decal.h"
#include "../misc/setting_contants.h"
#include "../misc/string_names.h"

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

using namespace godot;

void BrushDecal::_bind_methods() {}

BrushDecal::BrushDecal() {}

BrushDecal::~BrushDecal() {}

void BrushDecal::_ready() {
    _decal = memnew(Decal);
    add_child(_decal);

    MeshInstance3D *sphereMeshInstance = memnew(MeshInstance3D);
    add_child(sphereMeshInstance);

    _sphereMesh = Ref<SphereMesh>(memnew(SphereMesh));
    sphereMeshInstance->set_mesh(_sphereMesh);

    Ref<ShaderMaterial> sphereMaterial = memnew(ShaderMaterial);
    sphereMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/decal_sphere_shader.gdshader"));
    sphereMeshInstance->set_material_override(sphereMaterial);

    sphereMaterial->set_shader_parameter(StringNames::Color(), ProjectSettings::get_singleton()->get_setting(SettingContants::DecalColor(), SettingContants::DecalColorDefaultValue()));
    sphereMaterial->set_shader_parameter(StringNames::Compatibility(), RenderingServer::get_singleton()->get_current_rendering_method() == "gl_compatibility");
}

void BrushDecal::setSize(const int size) {
    _decal->set_size(Vector3(size, 1000, size));
    _sphereMesh->set_radius(size / 2.0);
    _sphereMesh->set_height(size);
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
    _decal->set_modulate(ProjectSettings::get_singleton()->get_setting(SettingContants::DecalColor(), SettingContants::DecalColorDefaultValue()));
}