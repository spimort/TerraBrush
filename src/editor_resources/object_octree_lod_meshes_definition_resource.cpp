#include "object_octree_lod_meshes_definition_resource.h"

using namespace godot;

void ObjectOctreeLODMeshesDefinitionResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_meshes"), &ObjectOctreeLODMeshesDefinitionResource::get_meshes);
    ClassDB::bind_method(D_METHOD("set_meshes", "value"), &ObjectOctreeLODMeshesDefinitionResource::set_meshes);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "meshes", PROPERTY_HINT_TYPE_STRING, "24/17:ObjectOctreeLODMeshDefinitionResource"), "set_meshes", "get_meshes");

    ClassDB::bind_method(D_METHOD("get_collisionShape"), &ObjectOctreeLODMeshesDefinitionResource::get_collisionShape);
    ClassDB::bind_method(D_METHOD("set_collisionShape", "value"), &ObjectOctreeLODMeshesDefinitionResource::set_collisionShape);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collisionShape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_collisionShape", "get_collisionShape");
}

ObjectOctreeLODMeshesDefinitionResource::ObjectOctreeLODMeshesDefinitionResource() {}

ObjectOctreeLODMeshesDefinitionResource::~ObjectOctreeLODMeshesDefinitionResource() {}

TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>> ObjectOctreeLODMeshesDefinitionResource::get_meshes() const {
    return _meshes;
}
void ObjectOctreeLODMeshesDefinitionResource::set_meshes(const TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>> value) {
    _meshes = value;
}

Ref<Shape3D> ObjectOctreeLODMeshesDefinitionResource::get_collisionShape() const {
    return _collisionShape;
}
void ObjectOctreeLODMeshesDefinitionResource::set_collisionShape(const Ref<Shape3D> &value) {
    _collisionShape = value;
}