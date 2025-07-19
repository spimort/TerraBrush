#nullable enable

namespace TerraBrush;

partial class MetaInfoLayer
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Name { get; } = global::Godot.StringName.CreateStaticFromAscii("Name"u8);
        public static global::Godot.StringName @Color { get; } = global::Godot.StringName.CreateStaticFromAscii("Color"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new MetaInfoLayer());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Name, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (MetaInfoLayer __instance) =>
            {
                return __instance.@Name;
            },
            static (MetaInfoLayer __instance, string value) =>
            {
                __instance.@Name = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Color, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (MetaInfoLayer __instance) =>
            {
                return __instance.@Color;
            },
            static (MetaInfoLayer __instance, global::Godot.Color value) =>
            {
                __instance.@Color = value;
            });
    }
}
