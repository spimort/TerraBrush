#include "objects_base.h"
#include "../misc/string_names.h"

using namespace godot;

void ObjectsBase::_bind_methods() {
    ADD_SIGNAL(MethodInfo(StringNames::ObjectUpdated(), PropertyInfo(Variant::INT, "index")));
}

ObjectsBase::ObjectsBase() {}

ObjectsBase::~ObjectsBase() {}

void ObjectsBase::set_objectsIndex(const int value) {
    _objectsIndex = value;
}

void ObjectsBase::set_definition(const Ref<ObjectDefinitionResource> &value) {
    _definition = value;
}

void ObjectsBase::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void ObjectsBase::set_zonesSize(const int value) {
    _zonesSize = value;
}

void ObjectsBase::set_resolution(const int value) {
    _resolution = value;
}

void ObjectsBase::set_waterFactor(const float value) {
    _waterFactor = value;
}

void ObjectsBase::set_loadInThread(const bool value) {
    _loadInThread = value;
}

void ObjectsBase::set_defaultObjectFrequency(const int value) {
    _defaultObjectFrequency = value;
}

void ObjectsBase::updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) {}
void ObjectsBase::updateMeshesFromTool() {}
void ObjectsBase::addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage) {}
