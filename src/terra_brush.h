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
    enum TerrainToolType {
        None = 0,
        TerrainAdd = 1,
        TerrainRemove = 2,
        TerrainSmooth = 3,
        TerrainFlatten = 4,
        TerrainSetHeight = 5,
        TerrainSetAngle = 19,
        Paint = 6,
        FoliageAdd = 7,
        FoliageRemove = 8,
        ObjectAdd = 9,
        ObjectRemove = 10,
        WaterAdd = 11,
        WaterRemove = 12,
        WaterFlowAdd = 13,
        WaterFlowRemove = 14,
        SnowAdd = 15,
        SnowRemove = 16,
        HoleAdd = 17,
        HoleRemove = 18,
        LockAdd = 20,
        LockRemove = 21,
        MetaInfoAdd = 22,
        MetaInfoRemove = 23,
    };

    TerraBrush();
    ~TerraBrush();
};
#endif