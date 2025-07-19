#nullable enable

namespace TerraBrush;

partial class PieMenu
{
    public new partial class MethodName : global::Godot.Control.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Control.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Control.PropertyName
    {
        public static global::Godot.StringName @ButtonsRadiusRatio { get; } = global::Godot.StringName.CreateStaticFromAscii("ButtonsRadiusRatio"u8);
        public static global::Godot.StringName @ButtonsMinRadius { get; } = global::Godot.StringName.CreateStaticFromAscii("ButtonsMinRadius"u8);
        public static global::Godot.StringName @Label { get; } = global::Godot.StringName.CreateStaticFromAscii("Label"u8);
    }
    public new partial class SignalName : global::Godot.Control.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new PieMenu());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ButtonsRadiusRatio, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (PieMenu __instance) =>
            {
                return __instance.@ButtonsRadiusRatio;
            },
            static (PieMenu __instance, int value) =>
            {
                __instance.@ButtonsRadiusRatio = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ButtonsMinRadius, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (PieMenu __instance) =>
            {
                return __instance.@ButtonsMinRadius;
            },
            static (PieMenu __instance, int value) =>
            {
                __instance.@ButtonsMinRadius = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Label, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (PieMenu __instance) =>
            {
                return __instance.@Label;
            },
            static (PieMenu __instance, string value) =>
            {
                __instance.@Label = value;
            });
    }
}
