namespace TerraBrush;

public enum FoliageStrategy {
    MultiMesh = 1,
    GPUParticle = 2
}

public enum ObjectStrategy {
    PackedScenes = 1,
    OctreeMultiMeshes = 2
}

public enum AlphaChannelUsage {
    None = 0,
    Roughness = 1,
    Height = 2
}

public enum ObjectLoadingStrategy {
    ThreadedInEditorOnly = 1,
    Threaded = 2,
    NotThreaded = 3
}

public enum TerrainToolType {
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
    ColorAdd = 24,
    ColorRemove = 25,
}