#ifndef OBJECTS_BASE_H
#define OBJECTS_BASE_H

#include "../editor_resources/zone_resource.h"
#include "../editor_resources/zones_resource.h"
#include "../editor_resources/object_definition_resource.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/image.hpp>

using namespace godot;

class ObjectsBase: public Node3D {
    GDCLASS(ObjectsBase, Node3D);

protected:
    static void _bind_methods();

    int _objectsIndex;
    Ref<ObjectDefinitionResource> _definition;
    Ref<ZonesResource> _terrainZones;
    int _zonesSize;
    int _resolution;
    float _waterFactor;
    bool _loadInThread;
    int _defaultObjectFrequency;

public:
    ObjectsBase::ObjectsBase();
    ObjectsBase::~ObjectsBase();

    void set_objectsIndex(const int value);
    void set_definition(const Ref<ObjectDefinitionResource> &value);
    void set_terrainZones(const Ref<ZonesResource> &value);
    void set_zonesSize(const int value);
    void set_resolution(const int value);
    void set_waterFactor(const float value);
    void set_loadInThread(const bool value);
    void set_defaultObjectFrequency(const int value);

    virtual void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones);
    virtual void updateMeshesFromTool();
    virtual void addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage);
};
#endif