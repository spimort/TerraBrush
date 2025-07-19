#nullable enable

namespace TerraBrush;

partial class Plugin
{
    public new partial class MethodName : global::Godot.EditorPlugin.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.EditorPlugin.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.EditorPlugin.PropertyName
    {
    }
    public new partial class SignalName : global::Godot.EditorPlugin.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new Plugin());
    }
}
