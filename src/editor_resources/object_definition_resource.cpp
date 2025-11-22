#include "object_definition_resource.h"

using namespace godot;

void ObjectDefinitionResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_strategy"), &ObjectDefinitionResource::get_strategy);
    ClassDB::bind_method(D_METHOD("set_strategy", "value"), &ObjectDefinitionResource::set_strategy);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "strategy", PROPERTY_HINT_ENUM, "PackedScenes:1,OctreeMultiMeshes:2"), "set_strategy", "get_strategy");

    ClassDB::bind_method(D_METHOD("get_objectFrequency"), &ObjectDefinitionResource::get_objectFrequency);
    ClassDB::bind_method(D_METHOD("set_objectFrequency", "value"), &ObjectDefinitionResource::set_objectFrequency);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "objectFrequency"), "set_objectFrequency", "get_objectFrequency");

    ClassDB::bind_method(D_METHOD("get_randomRange"), &ObjectDefinitionResource::get_randomRange);
    ClassDB::bind_method(D_METHOD("set_randomRange", "value"), &ObjectDefinitionResource::set_randomRange);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomRange"), "set_randomRange", "get_randomRange");

    ClassDB::bind_method(D_METHOD("get_noiseTexture"), &ObjectDefinitionResource::get_noiseTexture);
    ClassDB::bind_method(D_METHOD("set_noiseTexture", "value"), &ObjectDefinitionResource::set_noiseTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noiseTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_noiseTexture", "get_noiseTexture");

    ClassDB::bind_method(D_METHOD("get_randomYRotation"), &ObjectDefinitionResource::get_randomYRotation);
    ClassDB::bind_method(D_METHOD("set_randomYRotation", "value"), &ObjectDefinitionResource::set_randomYRotation);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "randomYRotation"), "set_randomYRotation", "get_randomYRotation");

    ClassDB::bind_method(D_METHOD("get_randomSize"), &ObjectDefinitionResource::get_randomSize);
    ClassDB::bind_method(D_METHOD("set_randomSize", "value"), &ObjectDefinitionResource::set_randomSize);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "randomSize"), "set_randomSize", "get_randomSize");

    ClassDB::bind_method(D_METHOD("get_randomSizeFactorMin"), &ObjectDefinitionResource::get_randomSizeFactorMin);
    ClassDB::bind_method(D_METHOD("set_randomSizeFactorMin", "value"), &ObjectDefinitionResource::set_randomSizeFactorMin);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomSizeFactorMin"), "set_randomSizeFactorMin", "get_randomSizeFactorMin");

    ClassDB::bind_method(D_METHOD("get_randomSizeFactorMax"), &ObjectDefinitionResource::get_randomSizeFactorMax);
    ClassDB::bind_method(D_METHOD("set_randomSizeFactorMax", "value"), &ObjectDefinitionResource::set_randomSizeFactorMax);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "randomSizeFactorMax"), "set_randomSizeFactorMax", "get_randomSizeFactorMax");

    ClassDB::bind_method(D_METHOD("get_objectScenes"), &ObjectDefinitionResource::get_objectScenes);
    ClassDB::bind_method(D_METHOD("set_objectScenes", "value"), &ObjectDefinitionResource::set_objectScenes);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "objectScenes", PROPERTY_HINT_TYPE_STRING, "24/17:PackedScene"), "set_objectScenes", "get_objectScenes");

    ClassDB::bind_method(D_METHOD("get_lodList"), &ObjectDefinitionResource::get_lodList);
    ClassDB::bind_method(D_METHOD("set_lodList", "value"), &ObjectDefinitionResource::set_lodList);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lodList", PROPERTY_HINT_TYPE_STRING, "24/17:ObjectOctreeLODDefinitionResource"), "set_lodList", "get_lodList");

    ClassDB::bind_method(D_METHOD("get_lodMeshes"), &ObjectDefinitionResource::get_lodMeshes);
    ClassDB::bind_method(D_METHOD("set_lodMeshes", "value"), &ObjectDefinitionResource::set_lodMeshes);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lodMeshes", PROPERTY_HINT_TYPE_STRING, "24/17:ObjectOctreeLODMeshesDefinitionResource"), "set_lodMeshes", "get_lodMeshes");

    ClassDB::bind_method(D_METHOD("get_updateDistanceThreshold"), &ObjectDefinitionResource::get_updateDistanceThreshold);
    ClassDB::bind_method(D_METHOD("set_updateDistanceThreshold", "value"), &ObjectDefinitionResource::set_updateDistanceThreshold);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "updateDistanceThreshold"), "set_updateDistanceThreshold", "get_updateDistanceThreshold");

    ClassDB::bind_method(D_METHOD("get_updateTimeFrequency"), &ObjectDefinitionResource::get_updateTimeFrequency);
    ClassDB::bind_method(D_METHOD("set_updateTimeFrequency", "value"), &ObjectDefinitionResource::set_updateTimeFrequency);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "updateTimeFrequency"), "set_updateTimeFrequency", "get_updateTimeFrequency");

    ClassDB::bind_method(D_METHOD("get_visualInstanceLayers"), &ObjectDefinitionResource::get_visualInstanceLayers);
    ClassDB::bind_method(D_METHOD("set_visualInstanceLayers", "value"), &ObjectDefinitionResource::set_visualInstanceLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visualInstanceLayers", PROPERTY_HINT_LAYERS_3D_RENDER), "set_visualInstanceLayers", "get_visualInstanceLayers");
}

ObjectDefinitionResource::ObjectDefinitionResource() {
    _strategy = ObjectStrategy::OBJECTSTRATEGY_PACKEDSCENES;
    _objectFrequency = -1;
    _randomRange = 1;
    _randomSize = false;
    _randomSizeFactorMin = 0.2;
    _randomSizeFactorMax = 1.2;
    _updateDistanceThreshold = 1;
    _updateTimeFrequency = 0.1;
    _visualInstanceLayers = 1;
    _randomYRotation = false;
    _noiseTexture = Ref<Texture2D>(nullptr);
    _objectScenes = TypedArray<Ref<PackedScene>>();
    _lodList = TypedArray<Ref<ObjectOctreeLODDefinitionResource>>();
    _lodMeshes = TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>>();
}

ObjectDefinitionResource::~ObjectDefinitionResource() {}

void ObjectDefinitionResource::_validate_property(PropertyInfo &property) const {
    static const TypedArray<StringName> PackedScenesProperties = {
    };

    static const TypedArray<StringName> OctreeMultiMeshesProperties = {
        "lodList",
        "lodMeshes",
        "updateDistanceThreshold",
        "updateTimeFrequency",
        "visualInstanceLayers",
    };

    if (_strategy == ObjectStrategy::OBJECTSTRATEGY_PACKEDSCENES) {
        if (OctreeMultiMeshesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_NO_EDITOR;
        } else if (PackedScenesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_DEFAULT;
        }
    } else if (_strategy == ObjectStrategy::OBJECTSTRATEGY_OCTREEMULTIMESHES) {
        if (OctreeMultiMeshesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_DEFAULT;
        } else if (PackedScenesProperties.has(property.name)) {
            property.usage = PROPERTY_USAGE_NO_EDITOR;
        }
    }

    if (property.name == StringName("randomSizeFactorMin") || property.name == StringName("randomSizeFactorMax")) {
        property.usage = (_randomSize ? PROPERTY_USAGE_DEFAULT : PROPERTY_USAGE_NO_EDITOR);
    }
}

ObjectStrategy ObjectDefinitionResource::get_strategy() const {
    return _strategy;
}
void ObjectDefinitionResource::set_strategy(const ObjectStrategy value) {
    _strategy = value;
    notify_property_list_changed();
}

int ObjectDefinitionResource::get_objectFrequency() const {
    return _objectFrequency;
}
void ObjectDefinitionResource::set_objectFrequency(const int value) {
    _objectFrequency = value;
}

float ObjectDefinitionResource::get_randomRange() const {
    return _randomRange;
}
void ObjectDefinitionResource::set_randomRange(const float value) {
    _randomRange = value;
}

Ref<Texture2D> ObjectDefinitionResource::get_noiseTexture() const {
    return _noiseTexture;
}
void ObjectDefinitionResource::set_noiseTexture(const Ref<Texture2D> &value) {
    _noiseTexture = value;
}

bool ObjectDefinitionResource::get_randomYRotation() const {
    return _randomYRotation;
}
void ObjectDefinitionResource::set_randomYRotation(const bool value) {
    _randomYRotation = value;
}

bool ObjectDefinitionResource::get_randomSize() const {
    return _randomSize;
}
void ObjectDefinitionResource::set_randomSize(const bool value) {
    _randomSize = value;
    notify_property_list_changed();
}

float ObjectDefinitionResource::get_randomSizeFactorMin() const {
    return _randomSizeFactorMin;
}
void ObjectDefinitionResource::set_randomSizeFactorMin(const float value) {
    _randomSizeFactorMin = value;
}

float ObjectDefinitionResource::get_randomSizeFactorMax() const {
    return _randomSizeFactorMax;
}
void ObjectDefinitionResource::set_randomSizeFactorMax(const float value) {
    _randomSizeFactorMax = value;
}

TypedArray<Ref<PackedScene>> ObjectDefinitionResource::get_objectScenes() const {
    return _objectScenes;
}
void ObjectDefinitionResource::set_objectScenes(const TypedArray<Ref<PackedScene>> &value) {
    _objectScenes = value;
}

TypedArray<Ref<ObjectOctreeLODDefinitionResource>> ObjectDefinitionResource::get_lodList() const {
    return _lodList;
}
void ObjectDefinitionResource::set_lodList(const TypedArray<Ref<ObjectOctreeLODDefinitionResource>> &value) {
    _lodList = value;
}

TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>> ObjectDefinitionResource::get_lodMeshes() const {
    return _lodMeshes;
}
void ObjectDefinitionResource::set_lodMeshes(const TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>> &value) {
    _lodMeshes = value;
}

float ObjectDefinitionResource::get_updateDistanceThreshold() const {
    return _updateDistanceThreshold;
}
void ObjectDefinitionResource::set_updateDistanceThreshold(const float value) {
    _updateDistanceThreshold = value;
}

float ObjectDefinitionResource::get_updateTimeFrequency() const {
    return _updateTimeFrequency;
}
void ObjectDefinitionResource::set_updateTimeFrequency(const float value) {
    _updateTimeFrequency = value;
}

int ObjectDefinitionResource::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void ObjectDefinitionResource::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}