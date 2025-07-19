#nullable enable

namespace TerraBrush;

partial class ObjectResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Definition { get; } = global::Godot.StringName.CreateStaticFromAscii("Definition"u8);
        public static global::Godot.StringName @Hide { get; } = global::Godot.StringName.CreateStaticFromAscii("Hide"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Definition, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ObjectDefinitionResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (ObjectResource __instance) =>
            {
                return __instance.@Definition;
            },
            static (ObjectResource __instance, global::TerraBrush.ObjectDefinitionResource value) =>
            {
                __instance.@Definition = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Hide, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectResource __instance) =>
            {
                return __instance.@Hide;
            },
            static (ObjectResource __instance, bool value) =>
            {
                __instance.@Hide = value;
            });
    }
}
