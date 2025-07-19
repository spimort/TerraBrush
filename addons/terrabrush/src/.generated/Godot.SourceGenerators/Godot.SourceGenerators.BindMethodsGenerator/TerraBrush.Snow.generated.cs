#nullable enable

namespace TerraBrush;

partial class Snow
{
    public new partial class MethodName : global::Godot.Node3D.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Node3D.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Node3D.PropertyName
    {
        public static global::Godot.StringName @ZonesSize { get; } = global::Godot.StringName.CreateStaticFromAscii("ZonesSize"u8);
        public static global::Godot.StringName @Resolution { get; } = global::Godot.StringName.CreateStaticFromAscii("Resolution"u8);
        public static global::Godot.StringName @TerrainZones { get; } = global::Godot.StringName.CreateStaticFromAscii("TerrainZones"u8);
        public static global::Godot.StringName @SnowDefinition { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowDefinition"u8);
        public static global::Godot.StringName @LODLevels { get; } = global::Godot.StringName.CreateStaticFromAscii("LODLevels"u8);
        public static global::Godot.StringName @LODRowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("LODRowsPerLevel"u8);
        public static global::Godot.StringName @LODInitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("LODInitialCellWidth"u8);
    }
    public new partial class SignalName : global::Godot.Node3D.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new Snow());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Snow __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (Snow __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Snow __instance) =>
            {
                return __instance.@Resolution;
            },
            static (Snow __instance, int value) =>
            {
                __instance.@Resolution = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TerrainZones, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ZonesResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (Snow __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (Snow __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowDefinition, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "SnowResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (Snow __instance) =>
            {
                return __instance.@SnowDefinition;
            },
            static (Snow __instance, global::TerraBrush.SnowResource value) =>
            {
                __instance.@SnowDefinition = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODLevels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Snow __instance) =>
            {
                return __instance.@LODLevels;
            },
            static (Snow __instance, int value) =>
            {
                __instance.@LODLevels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODRowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Snow __instance) =>
            {
                return __instance.@LODRowsPerLevel;
            },
            static (Snow __instance, int value) =>
            {
                __instance.@LODRowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODInitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Snow __instance) =>
            {
                return __instance.@LODInitialCellWidth;
            },
            static (Snow __instance, float value) =>
            {
                __instance.@LODInitialCellWidth = value;
            });
    }
}
