using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

public enum ObjectStrategy {
    PackedScenes = 1,
    OctreeMultiMeshes = 2
}

[Tool]
[GlobalClass]
public partial class ObjectDefinitionResource : Resource {
    private static readonly List<string> _packedScenesProperties = new List<string> {
    };

    private static readonly List<string> _octreeMultiMeshesProperties = new List<string> {
        nameof(LODList),
        nameof(LODMeshes),
        nameof(UpdateDistanceThreshold),
        nameof(UpdateTimeFrequency),
        nameof(VisualInstanceLayers),
    };

    private ObjectStrategy _strategy = ObjectStrategy.PackedScenes;

    [Export] public ObjectStrategy Strategy {
        get {
            return _strategy;
        }
        set {
            _strategy = value;
            NotifyPropertyListChanged();
        }
    }
    [Export] public int ObjectFrequency { get;set; } = -1;
    [Export] public float RandomRange { get;set; } = 1;
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public bool RandomYRotation { get;set; }
    [Export] public PackedScene[] ObjectScenes { get;set; }
    [Export] public ObjectOctreeLODDefinitionResource[] LODList { get;set;}
    [Export] public ObjectOctreeLODMeshesDefinitionResource[] LODMeshes { get;set;}
    [Export] public float UpdateDistanceThreshold { get;set; } = 1;
    [Export] public float UpdateTimeFrequency { get;set; } = 0.1f;
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    public override void _ValidateProperty(Dictionary property) {
        base._ValidateProperty(property);

        if (Strategy == ObjectStrategy.PackedScenes) {
            if (_octreeMultiMeshesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            } else if (_packedScenesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Default;
            }
        } else if (Strategy == ObjectStrategy.OctreeMultiMeshes) {
            if (_octreeMultiMeshesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.Default;
            } else if (_packedScenesProperties.Contains((string) property["name"])) {
                property["usage"] = (long) PropertyUsageFlags.NoEditor;
            }
        }
    }
}
