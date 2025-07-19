#nullable enable

namespace TerraBrush;

partial class BrushNumericSelector
{
    public new partial class MethodName : global::Godot.Control.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Control.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Control.PropertyName
    {
        public static global::Godot.StringName @BrushSizeFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("BrushSizeFactor"u8);
        public static global::Godot.StringName @WidgetColor { get; } = global::Godot.StringName.CreateStaticFromAscii("WidgetColor"u8);
        public static global::Godot.StringName @MinValue { get; } = global::Godot.StringName.CreateStaticFromAscii("MinValue"u8);
        public static global::Godot.StringName @MaxValue { get; } = global::Godot.StringName.CreateStaticFromAscii("MaxValue"u8);
    }
    public new partial class SignalName : global::Godot.Control.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new BrushNumericSelector());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@BrushSizeFactor, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (BrushNumericSelector __instance) =>
            {
                return __instance.@BrushSizeFactor;
            },
            static (BrushNumericSelector __instance, int value) =>
            {
                __instance.@BrushSizeFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WidgetColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (BrushNumericSelector __instance) =>
            {
                return __instance.@WidgetColor;
            },
            static (BrushNumericSelector __instance, global::Godot.Color value) =>
            {
                __instance.@WidgetColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MinValue, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (BrushNumericSelector __instance) =>
            {
                return __instance.@MinValue;
            },
            static (BrushNumericSelector __instance, int value) =>
            {
                __instance.@MinValue = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MaxValue, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (BrushNumericSelector __instance) =>
            {
                return __instance.@MaxValue;
            },
            static (BrushNumericSelector __instance, int value) =>
            {
                __instance.@MaxValue = value;
            });
    }
}
