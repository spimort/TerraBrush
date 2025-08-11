#ifndef OBJECT_DEFINITION_RESOURCE_H
#define OBJECT_DEFINITION_RESOURCE_H

#include "../nodes/objects.h"
#include "object_octree_lod_definition_resource.h"
#include "object_octree_lod_meshes_definition_resource.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class ObjectDefinitionResource : public Resource {
    GDCLASS(ObjectDefinitionResource, Resource)

private:
    Objects::ObjectStrategy _strategy;
    int _objectFrequency;
    float _randomRange;
    Ref<Texture2D> _noiseTexture;
    bool _randomYRotation;
    bool _randomSize;
    float _randomSizeFactorMin;
    float _randomSizeFactorMax;
    TypedArray<Ref<PackedScene>> _objectScenes;
    TypedArray<Ref<ObjectOctreeLODDefinitionResource>> _lodList;
    TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>> _lodMeshes;
    float _updateDistanceThreshold;
    float _updateTimeFrequency;
    int _visualInstanceLayers;

protected:
    static void _bind_methods();
    void _validate_property(PropertyInfo &property) const;

public:
    ObjectDefinitionResource();
    ~ObjectDefinitionResource();

    Objects::ObjectStrategy get_strategy() const;
    void set_strategy(const Objects::ObjectStrategy value);

    int get_objectFrequency() const;
    void set_objectFrequency(const int value);

    float get_randomRange() const;
    void set_randomRange(const float value);

    Ref<Texture2D> get_noiseTexture() const;
    void set_noiseTexture(const Ref<Texture2D> &value);

    bool get_randomYRotation() const;
    void set_randomYRotation(const bool value);

    bool get_randomSize() const;
    void set_randomSize(const bool value);

    float get_randomSizeFactorMin() const;
    void set_randomSizeFactorMin(const float value);

    float get_randomSizeFactorMax() const;
    void set_randomSizeFactorMax(const float value);

    TypedArray<Ref<PackedScene>> get_objectScenes() const;
    void set_objectScenes(const TypedArray<Ref<PackedScene>> &value);

    TypedArray<Ref<ObjectOctreeLODDefinitionResource>> get_lodList() const;
    void set_lodList(const TypedArray<Ref<ObjectOctreeLODDefinitionResource>> &value);

    TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>> get_lodMeshes() const;
    void set_lodMeshes(const TypedArray<Ref<ObjectOctreeLODMeshesDefinitionResource>> &value);

    float get_updateDistanceThreshold() const;
    void set_updateDistanceThreshold(const float value);

    float get_updateTimeFrequency() const;
    void set_updateTimeFrequency(const float value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers(const int value);
};
#endif