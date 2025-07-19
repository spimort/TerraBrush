#nullable enable

namespace TerraBrush;

partial class TextureSetsResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @TextureSets { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureSets"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new TextureSetsResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureSets, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:TextureSetResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TextureSetsResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::TerraBrush.TextureSetResource>)([.. __instance.@TextureSets]);
            },
            static (TextureSetsResource __instance, global::Godot.Collections.GodotArray<global::TerraBrush.TextureSetResource> value) =>
            {
                __instance.@TextureSets = [.. value];
            });
    }
}
