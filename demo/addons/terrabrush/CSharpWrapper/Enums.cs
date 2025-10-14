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