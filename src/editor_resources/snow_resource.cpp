#include "snow_resource.h"

#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void SnowResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_snowFactor"), &SnowResource::get_snowFactor);
    ClassDB::bind_method(D_METHOD("set_snowFactor", "value"), &SnowResource::set_snowFactor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "snowFactor"), "set_snowFactor", "get_snowFactor");

    ClassDB::bind_method(D_METHOD("get_snowInnerOffset"), &SnowResource::get_snowInnerOffset);
    ClassDB::bind_method(D_METHOD("set_snowInnerOffset", "value"), &SnowResource::set_snowInnerOffset);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "snowInnerOffset"), "set_snowInnerOffset", "get_snowInnerOffset");

    ClassDB::bind_method(D_METHOD("get_snowColorTexture"), &SnowResource::get_snowColorTexture);
    ClassDB::bind_method(D_METHOD("set_snowColorTexture", "value"), &SnowResource::set_snowColorTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowColorTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_snowColorTexture", "get_snowColorTexture");

    ClassDB::bind_method(D_METHOD("get_snowColorNormal"), &SnowResource::get_snowColorNormal);
    ClassDB::bind_method(D_METHOD("set_snowColorNormal", "value"), &SnowResource::set_snowColorNormal);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowColorNormal", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_snowColorNormal", "get_snowColorNormal");

    ClassDB::bind_method(D_METHOD("get_snowColorRoughness"), &SnowResource::get_snowColorRoughness);
    ClassDB::bind_method(D_METHOD("set_snowColorRoughness", "value"), &SnowResource::set_snowColorRoughness);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowColorRoughness", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_snowColorRoughness", "get_snowColorRoughness");

    ClassDB::bind_method(D_METHOD("get_snowColorDetail"), &SnowResource::get_snowColorDetail);
    ClassDB::bind_method(D_METHOD("set_snowColorDetail", "value"), &SnowResource::set_snowColorDetail);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "snowColorDetail"), "set_snowColorDetail", "get_snowColorDetail");

    ClassDB::bind_method(D_METHOD("get_noise"), &SnowResource::get_noise);
    ClassDB::bind_method(D_METHOD("set_noise", "value"), &SnowResource::set_noise);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noise", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_noise", "get_noise");

    ClassDB::bind_method(D_METHOD("get_noiseFactor"), &SnowResource::get_noiseFactor);
    ClassDB::bind_method(D_METHOD("set_noiseFactor", "value"), &SnowResource::set_noiseFactor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "noiseFactor"), "set_noiseFactor", "get_noiseFactor");

    ClassDB::bind_method(D_METHOD("get_metallic"), &SnowResource::get_metallic);
    ClassDB::bind_method(D_METHOD("set_metallic", "value"), &SnowResource::set_metallic);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "metallic", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_metallic", "get_metallic");

    ClassDB::bind_method(D_METHOD("get_visualInstanceLayers"), &SnowResource::get_visualInstanceLayers);
    ClassDB::bind_method(D_METHOD("set_visualInstanceLayers", "value"), &SnowResource::set_visualInstanceLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visualInstanceLayers", PROPERTY_HINT_LAYERS_3D_RENDER ), "set_visualInstanceLayers", "get_visualInstanceLayers");

    ClassDB::bind_method(D_METHOD("get_customShader"), &SnowResource::get_customShader);
    ClassDB::bind_method(D_METHOD("set_customShader", "value"), &SnowResource::set_customShader);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "customShader", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_customShader", "get_customShader");
}

SnowResource::SnowResource() {
    _snowFactor = 1;
    _snowInnerOffset = 0.3;
    _snowColorDetail = 20;
    _visualInstanceLayers = 1;
}

SnowResource::~SnowResource() {

}

float SnowResource::get_snowFactor() const {
    return _snowFactor;
}
void SnowResource::set_snowFactor (const float value) {
    _snowFactor = value;
}

float SnowResource::get_snowInnerOffset() const {
    return _snowInnerOffset;
}
void SnowResource::set_snowInnerOffset (const float value) {
    _snowInnerOffset = value;
}

Ref<Texture2D> SnowResource::get_snowColorTexture() const {
    return _snowColorTexture;
}
void SnowResource::set_snowColorTexture (const Ref<Texture2D> &value) {
    _snowColorTexture = value;
}

Ref<Texture2D> SnowResource::get_snowColorNormal() const {
    return _snowColorNormal;
}
void SnowResource::set_snowColorNormal (const Ref<Texture2D> &value) {
    _snowColorNormal = value;
}

Ref<Texture2D> SnowResource::get_snowColorRoughness() const {
    return _snowColorRoughness;
}
void SnowResource::set_snowColorRoughness (const Ref<Texture2D> &value) {
    _snowColorRoughness = value;
}

float SnowResource::get_snowColorDetail() const {
    return _snowColorDetail;
}
void SnowResource::set_snowColorDetail (const float value) {
    _snowColorDetail = value;
}

Ref<Texture2D> SnowResource::get_noise() const {
    return _noise;
}
void SnowResource::set_noise (const Ref<Texture2D> &value) {
    _noise = value;
}

float SnowResource::get_noiseFactor() const {
    return _noiseFactor;
}
void SnowResource::set_noiseFactor (const float value) {
    _noiseFactor = value;
}

float SnowResource::get_metallic() const {
    return _metallic;
}
void SnowResource::set_metallic (const float value) {
    _metallic = value;
}

int SnowResource::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void SnowResource::set_visualInstanceLayers (const int value) {
    _visualInstanceLayers = value;
}

Ref<ShaderMaterial> SnowResource::get_customShader() const {
    return _customShader;
}
void SnowResource::set_customShader (const Ref<ShaderMaterial> &value) {
    _customShader = value;

    if (!value.is_null() && value->get_shader().is_null()) {
        Ref<Shader> defaultShader = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader");
        auto defaultCode = defaultShader->get_code();

        Ref<Shader> newShader = memnew(Shader);
        newShader->set_code(defaultCode);
        value->set_shader(newShader);
    }
}