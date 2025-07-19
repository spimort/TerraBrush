#nullable enable

namespace TerraBrush;

partial class KeyListenDialog
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
        public static global::Godot.StringName @KeyListenAccepted { get; } = global::Godot.StringName.CreateStaticFromAscii("KeyListenAccepted"u8);
        public static global::Godot.StringName @KeyListenCancelled { get; } = global::Godot.StringName.CreateStaticFromAscii("KeyListenCancelled"u8);
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new KeyListenDialog());
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@KeyListenAccepted)
        {
            Parameters =
            {
                new global::Godot.Bridge.ParameterInfo(global::Godot.StringName.CreateStaticFromAscii("key"u8), global::Godot.VariantType.Object)
                {
                    Hint = global::Godot.PropertyHint.ResourceType,
                    HintString = "InputEventKey",
                    Usage = global::Godot.PropertyUsageFlags.Default,
                    ClassName = global::Godot.StringName.CreateStaticFromAscii("InputEventKey"u8),
                },
            },
        });
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@KeyListenCancelled));
    }
}
