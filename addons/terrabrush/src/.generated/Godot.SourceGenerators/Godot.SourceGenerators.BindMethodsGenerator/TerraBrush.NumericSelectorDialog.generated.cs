#nullable enable

namespace TerraBrush;

partial class NumericSelectorDialog
{
    public new partial class MethodName : global::Godot.Window.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Window.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Window.PropertyName
    {
    }
    public new partial class SignalName : global::Godot.Window.SignalName
    {
        public static global::Godot.StringName @NumericSelectorAccepted { get; } = global::Godot.StringName.CreateStaticFromAscii("NumericSelectorAccepted"u8);
        public static global::Godot.StringName @NumericSelectorCancelled { get; } = global::Godot.StringName.CreateStaticFromAscii("NumericSelectorCancelled"u8);
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new NumericSelectorDialog());
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@NumericSelectorAccepted)
        {
            Parameters =
            {
                new global::Godot.Bridge.ParameterInfo(global::Godot.StringName.CreateStaticFromAscii("value"u8), global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
                {
                    Usage = global::Godot.PropertyUsageFlags.Default,
                },
            },
        });
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@NumericSelectorCancelled));
    }
}
