#ifndef FOLIAGE_H
#define FOLIAGE_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

enum FoliageStrategy {
    MultiMesh = 1,
    GPUParticle = 2
};

class Foliage : public Node3D {
    GDCLASS(Foliage, Node3D)

private:

protected:
    static void _bind_methods();

public:
    Foliage();
    ~Foliage();
};
#endif