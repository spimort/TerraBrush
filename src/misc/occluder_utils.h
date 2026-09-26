#ifndef OCCLUDER_UTILS_H
#define OCCLUDER_UTILS_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/array_occluder3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

using namespace godot;

class OccluderUtils {
    private:
        static void populateOccluderVerticesForNode(Node *node, Ref<SurfaceTool> &surfaceTool, Transform3D rootTransform);

    public:
        static Ref<ArrayOccluder3D> createOccluderFromNode(Node3D *node);
};
#endif
