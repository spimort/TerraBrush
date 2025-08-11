#ifndef OBJECTS_H
#define OBJECTS_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class Objects : public Node3D {
    GDCLASS(Objects, Node3D)

private:

protected:
    static void _bind_methods();

public:
    enum ObjectStrategy {
        PackedScenes = 1,
        OctreeMultiMeshes = 2
    };

    Objects();
    ~Objects();
};

VARIANT_ENUM_CAST(Objects::ObjectStrategy);
#endif