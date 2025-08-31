#ifndef BRUSH_DECAL_H
#define BRUSH_DECAL_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/decal.hpp>
#include <godot_cpp/classes/image.hpp>

using namespace godot;

class BrushDecal : public Node3D {
    GDCLASS(BrushDecal, Node3D);

private:
    Decal *_decal = nullptr;

protected:
    static void _bind_methods();

public:
    BrushDecal();
    ~BrushDecal();

    void _ready() override;

    void setSize(const int size);
    void setBrushImage(const Ref<Image> image);
};
#endif