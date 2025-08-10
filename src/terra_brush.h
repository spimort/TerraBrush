#ifndef TERRA_BRUSH_H
#define TERRA_BRUSH_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class TerraBrush : public Node3D {
    GDCLASS(TerraBrush, Node3D)

private:

protected:
    static void _bind_methods();

public:
    TerraBrush();
    ~TerraBrush();
};
#endif