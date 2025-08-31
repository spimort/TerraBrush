#ifndef OBJECT_OCTREE_LOD_MESHES_DEFINITION_RESOURCE
#define OBJECT_OCTREE_LOD_MESHES_DEFINITION_RESOURCE

#include "object_octree_lod_mesh_definition_resource.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/shape3d.hpp>

using namespace godot;

class ObjectOctreeLODMeshesDefinitionResource : public Resource {
    GDCLASS(ObjectOctreeLODMeshesDefinitionResource, Resource);

private:
    TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>> _meshes = TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>>();
    Ref<Shape3D> _collisionShape = nullptr;

protected:
    static void _bind_methods();

public:
    ObjectOctreeLODMeshesDefinitionResource();
    ~ObjectOctreeLODMeshesDefinitionResource();

    TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>> get_meshes() const;
    void set_meshes(const TypedArray<Ref<ObjectOctreeLODMeshDefinitionResource>> value);

    Ref<Shape3D> get_collisionShape() const;
    void set_collisionShape(const Ref<Shape3D> &value);
};
#endif