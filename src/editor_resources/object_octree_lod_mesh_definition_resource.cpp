#include "object_octree_lod_mesh_definition_resource.h"

using namespace godot;

void ObjectOctreeLODMeshDefinitionResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_mesh"), &ObjectOctreeLODMeshDefinitionResource::get_mesh);
    ClassDB::bind_method(D_METHOD("set_mesh", "value"), &ObjectOctreeLODMeshDefinitionResource::set_mesh);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");

    ClassDB::bind_method(D_METHOD("get_materialOverride"), &ObjectOctreeLODMeshDefinitionResource::get_materialOverride);
    ClassDB::bind_method(D_METHOD("set_materialOverride", "value"), &ObjectOctreeLODMeshDefinitionResource::set_materialOverride);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "materialOverride", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_materialOverride", "get_materialOverride");

    ClassDB::bind_method(D_METHOD("get_scale"), &ObjectOctreeLODMeshDefinitionResource::get_scale);
    ClassDB::bind_method(D_METHOD("set_scale", "value"), &ObjectOctreeLODMeshDefinitionResource::set_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale", PROPERTY_HINT_LINK), "set_scale", "get_scale");

    ClassDB::bind_method(D_METHOD("get_castShadow"), &ObjectOctreeLODMeshDefinitionResource::get_castShadow);
    ClassDB::bind_method(D_METHOD("set_castShadow", "value"), &ObjectOctreeLODMeshDefinitionResource::set_castShadow);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "castShadow", PROPERTY_HINT_ENUM, "Off,On,DoubleSided,ShadowsOnly"), "set_castShadow", "get_castShadow");
}

ObjectOctreeLODMeshDefinitionResource::ObjectOctreeLODMeshDefinitionResource() {
    _scale = Vector3(1, 1, 1);
    _castShadow = GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_ON;
    _mesh = Ref<Mesh>(nullptr);
    _materialOverride = Ref<Material>(nullptr);
}

ObjectOctreeLODMeshDefinitionResource::~ObjectOctreeLODMeshDefinitionResource() {}

Ref<Mesh> ObjectOctreeLODMeshDefinitionResource::get_mesh() const {
    return _mesh;
}
void ObjectOctreeLODMeshDefinitionResource::set_mesh(const Ref<Mesh> &value) {
    _mesh = value;
}

Ref<Material> ObjectOctreeLODMeshDefinitionResource::get_materialOverride() const {
    return _materialOverride;
}
void ObjectOctreeLODMeshDefinitionResource::set_materialOverride(const Ref<Material> &value) {
    _materialOverride = value;
}

Vector3 ObjectOctreeLODMeshDefinitionResource::get_scale() const {
    return _scale;
}
void ObjectOctreeLODMeshDefinitionResource::set_scale(const Vector3 value) {
    _scale = value;
}

GeometryInstance3D::ShadowCastingSetting ObjectOctreeLODMeshDefinitionResource::get_castShadow() const {
    return _castShadow;
}
void ObjectOctreeLODMeshDefinitionResource::set_castShadow(const GeometryInstance3D::ShadowCastingSetting value) {
    _castShadow = value;
}