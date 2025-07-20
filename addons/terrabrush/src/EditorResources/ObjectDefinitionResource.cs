using System.Collections.Generic;
using Godot;
using Godot.Collections;

namespace TerraBrush;

public enum ObjectStrategy {
    PackedScenes = 1,
    OctreeMultiMeshes = 2
}

[GodotClass(Tool = true)]
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
    private bool _randomSize;

    [BindProperty]
    public ObjectStrategy Strategy {
        get {
            return _strategy;
        }
        set {
            _strategy = value;
            NotifyPropertyListChanged();
        }
    }
    [BindProperty] public int ObjectFrequency { get; set; } = -1;
    [BindProperty] public float RandomRange { get; set; } = 1;
    [BindProperty] public Texture2D NoiseTexture { get; set; }
    [BindProperty] public bool RandomYRotation { get; set; }
    [BindProperty]
    public bool RandomSize {
        get {
            return _randomSize;
        }
        set {
            _randomSize = value;
            NotifyPropertyListChanged();
        }
    }
    [BindProperty] public float RandomSizeFactorMin { get; set; } = 0.2f;
    [BindProperty] public float RandomSizeFactorMax { get; set; } = 1.2f;
    [BindProperty] public GodotArray<PackedScene> ObjectScenes { get; set; }
    [BindProperty] public GodotArray<ObjectOctreeLODDefinitionResource> LODList { get; set; }
    [BindProperty] public GodotArray<ObjectOctreeLODMeshesDefinitionResource> LODMeshes { get; set; }
    [BindProperty] public float UpdateDistanceThreshold { get; set; } = 1;
    [BindProperty] public float UpdateTimeFrequency { get; set; } = 0.1f;
    [BindProperty] public int VisualInstanceLayers { get; set; } = 1; // (PropertyHint.Layers3DRender)

    // TODO
    // public override void _ValidateProperty(GodotDictionary property) {
    //     base._ValidateProperty(property);

    //     if (Strategy == ObjectStrategy.PackedScenes) {
    //         if (_octreeMultiMeshesProperties.Contains((string) property["name"])) {
    //             property["usage"] = (long) PropertyUsageFlags.NoEditor;
    //         } else if (_packedScenesProperties.Contains((string) property["name"])) {
    //             property["usage"] = (long) PropertyUsageFlags.Default;
    //         }
    //     } else if (Strategy == ObjectStrategy.OctreeMultiMeshes) {
    //         if (_octreeMultiMeshesProperties.Contains((string) property["name"])) {
    //             property["usage"] = (long) PropertyUsageFlags.Default;
    //         } else if (_packedScenesProperties.Contains((string) property["name"])) {
    //             property["usage"] = (long) PropertyUsageFlags.NoEditor;
    //         }
    //     }

    //     if ((string)property["name"] == nameof(RandomSizeFactorMin) || (string)property["name"] == nameof(RandomSizeFactorMax)) {
    //         property["usage"] = (long) (RandomSize ? PropertyUsageFlags.Default : PropertyUsageFlags.NoEditor);
    //     }
    // }
}
