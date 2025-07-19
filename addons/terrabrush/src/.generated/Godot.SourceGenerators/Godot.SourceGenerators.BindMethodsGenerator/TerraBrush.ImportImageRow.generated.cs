#nullable enable

namespace TerraBrush;

partial class ImportImageRow
{
    public new partial class MethodName : global::Godot.PanelContainer.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.PanelContainer.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.PanelContainer.PropertyName
    {
        public static global::Godot.StringName @ImageTypeName { get; } = global::Godot.StringName.CreateStaticFromAscii("ImageTypeName"u8);
        public static global::Godot.StringName @ImageTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("ImageTexture"u8);
    }
    public new partial class SignalName : global::Godot.PanelContainer.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ImportImageRow());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ImageTypeName, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ImportImageRow __instance) =>
            {
                return __instance.@ImageTypeName;
            },
            static (ImportImageRow __instance, string value) =>
            {
                __instance.@ImageTypeName = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ImageTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (ImportImageRow __instance) =>
            {
                return __instance.@ImageTexture;
            },
            static (ImportImageRow __instance, global::Godot.Texture2D value) =>
            {
                __instance.@ImageTexture = value;
            });
    }
}
