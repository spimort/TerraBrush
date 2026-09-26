#include "occluder_utils.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

using namespace godot;

void OccluderUtils::populateOccluderVerticesForNode(Node *node, Ref<SurfaceTool> &surfaceTool, Transform3D rootTransform) {
    MeshInstance3D *meshInstance = Object::cast_to<MeshInstance3D>(node);
	if (meshInstance != nullptr) {
        Ref<SurfaceTool> instanceSurfaceTool = memnew(SurfaceTool);

        Ref<Mesh> mesh = meshInstance->get_mesh();
        bool found = false;

        for (int i = 0; i < mesh->get_surface_count(); i++) {
            Ref<StandardMaterial3D> material = mesh->surface_get_material(i);
            if (material.is_null()) {
                continue;
            }

            if (material->get_transparency() != StandardMaterial3D::TRANSPARENCY_DISABLED) {
                continue;
            }

            Transform3D identity = Transform3D(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(0, 0, 0));
            instanceSurfaceTool->append_from(mesh, i, identity);
            found = true;
        }

        if (!found) {
            return;
        }

        Array arrays = instanceSurfaceTool->commit_to_arrays();
        PackedInt32Array simplified = instanceSurfaceTool->generate_lod(0.1f);
        arrays[Mesh::ARRAY_INDEX] = simplified;

        Ref<ArrayMesh> arrayMesh = memnew(ArrayMesh);
        arrayMesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

        for (int i = 0; i < arrayMesh->get_surface_count(); i++) {
            Transform3D meshLocalToRoot = rootTransform.affine_inverse() * meshInstance->get_global_transform();
            surfaceTool->append_from(arrayMesh, i, meshLocalToRoot);
        }
	}

	for (int i = 0; i < node->get_child_count(); i++) {
		Node *child = node->get_child(i);

		populateOccluderVerticesForNode(child, surfaceTool, rootTransform);
	}
}

Ref<ArrayOccluder3D> OccluderUtils::createOccluderFromNode(Node3D *node) {
    Ref<SurfaceTool> surfaceTool = memnew(SurfaceTool);

    populateOccluderVerticesForNode(node, surfaceTool,  node->get_global_transform());

    Array arrays = surfaceTool->commit_to_arrays();
    PackedVector3Array vertices = arrays[Mesh::ARRAY_VERTEX];
    PackedInt32Array indices = arrays[Mesh::ARRAY_INDEX];

    if (vertices.size() == 0) {
        return nullptr;
    }

    Ref<ArrayOccluder3D> arrayOccluder = memnew(ArrayOccluder3D);
    arrayOccluder->set_arrays(vertices, indices);

    return arrayOccluder;
}
