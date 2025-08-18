#ifndef ENUMS_H
#define ENUMS_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

enum FoliageStrategy : int {
    MultiMesh = 1,
    GPUParticle = 2
};
VARIANT_ENUM_CAST(FoliageStrategy);

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
VARIANT_ENUM_CAST(TerrainToolType);

enum ObjectStrategy {
    PackedScenes = 1,
    OctreeMultiMeshes = 2
};
VARIANT_ENUM_CAST(ObjectStrategy);
#endif