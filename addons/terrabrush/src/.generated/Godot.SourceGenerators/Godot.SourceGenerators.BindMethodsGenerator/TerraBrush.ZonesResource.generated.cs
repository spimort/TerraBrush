#nullable enable

namespace TerraBrush;

partial class ZonesResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Zones { get; } = global::Godot.StringName.CreateStaticFromAscii("Zones"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ZonesResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Zones, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ZoneResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ZonesResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::TerraBrush.ZoneResource>)([.. __instance.@Zones]);
            },
            static (ZonesResource __instance, global::Godot.Collections.GodotArray<global::TerraBrush.ZoneResource> value) =>
            {
                __instance.@Zones = [.. value];
            });
    }
}
