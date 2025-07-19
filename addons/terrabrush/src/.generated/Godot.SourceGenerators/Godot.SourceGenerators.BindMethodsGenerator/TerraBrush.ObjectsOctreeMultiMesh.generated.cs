#nullable enable

namespace TerraBrush;

partial class ObjectsOctreeMultiMesh
{
    public new partial class MethodName : global::Godot.Node3D.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Node3D.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Node3D.PropertyName
    {
        public static global::Godot.StringName @ObjectsIndex { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectsIndex"u8);
        public static global::Godot.StringName @Definition { get; } = global::Godot.StringName.CreateStaticFromAscii("Definition"u8);
        public static global::Godot.StringName @TerrainZones { get; } = global::Godot.StringName.CreateStaticFromAscii("TerrainZones"u8);
        public static global::Godot.StringName @ZonesSize { get; } = global::Godot.StringName.CreateStaticFromAscii("ZonesSize"u8);
        public static global::Godot.StringName @Resolution { get; } = global::Godot.StringName.CreateStaticFromAscii("Resolution"u8);
        public static global::Godot.StringName @WaterFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFactor"u8);
        public static global::Godot.StringName @LoadInThread { get; } = global::Godot.StringName.CreateStaticFromAscii("LoadInThread"u8);
        public static global::Godot.StringName @DefaultObjectFrequency { get; } = global::Godot.StringName.CreateStaticFromAscii("DefaultObjectFrequency"u8);
    }
    public new partial class SignalName : global::Godot.Node3D.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectsOctreeMultiMesh());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectsIndex, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@ObjectsIndex;
            },
            static (ObjectsOctreeMultiMesh __instance, int value) =>
            {
                __instance.@ObjectsIndex = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Definition, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ObjectDefinitionResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@Definition;
            },
            static (ObjectsOctreeMultiMesh __instance, global::TerraBrush.ObjectDefinitionResource value) =>
            {
                __instance.@Definition = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TerrainZones, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ZonesResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (ObjectsOctreeMultiMesh __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (ObjectsOctreeMultiMesh __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@Resolution;
            },
            static (ObjectsOctreeMultiMesh __instance, int value) =>
            {
                __instance.@Resolution = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@WaterFactor;
            },
            static (ObjectsOctreeMultiMesh __instance, float value) =>
            {
                __instance.@WaterFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LoadInThread, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@LoadInThread;
            },
            static (ObjectsOctreeMultiMesh __instance, bool value) =>
            {
                __instance.@LoadInThread = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@DefaultObjectFrequency, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectsOctreeMultiMesh __instance) =>
            {
                return __instance.@DefaultObjectFrequency;
            },
            static (ObjectsOctreeMultiMesh __instance, int value) =>
            {
                __instance.@DefaultObjectFrequency = value;
            });
    }
}
