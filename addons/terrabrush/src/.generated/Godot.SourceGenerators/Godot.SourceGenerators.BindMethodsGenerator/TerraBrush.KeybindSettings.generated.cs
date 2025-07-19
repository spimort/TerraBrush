#nullable enable

namespace TerraBrush;

partial class KeybindSettings
{
    public new partial class MethodName : global::Godot.AcceptDialog.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.AcceptDialog.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.AcceptDialog.PropertyName
    {
    }
    public new partial class SignalName : global::Godot.AcceptDialog.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new KeybindSettings());
    }
}
