#nullable enable

namespace TerraBrush;

partial class ImportDialog
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
        public static global::Godot.StringName @Accepted { get; } = global::Godot.StringName.CreateStaticFromAscii("Accepted"u8);
        public static global::Godot.StringName @Cancelled { get; } = global::Godot.StringName.CreateStaticFromAscii("Cancelled"u8);
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ImportDialog());
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@Accepted)
        {
            Parameters =
            {
                new global::Godot.Bridge.ParameterInfo(global::Godot.StringName.CreateStaticFromAscii("settings"u8), global::Godot.VariantType.Object)
                {
                    Usage = global::Godot.PropertyUsageFlags.Default,
                    ClassName = global::Godot.StringName.CreateStaticFromAscii("Object"u8),
                },
            },
        });
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@Cancelled));
    }
}
