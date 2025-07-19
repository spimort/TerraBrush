#nullable enable

namespace TerraBrush;

partial class ToolInfo
{
    public new partial class MethodName : global::Godot.Control.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Control.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Control.PropertyName
    {
    }
    public new partial class SignalName : global::Godot.Control.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ToolInfo());
    }
}
