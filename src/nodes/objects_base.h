#ifndef OBJECTS_BASE_H
#define OBJECTS_BASE_H

#include "../editor_resources/zone_resource.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/image.hpp>

using namespace godot;

class ObjectsBase {
public:
    virtual void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones);
    virtual void updateMeshesFromTool();
    virtual void addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage);
};
#endif