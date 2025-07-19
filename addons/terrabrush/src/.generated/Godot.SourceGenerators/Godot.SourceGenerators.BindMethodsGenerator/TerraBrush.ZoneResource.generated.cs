#nullable enable

namespace TerraBrush;

partial class ZoneResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @ZonePosition { get; } = global::Godot.StringName.CreateStaticFromAscii("ZonePosition"u8);
        public static global::Godot.StringName @HeightMapTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightMapTexture"u8);
        public static global::Godot.StringName @SplatmapsTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("SplatmapsTexture"u8);
        public static global::Godot.StringName @FoliagesTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("FoliagesTexture"u8);
        public static global::Godot.StringName @ObjectsTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectsTexture"u8);
        public static global::Godot.StringName @WaterTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterTexture"u8);
        public static global::Godot.StringName @SnowTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowTexture"u8);
        public static global::Godot.StringName @MetaInfoTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("MetaInfoTexture"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ZoneResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonePosition, global::Godot.VariantType.Vector2I)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ZoneResource __instance) =>
            {
                return __instance.@ZonePosition;
            },
            static (ZoneResource __instance, global::Godot.Vector2I value) =>
            {
                __instance.@ZonePosition = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightMapTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ImageTexture"u8),
            },
            static (ZoneResource __instance) =>
            {
                return __instance.@HeightMapTexture;
            },
            static (ZoneResource __instance, global::Godot.ImageTexture value) =>
            {
                __instance.@HeightMapTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SplatmapsTexture, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ZoneResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::Godot.ImageTexture>)([.. __instance.@SplatmapsTexture]);
            },
            static (ZoneResource __instance, global::Godot.Collections.GodotArray<global::Godot.ImageTexture> value) =>
            {
                __instance.@SplatmapsTexture = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@FoliagesTexture, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ZoneResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::Godot.ImageTexture>)([.. __instance.@FoliagesTexture]);
            },
            static (ZoneResource __instance, global::Godot.Collections.GodotArray<global::Godot.ImageTexture> value) =>
            {
                __instance.@FoliagesTexture = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectsTexture, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ZoneResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::Godot.ImageTexture>)([.. __instance.@ObjectsTexture]);
            },
            static (ZoneResource __instance, global::Godot.Collections.GodotArray<global::Godot.ImageTexture> value) =>
            {
                __instance.@ObjectsTexture = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ImageTexture"u8),
            },
            static (ZoneResource __instance) =>
            {
                return __instance.@WaterTexture;
            },
            static (ZoneResource __instance, global::Godot.ImageTexture value) =>
            {
                __instance.@WaterTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ImageTexture"u8),
            },
            static (ZoneResource __instance) =>
            {
                return __instance.@SnowTexture;
            },
            static (ZoneResource __instance, global::Godot.ImageTexture value) =>
            {
                __instance.@SnowTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MetaInfoTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ImageTexture",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ImageTexture"u8),
            },
            static (ZoneResource __instance) =>
            {
                return __instance.@MetaInfoTexture;
            },
            static (ZoneResource __instance, global::Godot.ImageTexture value) =>
            {
                __instance.@MetaInfoTexture = value;
            });
    }
}
