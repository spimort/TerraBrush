#include "object_octree_lod_definition_resource.h"

using namespace godot;

void ObjectOctreeLODDefinitionResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_maxDistance"), &ObjectOctreeLODDefinitionResource::get_maxDistance);
    ClassDB::bind_method(D_METHOD("set_maxDistance", "value"), &ObjectOctreeLODDefinitionResource::set_maxDistance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maxDistance"), "set_maxDistance", "get_maxDistance");

    ClassDB::bind_method(D_METHOD("get_addCollision"), &ObjectOctreeLODDefinitionResource::get_addCollision);
    ClassDB::bind_method(D_METHOD("set_addCollision", "value"), &ObjectOctreeLODDefinitionResource::set_addCollision);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "addCollision"), "set_addCollision", "get_addCollision");

    ClassDB::bind_method(D_METHOD("get_automaticLOD"), &ObjectOctreeLODDefinitionResource::get_automaticLOD);
    ClassDB::bind_method(D_METHOD("set_automaticLOD", "value"), &ObjectOctreeLODDefinitionResource::set_automaticLOD);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "automaticLOD"), "set_automaticLOD", "get_automaticLOD");

    ClassDB::bind_method(D_METHOD("get_automaticLODTargetVertices"), &ObjectOctreeLODDefinitionResource::get_automaticLODTargetVertices);
    ClassDB::bind_method(D_METHOD("set_automaticLODTargetVertices", "value"), &ObjectOctreeLODDefinitionResource::set_automaticLODTargetVertices);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "automaticLODTargetVertices"), "set_automaticLODTargetVertices", "get_automaticLODTargetVertices");

    ClassDB::bind_method(D_METHOD("get_objectFrequency"), &ObjectOctreeLODDefinitionResource::get_objectFrequency);
    ClassDB::bind_method(D_METHOD("set_objectFrequency", "value"), &ObjectOctreeLODDefinitionResource::set_objectFrequency);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "objectFrequency"), "set_objectFrequency", "get_objectFrequency");
}

ObjectOctreeLODDefinitionResource::ObjectOctreeLODDefinitionResource() {
    _automaticLODTargetVertices = -1;
    _objectFrequency = -1;
    _addCollision = false;
}

ObjectOctreeLODDefinitionResource::~ObjectOctreeLODDefinitionResource() {}

float ObjectOctreeLODDefinitionResource::get_maxDistance() const {
    return _maxDistance;
}
void ObjectOctreeLODDefinitionResource::set_maxDistance(const float value) {
    _maxDistance = value;
}

bool ObjectOctreeLODDefinitionResource::get_addCollision() const {
    return _addCollision;
}
void ObjectOctreeLODDefinitionResource::set_addCollision(const bool value) {
    _addCollision = value;
}

bool ObjectOctreeLODDefinitionResource::get_automaticLOD() const {
    return _automaticLOD;
}
void ObjectOctreeLODDefinitionResource::set_automaticLOD(const bool value) {
    _automaticLOD = value;
}

int ObjectOctreeLODDefinitionResource::get_automaticLODTargetVertices() const {
    return _automaticLODTargetVertices;
}
void ObjectOctreeLODDefinitionResource::set_automaticLODTargetVertices(const int value) {
    _automaticLODTargetVertices = value;
}

int ObjectOctreeLODDefinitionResource::get_objectFrequency() const {
    return _objectFrequency;
}
void ObjectOctreeLODDefinitionResource::set_objectFrequency(const int value) {
    _objectFrequency = value;
}

