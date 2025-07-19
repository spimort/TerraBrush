#nullable enable

namespace TerraBrush;

partial class TerrainPositionInformation
{
    public new partial class MethodName : global::Godot.GodotObject.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.GodotObject.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.GodotObject.PropertyName
    {
        public static global::Godot.StringName @Textures { get; } = global::Godot.StringName.CreateStaticFromAscii("Textures"u8);
        public static global::Godot.StringName @WaterFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFactor"u8);
        public static global::Godot.StringName @WaterDeepness { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterDeepness"u8);
        public static global::Godot.StringName @SnowFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowFactor"u8);
        public static global::Godot.StringName @SnowHeight { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowHeight"u8);
        public static global::Godot.StringName @MetaInfoIndex { get; } = global::Godot.StringName.CreateStaticFromAscii("MetaInfoIndex"u8);
        public static global::Godot.StringName @MetaInfoName { get; } = global::Godot.StringName.CreateStaticFromAscii("MetaInfoName"u8);
    }
    public new partial class SignalName : global::Godot.GodotObject.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new TerrainPositionInformation());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Textures, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/0:",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::TerraBrush.TerrainPositionTextureInformation>)([.. __instance.@Textures]);
            },
            static (TerrainPositionInformation __instance, global::Godot.Collections.GodotArray<global::TerraBrush.TerrainPositionTextureInformation> value) =>
            {
                __instance.@Textures = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@WaterFactor;
            },
            static (TerrainPositionInformation __instance, float value) =>
            {
                __instance.@WaterFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterDeepness, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@WaterDeepness;
            },
            static (TerrainPositionInformation __instance, float value) =>
            {
                __instance.@WaterDeepness = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@SnowFactor;
            },
            static (TerrainPositionInformation __instance, float value) =>
            {
                __instance.@SnowFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowHeight, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@SnowHeight;
            },
            static (TerrainPositionInformation __instance, float value) =>
            {
                __instance.@SnowHeight = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MetaInfoIndex, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@MetaInfoIndex;
            },
            static (TerrainPositionInformation __instance, int value) =>
            {
                __instance.@MetaInfoIndex = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MetaInfoName, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionInformation __instance) =>
            {
                return __instance.@MetaInfoName;
            },
            static (TerrainPositionInformation __instance, string value) =>
            {
                __instance.@MetaInfoName = value;
            });
    }
}
