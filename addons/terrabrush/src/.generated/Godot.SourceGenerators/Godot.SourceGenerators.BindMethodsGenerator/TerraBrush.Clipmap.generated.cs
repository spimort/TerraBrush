#nullable enable

namespace TerraBrush;

partial class Clipmap
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
        public static global::Godot.StringName @Levels { get; } = global::Godot.StringName.CreateStaticFromAscii("Levels"u8);
        public static global::Godot.StringName @RowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("RowsPerLevel"u8);
        public static global::Godot.StringName @InitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("InitialCellWidth"u8);
        public static global::Godot.StringName @Shader { get; } = global::Godot.StringName.CreateStaticFromAscii("Shader"u8);
    }
    public new partial class SignalName : global::Godot.Node3D.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new Clipmap());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Clipmap __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (Clipmap __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Clipmap __instance) =>
            {
                return __instance.@Resolution;
            },
            static (Clipmap __instance, int value) =>
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
            static (Clipmap __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (Clipmap __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Levels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Clipmap __instance) =>
            {
                return __instance.@Levels;
            },
            static (Clipmap __instance, int value) =>
            {
                __instance.@Levels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Clipmap __instance) =>
            {
                return __instance.@RowsPerLevel;
            },
            static (Clipmap __instance, int value) =>
            {
                __instance.@RowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@InitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Clipmap __instance) =>
            {
                return __instance.@InitialCellWidth;
            },
            static (Clipmap __instance, float value) =>
            {
                __instance.@InitialCellWidth = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Shader, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ShaderMaterial"u8),
            },
            static (Clipmap __instance) =>
            {
                return __instance.@Shader;
            },
            static (Clipmap __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@Shader = value;
            });
    }
}
