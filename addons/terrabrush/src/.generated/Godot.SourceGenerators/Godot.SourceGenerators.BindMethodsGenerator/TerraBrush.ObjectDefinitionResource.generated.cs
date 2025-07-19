#nullable enable

namespace TerraBrush;

partial class ObjectDefinitionResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Strategy { get; } = global::Godot.StringName.CreateStaticFromAscii("Strategy"u8);
        public static global::Godot.StringName @ObjectFrequency { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectFrequency"u8);
        public static global::Godot.StringName @RandomRange { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomRange"u8);
        public static global::Godot.StringName @NoiseTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("NoiseTexture"u8);
        public static global::Godot.StringName @RandomYRotation { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomYRotation"u8);
        public static global::Godot.StringName @RandomSize { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomSize"u8);
        public static global::Godot.StringName @RandomSizeFactorMin { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomSizeFactorMin"u8);
        public static global::Godot.StringName @RandomSizeFactorMax { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomSizeFactorMax"u8);
        public static global::Godot.StringName @ObjectScenes { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectScenes"u8);
        public static global::Godot.StringName @LODList { get; } = global::Godot.StringName.CreateStaticFromAscii("LODList"u8);
        public static global::Godot.StringName @LODMeshes { get; } = global::Godot.StringName.CreateStaticFromAscii("LODMeshes"u8);
        public static global::Godot.StringName @UpdateDistanceThreshold { get; } = global::Godot.StringName.CreateStaticFromAscii("UpdateDistanceThreshold"u8);
        public static global::Godot.StringName @UpdateTimeFrequency { get; } = global::Godot.StringName.CreateStaticFromAscii("UpdateTimeFrequency"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectDefinitionResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Strategy, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "PackedScenes:1,OctreeMultiMeshes:2",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@Strategy;
            },
            static (ObjectDefinitionResource __instance, global::TerraBrush.ObjectStrategy value) =>
            {
                __instance.@Strategy = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectFrequency, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@ObjectFrequency;
            },
            static (ObjectDefinitionResource __instance, int value) =>
            {
                __instance.@ObjectFrequency = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomRange, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@RandomRange;
            },
            static (ObjectDefinitionResource __instance, float value) =>
            {
                __instance.@RandomRange = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NoiseTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@NoiseTexture;
            },
            static (ObjectDefinitionResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@NoiseTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomYRotation, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@RandomYRotation;
            },
            static (ObjectDefinitionResource __instance, bool value) =>
            {
                __instance.@RandomYRotation = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomSize, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@RandomSize;
            },
            static (ObjectDefinitionResource __instance, bool value) =>
            {
                __instance.@RandomSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomSizeFactorMin, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@RandomSizeFactorMin;
            },
            static (ObjectDefinitionResource __instance, float value) =>
            {
                __instance.@RandomSizeFactorMin = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomSizeFactorMax, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@RandomSizeFactorMax;
            },
            static (ObjectDefinitionResource __instance, float value) =>
            {
                __instance.@RandomSizeFactorMax = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectScenes, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:PackedScene",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@ObjectScenes;
            },
            static (ObjectDefinitionResource __instance, global::Godot.Collections.GodotArray<global::Godot.PackedScene> value) =>
            {
                __instance.@ObjectScenes = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODList, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ObjectOctreeLODDefinitionResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@LODList;
            },
            static (ObjectDefinitionResource __instance, global::Godot.Collections.GodotArray<global::TerraBrush.ObjectOctreeLODDefinitionResource> value) =>
            {
                __instance.@LODList = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODMeshes, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ObjectOctreeLODMeshesDefinitionResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@LODMeshes;
            },
            static (ObjectDefinitionResource __instance, global::Godot.Collections.GodotArray<global::TerraBrush.ObjectOctreeLODMeshesDefinitionResource> value) =>
            {
                __instance.@LODMeshes = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UpdateDistanceThreshold, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@UpdateDistanceThreshold;
            },
            static (ObjectDefinitionResource __instance, float value) =>
            {
                __instance.@UpdateDistanceThreshold = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UpdateTimeFrequency, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@UpdateTimeFrequency;
            },
            static (ObjectDefinitionResource __instance, float value) =>
            {
                __instance.@UpdateTimeFrequency = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectDefinitionResource __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (ObjectDefinitionResource __instance, int value) =>
            {
                __instance.@VisualInstanceLayers = value;
            });
    }
}
