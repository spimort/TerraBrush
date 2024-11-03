using Godot;

namespace TerraBrush;

public enum ObjectStrategy {
    PackedScenes = 1,
    OctreeMultiMeshes = 2
}

[Tool]
[GlobalClass]
public partial class ObjectDefinitionResource : Resource {
    [Export] public ObjectStrategy Strategy { get;set; } = ObjectStrategy.PackedScenes;
    [Export] public int ObjectFrequency { get;set; } = -1;
    [Export] public float RandomRange { get;set; } = 1;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public bool RandomYRotation { get;set; }
    [Export] public PackedScene[] ObjectScenes { get;set; }
    [Export] public ObjectOctreeLODDefinitionResource[] LODList { get;set;}
    [Export] public ObjectOctreeLODMeshesDefinitionResource[] LODMeshes { get;set;}
    [Export] public float UpdateDistanceThreshold { get;set; } = 1;
    [Export] public float UpdateTimeFrequency { get;set; } = 1;
}
