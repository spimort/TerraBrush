#include "foliage_definition_resource.h"
#include "../nodes/foliage.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void FoliageDefinitionResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_strategy"), &FoliageDefinitionResource::get_strategy);
    ClassDB::bind_method(D_METHOD("set_strategy", "value"), &FoliageDefinitionResource::set_strategy);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "strategy", PROPERTY_HINT_ENUM, "MultiMesh:1,GPUParticle:2"), "set_strategy", "get_strategy");

    ClassDB::bind_method(D_METHOD("get_mesh"), &FoliageDefinitionResource::get_mesh);
    ClassDB::bind_method(D_METHOD("set_mesh", "value"), &FoliageDefinitionResource::set_mesh);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");

    ClassDB::bind_method(D_METHOD("get_meshScale"), &FoliageDefinitionResource::get_meshScale);
    ClassDB::bind_method(D_METHOD("set_meshScale", "value"), &FoliageDefinitionResource::set_meshScale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "meshScale", PROPERTY_HINT_LINK), "set_meshScale", "get_meshScale");

    ClassDB::bind_method(D_METHOD("get_windStrength"), &FoliageDefinitionResource::get_windStrength);
    ClassDB::bind_method(D_METHOD("set_windStrength", "value"), &FoliageDefinitionResource::set_windStrength);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "windStrength"), "set_windStrength", "get_windStrength");

    ClassDB::bind_method(D_METHOD("get_noiseTexture"), &FoliageDefinitionResource::get_noiseTexture);
    ClassDB::bind_method(D_METHOD("set_noiseTexture", "value"), &FoliageDefinitionResource::set_noiseTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noiseTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_noiseTexture", "get_noiseTexture");

    ClassDB::bind_method(D_METHOD("get_visualInstanceLayers"), &FoliageDefinitionResource::get_visualInstanceLayers);
    ClassDB::bind_method(D_METHOD("set_visualInstanceLayers", "value"), &FoliageDefinitionResource::set_visualInstanceLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visualInstanceLayers", PROPERTY_HINT_LAYERS_3D_RENDER), "set_visualInstanceLayers", "get_visualInstanceLayers");

    ClassDB::bind_method(D_METHOD("get_lodLevels"), &FoliageDefinitionResource::get_lodLevels);
    ClassDB::bind_method(D_METHOD("set_lodLevels", "value"), &FoliageDefinitionResource::set_lodLevels);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lodLevels"), "set_lodLevels", "get_lodLevels");

    ClassDB::bind_method(D_METHOD("get_lodRowsPerLevel"), &FoliageDefinitionResource::get_lodRowsPerLevel);
    ClassDB::bind_method(D_METHOD("set_lodRowsPerLevel", "value"), &FoliageDefinitionResource::set_lodRowsPerLevel);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lodRowsPerLevel"), "set_lodRowsPerLevel", "get_lodRowsPerLevel");

    ClassDB::bind_method(D_METHOD("get_lodInitialCellWidth"), &FoliageDefinitionResource::get_lodInitialCellWidth);
    ClassDB::bind_method(D_METHOD("set_lodInitialCellWidth", "value"), &FoliageDefinitionResource::set_lodInitialCellWidth);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lodInitialCellWidth"), "set_lodInitialCellWidth", "get_lodInitialCellWidth");

    ClassDB::bind_method(D_METHOD("get_albedo"), &FoliageDefinitionResource::get_albedo);
    ClassDB::bind_method(D_METHOD("set_albedo", "value"), &FoliageDefinitionResource::set_albedo);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo"), "set_albedo", "get_albedo");

    ClassDB::bind_method(D_METHOD("get_albedoTextures"), &FoliageDefinitionResource::get_albedoTextures);
    ClassDB::bind_method(D_METHOD("set_albedoTextures", "value"), &FoliageDefinitionResource::set_albedoTextures);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "albedoTextures", PROPERTY_HINT_TYPE_STRING, "24/17:Texture2D"), "set_albedoTextures", "get_albedoTextures");

    ClassDB::bind_method(D_METHOD("get_nearestTextureFilter"), &FoliageDefinitionResource::get_nearestTextureFilter);
    ClassDB::bind_method(D_METHOD("set_nearestTextureFilter", "value"), &FoliageDefinitionResource::set_nearestTextureFilter);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "nearestTextureFilter"), "set_nearestTextureFilter", "get_nearestTextureFilter");

    ClassDB::bind_method(D_METHOD("get_useGroundColor"), &FoliageDefinitionResource::get_useGroundColor);
    ClassDB::bind_method(D_METHOD("set_useGroundColor", "value"), &FoliageDefinitionResource::set_useGroundColor);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "useGroundColor"), "set_useGroundColor", "get_useGroundColor");

    ClassDB::bind_method(D_METHOD("get_castShadow"), &FoliageDefinitionResource::get_castShadow);
    ClassDB::bind_method(D_METHOD("set_castShadow", "value"), &FoliageDefinitionResource::set_castShadow);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "castShadow"), "set_castShadow", "get_castShadow");

    ClassDB::bind_method(D_METHOD("get_useBrushScale"), &FoliageDefinitionResource::get_useBrushScale);
    ClassDB::bind_method(D_METHOD("set_useBrushScale", "value"), &FoliageDefinitionResource::set_useBrushScale);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "useBrushScale"), "set_useBrushScale", "get_useBrushScale");

    ClassDB::bind_method(D_METHOD("get_scaleNoiseTexture"), &FoliageDefinitionResource::get_scaleNoiseTexture);
    ClassDB::bind_method(D_METHOD("set_scaleNoiseTexture", "value"), &FoliageDefinitionResource::set_scaleNoiseTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scaleNoiseTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_scaleNoiseTexture", "get_scaleNoiseTexture");

    ClassDB::bind_method(D_METHOD("get_randomPlacementRange"), &FoliageDefinitionResource::get_randomPlacementRange);
    ClassDB::bind_method(D_METHOD("set_randomPlacementRange", "value"), &FoliageDefinitionResource::set_randomPlacementRange);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomPlacementRange"), "set_randomPlacementRange", "get_randomPlacementRange");

    ClassDB::bind_method(D_METHOD("get_applyOnTextureIndexes"), &FoliageDefinitionResource::get_applyOnTextureIndexes);
    ClassDB::bind_method(D_METHOD("set_applyOnTextureIndexes", "value"), &FoliageDefinitionResource::set_applyOnTextureIndexes);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "applyOnTextureIndexes", PROPERTY_HINT_TYPE_STRING, "2/0:"), "set_applyOnTextureIndexes", "get_applyOnTextureIndexes");

    ClassDB::bind_method(D_METHOD("get_customShader"), &FoliageDefinitionResource::get_customShader);
    ClassDB::bind_method(D_METHOD("set_customShader", "value"), &FoliageDefinitionResource::set_customShader);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "customShader", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_customShader", "get_customShader");

    ClassDB::bind_method(D_METHOD("get_meshMaterial"), &FoliageDefinitionResource::get_meshMaterial);
    ClassDB::bind_method(D_METHOD("set_meshMaterial", "value"), &FoliageDefinitionResource::set_meshMaterial);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "meshMaterial", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_meshMaterial", "get_meshMaterial");

    ClassDB::bind_method(D_METHOD("get_maximumRenderDistance"), &FoliageDefinitionResource::get_maximumRenderDistance);
    ClassDB::bind_method(D_METHOD("set_maximumRenderDistance", "value"), &FoliageDefinitionResource::set_maximumRenderDistance);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "maximumRenderDistance"), "set_maximumRenderDistance", "get_maximumRenderDistance");

    ClassDB::bind_method(D_METHOD("get_editorMaximumRenderDistance"), &FoliageDefinitionResource::get_editorMaximumRenderDistance);
    ClassDB::bind_method(D_METHOD("set_editorMaximumRenderDistance", "value"), &FoliageDefinitionResource::set_editorMaximumRenderDistance);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "editorMaximumRenderDistance"), "set_editorMaximumRenderDistance", "get_editorMaximumRenderDistance");
}

FoliageDefinitionResource::FoliageDefinitionResource() {
    _strategy = Foliage::FoliageStrategy::MultiMesh;
    _meshScale = Vector3(1.0, 1.0, 1.0);
    _windStrength = 0.1;
    _visualInstanceLayers = 1;
    _lodLevels = 3;
    _lodRowsPerLevel = 50;
    _lodInitialCellWidth = 1;
    _albedo = Color::named("WHITE");
    _randomPlacementRange = 3.0;
    _maximumRenderDistance = 50;
    _editorMaximumRenderDistance = 50;
    _nearestTextureFilter = false;
}

FoliageDefinitionResource::~FoliageDefinitionResource() {}

void FoliageDefinitionResource::_validate_property(PropertyInfo &property) const {
    static TypedArray<StringName> multimeshProperties = {
        "lodLevels",
        "lodRowsPerLevel",
        "lodInitialCellWidth",
        "albedo",
        "albedoTextures",
        "nearestTextureFilter",
        "useGroundColor",
        "castShadow",
        "useBrushScale",
        "scaleNoiseTexture",
        "randomPlacementRange",
        "customShader",
    };

    static TypedArray<StringName> gpuParticlesProperties = {
        "meshMaterial",
        "maximumRenderDistance",
        "editorMaximumRenderDistance",
    };

    if (_strategy == Foliage::FoliageStrategy::MultiMesh) {
        if (gpuParticlesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_NO_EDITOR;
        }
        else if (multimeshProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_DEFAULT;
        }
    }
    else if (_strategy == Foliage::FoliageStrategy::GPUParticle) {
        if (gpuParticlesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_DEFAULT;
        }
        else if (multimeshProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_NO_EDITOR;
        }
    }
}

Foliage::FoliageStrategy FoliageDefinitionResource::get_strategy() const {
    return _strategy;
}
void FoliageDefinitionResource::set_strategy(const Foliage::FoliageStrategy value) {
    _strategy = value;
    notify_property_list_changed();
}

Ref<Mesh> FoliageDefinitionResource::get_mesh() const {
    return _mesh;
}
void FoliageDefinitionResource::set_mesh(const Ref<Mesh> &value) {
    _mesh = value;
}

Vector3 FoliageDefinitionResource::get_meshScale() const {
    return _meshScale;
}
void FoliageDefinitionResource::set_meshScale(const Vector3 value) {
    _meshScale = value;
}

float FoliageDefinitionResource::get_windStrength() const {
    return _windStrength;
}
void FoliageDefinitionResource::set_windStrength(const float value) {
    _windStrength = value;
}

Ref<Texture2D> FoliageDefinitionResource::get_noiseTexture() const {
    return _noiseTexture;
}
void FoliageDefinitionResource::set_noiseTexture(const Ref<Texture2D> &value) {
    _noiseTexture = value;
}

int FoliageDefinitionResource::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void FoliageDefinitionResource::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

// _multimesh settings
int FoliageDefinitionResource::get_lodLevels() const {
    return _lodLevels;
}
void FoliageDefinitionResource::set_lodLevels(const int value) {
    _lodLevels = value;
}

int FoliageDefinitionResource::get_lodRowsPerLevel() const {
    return _lodRowsPerLevel;
}
void FoliageDefinitionResource::set_lodRowsPerLevel(const int value) {
    _lodRowsPerLevel = value;
}

float FoliageDefinitionResource::get_lodInitialCellWidth() const {
    return _lodInitialCellWidth;
}
void FoliageDefinitionResource::set_lodInitialCellWidth(const float value) {
    _lodInitialCellWidth = value;
}

Color FoliageDefinitionResource::get_albedo() const {
    return _albedo;
}
void FoliageDefinitionResource::set_albedo(const Color value) {
    _albedo = value;
}

TypedArray<Ref<Texture2D>> FoliageDefinitionResource::get_albedoTextures() const {
    return _albedoTextures;
}
void FoliageDefinitionResource::set_albedoTextures(const TypedArray<Ref<Texture2D>> value) {
    _albedoTextures = value;
}

bool FoliageDefinitionResource::get_nearestTextureFilter() const {
    return _nearestTextureFilter;
}
void FoliageDefinitionResource::set_nearestTextureFilter(const bool value) {
    _nearestTextureFilter = value;
}

bool FoliageDefinitionResource::get_useGroundColor() const {
    return _useGroundColor;
}
void FoliageDefinitionResource::set_useGroundColor(const bool value) {
    _useGroundColor = value;
}

bool FoliageDefinitionResource::get_castShadow() const {
    return _castShadow;
}
void FoliageDefinitionResource::set_castShadow(const bool value) {
    _castShadow = value;
}

bool FoliageDefinitionResource::get_useBrushScale() const {
    return _useBrushScale;
}
void FoliageDefinitionResource::set_useBrushScale(const bool value) {
    _useBrushScale = value;
}

Ref<Texture2D> FoliageDefinitionResource::get_scaleNoiseTexture() const {
    return _scaleNoiseTexture;
}
void FoliageDefinitionResource::set_scaleNoiseTexture(const Ref<Texture2D> &value) {
    _scaleNoiseTexture = value;
}

float FoliageDefinitionResource::get_randomPlacementRange() const {
    return _randomPlacementRange;
}
void FoliageDefinitionResource::set_randomPlacementRange(const float value) {
    _randomPlacementRange = value;
}

TypedArray<int> FoliageDefinitionResource::get_applyOnTextureIndexes() const {
    return _applyOnTextureIndexes;
}
void FoliageDefinitionResource::set_applyOnTextureIndexes(const TypedArray<int> value) {
    _applyOnTextureIndexes = value;
}

Ref<ShaderMaterial> FoliageDefinitionResource::get_customShader() const {
    return _customShader;
}
void FoliageDefinitionResource::set_customShader(const Ref<ShaderMaterial> &value) {
    _customShader = value;

    if (!value.is_null() && value->get_shader().is_null()) {
        Ref<Shader> defaultShader = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader");
        auto defaultCode = defaultShader->get_code();

        Ref<Shader> newShader = memnew(Shader);
        newShader->set_code(defaultCode);
        value->set_shader(newShader);
    }
}

// _particles settings
Ref<Material> FoliageDefinitionResource::get_meshMaterial() const {
    return _meshMaterial;
}
void FoliageDefinitionResource::set_meshMaterial(const Ref<Material> &value) {
    _meshMaterial = value;
}

int FoliageDefinitionResource::get_maximumRenderDistance() const {
    return _maximumRenderDistance;
}
void FoliageDefinitionResource::set_maximumRenderDistance(const int value) {
    _maximumRenderDistance = value;
}

int FoliageDefinitionResource::get_editorMaximumRenderDistance() const {
    return _editorMaximumRenderDistance;
}
void FoliageDefinitionResource::set_editorMaximumRenderDistance(const int value) {
    _editorMaximumRenderDistance = value;
}