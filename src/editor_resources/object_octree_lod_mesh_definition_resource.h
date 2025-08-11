#ifndef OBJECT_OCTREE_LOD_MESH_DEFINITION_RESOURCE_H
#define OBJECT_OCTREE_LOD_MESH_DEFINITION_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/geometry_instance3d.hpp>

using namespace godot;

class ObjectOctreeLODMeshDefinitionResource : public Resource {
    GDCLASS(ObjectOctreeLODMeshDefinitionResource, Resource)

private:
    Ref<Mesh> _mesh;
    Ref<Material> _materialOverride;
    Vector3 _scale;
    GeometryInstance3D::ShadowCastingSetting _castShadow;

protected:
    static void _bind_methods();

public:
    ObjectOctreeLODMeshDefinitionResource();
    ~ObjectOctreeLODMeshDefinitionResource();

    Ref<Mesh> get_mesh() const;
    void set_mesh(const Ref<Mesh> &value);

    Ref<Material> get_materialOverride() const;
    void set_materialOverride(const Ref<Material> &value);

    Vector3 get_scale() const;
    void set_scale(const Vector3 value);

    GeometryInstance3D::ShadowCastingSetting get_castShadow() const;
    void set_castShadow(const GeometryInstance3D::ShadowCastingSetting value);
};
#endif