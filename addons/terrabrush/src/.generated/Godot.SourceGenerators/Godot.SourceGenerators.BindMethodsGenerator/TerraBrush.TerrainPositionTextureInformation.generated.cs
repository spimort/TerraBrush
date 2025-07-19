#nullable enable

namespace TerraBrush;

partial class TerrainPositionTextureInformation
{
    public new partial class MethodName : global::Godot.GodotObject.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.GodotObject.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.GodotObject.PropertyName
    {
        public static global::Godot.StringName @Index { get; } = global::Godot.StringName.CreateStaticFromAscii("Index"u8);
        public static global::Godot.StringName @Name { get; } = global::Godot.StringName.CreateStaticFromAscii("Name"u8);
        public static global::Godot.StringName @Factor { get; } = global::Godot.StringName.CreateStaticFromAscii("Factor"u8);
    }
    public new partial class SignalName : global::Godot.GodotObject.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new TerrainPositionTextureInformation());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Index, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionTextureInformation __instance) =>
            {
                return __instance.@Index;
            },
            static (TerrainPositionTextureInformation __instance, int value) =>
            {
                __instance.@Index = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Name, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionTextureInformation __instance) =>
            {
                return __instance.@Name;
            },
            static (TerrainPositionTextureInformation __instance, string value) =>
            {
                __instance.@Name = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Factor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerrainPositionTextureInformation __instance) =>
            {
                return __instance.@Factor;
            },
            static (TerrainPositionTextureInformation __instance, float value) =>
            {
                __instance.@Factor = value;
            });
    }
}
