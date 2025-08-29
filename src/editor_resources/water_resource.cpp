#include "water_resource.h"

#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void WaterResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_waterFactor"), &WaterResource::get_waterFactor);
    ClassDB::bind_method(D_METHOD("set_waterFactor", "value"), &WaterResource::set_waterFactor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterFactor"), "set_waterFactor", "get_waterFactor");

    ClassDB::bind_method(D_METHOD("get_waterInnerOffset"), &WaterResource::get_waterInnerOffset);
    ClassDB::bind_method(D_METHOD("set_waterInnerOffset", "value"), &WaterResource::set_waterInnerOffset);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterInnerOffset"), "set_waterInnerOffset", "get_waterInnerOffset");

    ClassDB::bind_method(D_METHOD("get_waterColor"), &WaterResource::get_waterColor);
    ClassDB::bind_method(D_METHOD("set_waterColor", "value"), &WaterResource::set_waterColor);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "waterColor", PROPERTY_HINT_COLOR_NO_ALPHA), "set_waterColor", "get_waterColor");

    ClassDB::bind_method(D_METHOD("get_waterFresnelColor"), &WaterResource::get_waterFresnelColor);
    ClassDB::bind_method(D_METHOD("set_waterFresnelColor", "value"), &WaterResource::set_waterFresnelColor);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "waterFresnelColor", PROPERTY_HINT_COLOR_NO_ALPHA), "set_waterFresnelColor", "get_waterFresnelColor");

    ClassDB::bind_method(D_METHOD("get_waterMetallic"), &WaterResource::get_waterMetallic);
    ClassDB::bind_method(D_METHOD("set_waterMetallic", "value"), &WaterResource::set_waterMetallic);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterMetallic", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_waterMetallic", "get_waterMetallic");

    ClassDB::bind_method(D_METHOD("get_waterRoughness"), &WaterResource::get_waterRoughness);
    ClassDB::bind_method(D_METHOD("set_waterRoughness", "value"), &WaterResource::set_waterRoughness);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterRoughness", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_waterRoughness", "get_waterRoughness");

    ClassDB::bind_method(D_METHOD("get_waterNormalMap"), &WaterResource::get_waterNormalMap);
    ClassDB::bind_method(D_METHOD("set_waterNormalMap", "value"), &WaterResource::set_waterNormalMap);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterNormalMap", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_waterNormalMap", "get_waterNormalMap");

    ClassDB::bind_method(D_METHOD("get_waterNormalMap2"), &WaterResource::get_waterNormalMap2);
    ClassDB::bind_method(D_METHOD("set_waterNormalMap2", "value"), &WaterResource::set_waterNormalMap2);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterNormalMap2", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_waterNormalMap2", "get_waterNormalMap2");

    ClassDB::bind_method(D_METHOD("get_waterTimeScale"), &WaterResource::get_waterTimeScale);
    ClassDB::bind_method(D_METHOD("set_waterTimeScale", "value"), &WaterResource::set_waterTimeScale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterTimeScale"), "set_waterTimeScale", "get_waterTimeScale");

    ClassDB::bind_method(D_METHOD("get_waterStrength"), &WaterResource::get_waterStrength);
    ClassDB::bind_method(D_METHOD("set_waterStrength", "value"), &WaterResource::set_waterStrength);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterStrength"), "set_waterStrength", "get_waterStrength");

    ClassDB::bind_method(D_METHOD("get_waterWave"), &WaterResource::get_waterWave);
    ClassDB::bind_method(D_METHOD("set_waterWave", "value"), &WaterResource::set_waterWave);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterWave", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_waterWave", "get_waterWave");

    ClassDB::bind_method(D_METHOD("get_waterNoiseScale"), &WaterResource::get_waterNoiseScale);
    ClassDB::bind_method(D_METHOD("set_waterNoiseScale", "value"), &WaterResource::set_waterNoiseScale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterNoiseScale"), "set_waterNoiseScale", "get_waterNoiseScale");

    ClassDB::bind_method(D_METHOD("get_waterHeightScale"), &WaterResource::get_waterHeightScale);
    ClassDB::bind_method(D_METHOD("set_waterHeightScale", "value"), &WaterResource::set_waterHeightScale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterHeightScale"), "set_waterHeightScale", "get_waterHeightScale");

    ClassDB::bind_method(D_METHOD("get_waterColorDeep"), &WaterResource::get_waterColorDeep);
    ClassDB::bind_method(D_METHOD("set_waterColorDeep", "value"), &WaterResource::set_waterColorDeep);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "waterColorDeep"), "set_waterColorDeep", "get_waterColorDeep");

    ClassDB::bind_method(D_METHOD("get_waterColorShallow"), &WaterResource::get_waterColorShallow);
    ClassDB::bind_method(D_METHOD("set_waterColorShallow", "value"), &WaterResource::set_waterColorShallow);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "waterColorShallow"), "set_waterColorShallow", "get_waterColorShallow");

    ClassDB::bind_method(D_METHOD("get_waterBeersLaw"), &WaterResource::get_waterBeersLaw);
    ClassDB::bind_method(D_METHOD("set_waterBeersLaw", "value"), &WaterResource::set_waterBeersLaw);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterBeersLaw"), "set_waterBeersLaw", "get_waterBeersLaw");

    ClassDB::bind_method(D_METHOD("get_waterDepthOffset"), &WaterResource::get_waterDepthOffset);
    ClassDB::bind_method(D_METHOD("set_waterDepthOffset", "value"), &WaterResource::set_waterDepthOffset);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterDepthOffset"), "set_waterDepthOffset", "get_waterDepthOffset");

    ClassDB::bind_method(D_METHOD("get_waterEdgeScale"), &WaterResource::get_waterEdgeScale);
    ClassDB::bind_method(D_METHOD("set_waterEdgeScale", "value"), &WaterResource::set_waterEdgeScale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterEdgeScale"), "set_waterEdgeScale", "get_waterEdgeScale");

    ClassDB::bind_method(D_METHOD("get_waterNear"), &WaterResource::get_waterNear);
    ClassDB::bind_method(D_METHOD("set_waterNear", "value"), &WaterResource::set_waterNear);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterNear"), "set_waterNear", "get_waterNear");

    ClassDB::bind_method(D_METHOD("get_waterFar"), &WaterResource::get_waterFar);
    ClassDB::bind_method(D_METHOD("set_waterFar", "value"), &WaterResource::set_waterFar);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "waterFar"), "set_waterFar", "get_waterFar");

    ClassDB::bind_method(D_METHOD("get_waterEdgeColor"), &WaterResource::get_waterEdgeColor);
    ClassDB::bind_method(D_METHOD("set_waterEdgeColor", "value"), &WaterResource::set_waterEdgeColor);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "waterEdgeColor", PROPERTY_HINT_COLOR_NO_ALPHA), "set_waterEdgeColor", "get_waterEdgeColor");

    ClassDB::bind_method(D_METHOD("get_visualInstanceLayers"), &WaterResource::get_visualInstanceLayers);
    ClassDB::bind_method(D_METHOD("set_visualInstanceLayers", "value"), &WaterResource::set_visualInstanceLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visualInstanceLayers", PROPERTY_HINT_LAYERS_3D_RENDER), "set_visualInstanceLayers", "get_visualInstanceLayers");

    ClassDB::bind_method(D_METHOD("get_customShader"), &WaterResource::get_customShader);
    ClassDB::bind_method(D_METHOD("set_customShader", "value"), &WaterResource::set_customShader);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "customShader", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_customShader", "get_customShader");
}

WaterResource::WaterResource() {
    _waterFactor = 1;
    _waterInnerOffset = 0.2;
    _waterColor = Color::named("WHITE");
    _waterFresnelColor = Color::named("WHITE");
    _waterMetallic = 0;
    _waterRoughness = 0;
    _waterNormalMap = Ref<Texture2D>(nullptr);
    _waterNormalMap2 = Ref<Texture2D>(nullptr);
    _waterTimeScale = 0.1;
    _waterStrength = 0.4;
    _waterWave = Ref<Texture2D>(nullptr);
    _waterNoiseScale = 10;
    _waterHeightScale = 0.15;
    _waterColorDeep = Color::named("WHITE");
    _waterColorShallow = Color::named("WHITE");
    _waterBeersLaw = 2;
    _waterDepthOffset = -0.75;
    _waterEdgeScale = 0.1;
    _waterNear = 0.5;
    _waterFar = 100;
    _waterEdgeColor = Color::named("WHITE");
    _visualInstanceLayers = 1;
    _customShader = Ref<ShaderMaterial>(nullptr);
}

WaterResource::~WaterResource() {}

float WaterResource::get_waterFactor() const {
    return _waterFactor;
}
void WaterResource::set_waterFactor(const float value) {
    _waterFactor = value;
}

float WaterResource::get_waterInnerOffset() const {
    return _waterInnerOffset;
}
void WaterResource::set_waterInnerOffset(const float value) {
    _waterInnerOffset = value;
}

Color WaterResource::get_waterColor() const {
    return _waterColor;
}
void WaterResource::set_waterColor(const Color value) {
    _waterColor = value;
}

Color WaterResource::get_waterFresnelColor() const {
    return _waterFresnelColor;
}
void WaterResource::set_waterFresnelColor(const Color value) {
    _waterFresnelColor = value;
}

float WaterResource::get_waterMetallic() const {
    return _waterMetallic;
}
void WaterResource::set_waterMetallic(const float value) {
    _waterMetallic = value;
}

float WaterResource::get_waterRoughness() const {
    return _waterRoughness;
}
void WaterResource::set_waterRoughness(const float value) {
    _waterRoughness = value;
}

Ref<Texture2D> WaterResource::get_waterNormalMap() const {
    return _waterNormalMap;
}
void WaterResource::set_waterNormalMap(const Ref<Texture2D> &value) {
    _waterNormalMap = value;
}

Ref<Texture2D> WaterResource::get_waterNormalMap2() const {
    return _waterNormalMap2;
}
void WaterResource::set_waterNormalMap2(const Ref<Texture2D> &value) {
    _waterNormalMap2 = value;
}

float WaterResource::get_waterTimeScale() const {
    return _waterTimeScale;
}
void WaterResource::set_waterTimeScale(const float value) {
    _waterTimeScale = value;
}

float WaterResource::get_waterStrength() const {
    return _waterStrength;
}
void WaterResource::set_waterStrength(const float value) {
    _waterStrength = value;
}

Ref<Texture2D> WaterResource::get_waterWave() const {
    return _waterWave;
}
void WaterResource::set_waterWave(const Ref<Texture2D> &value) {
    _waterWave = value;
}

float WaterResource::get_waterNoiseScale() const {
    return _waterNoiseScale;
}
void WaterResource::set_waterNoiseScale(const float value) {
    _waterNoiseScale = value;
}

float WaterResource::get_waterHeightScale() const {
    return _waterHeightScale;
}
void WaterResource::set_waterHeightScale(const float value) {
    _waterHeightScale = value;
}

Color WaterResource::get_waterColorDeep() const {
    return _waterColorDeep;
}
void WaterResource::set_waterColorDeep(const Color value) {
    _waterColorDeep = value;
}

Color WaterResource::get_waterColorShallow() const {
    return _waterColorShallow;
}
void WaterResource::set_waterColorShallow(const Color value) {
    _waterColorShallow = value;
}

float WaterResource::get_waterBeersLaw() const {
    return _waterBeersLaw;
}
void WaterResource::set_waterBeersLaw(const float value) {
    _waterBeersLaw = value;
}

float WaterResource::get_waterDepthOffset() const {
    return _waterDepthOffset;
}
void WaterResource::set_waterDepthOffset(const float value) {
    _waterDepthOffset = value;
}

float WaterResource::get_waterEdgeScale() const {
    return _waterEdgeScale;
}
void WaterResource::set_waterEdgeScale(const float value) {
    _waterEdgeScale = value;
}

float WaterResource::get_waterNear() const {
    return _waterNear;
}
void WaterResource::set_waterNear(const float value) {
    _waterNear = value;
}

float WaterResource::get_waterFar() const {
    return _waterFar;
}
void WaterResource::set_waterFar(const float value) {
    _waterFar = value;
}

Color WaterResource::get_waterEdgeColor() const {
    return _waterEdgeColor;
}
void WaterResource::set_waterEdgeColor(const Color value) {
    _waterEdgeColor = value;
}

int WaterResource::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void WaterResource::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

Ref<ShaderMaterial> WaterResource::get_customShader() const {
    return _customShader;
}
void WaterResource::set_customShader(const Ref<ShaderMaterial> &value) {
    _customShader = value;

    if (!value.is_null() && value->get_shader().is_null()) {
        Ref<Shader> defaultShader = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader");
        auto defaultCode = defaultShader->get_code();

        Ref<Shader> newShader = memnew(Shader);
        newShader->set_code(defaultCode);
        value->set_shader(newShader);
    }
}