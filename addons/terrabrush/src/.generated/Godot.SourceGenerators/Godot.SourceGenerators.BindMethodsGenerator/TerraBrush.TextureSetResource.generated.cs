#nullable enable

namespace TerraBrush;

partial class TextureSetResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Name { get; } = global::Godot.StringName.CreateStaticFromAscii("Name"u8);
        public static global::Godot.StringName @AlbedoTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("AlbedoTexture"u8);
        public static global::Godot.StringName @NormalTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("NormalTexture"u8);
        public static global::Godot.StringName @RoughnessTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("RoughnessTexture"u8);
        public static global::Godot.StringName @HeightTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightTexture"u8);
        public static global::Godot.StringName @TextureDetail { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureDetail"u8);
        public static global::Godot.StringName @Triplanar { get; } = global::Godot.StringName.CreateStaticFromAscii("Triplanar"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new TextureSetResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Name, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@Name;
            },
            static (TextureSetResource __instance, string value) =>
            {
                __instance.@Name = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AlbedoTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@AlbedoTexture;
            },
            static (TextureSetResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@AlbedoTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NormalTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@NormalTexture;
            },
            static (TextureSetResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@NormalTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RoughnessTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@RoughnessTexture;
            },
            static (TextureSetResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@RoughnessTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@HeightTexture;
            },
            static (TextureSetResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@HeightTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureDetail, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@TextureDetail;
            },
            static (TextureSetResource __instance, int value) =>
            {
                __instance.@TextureDetail = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Triplanar, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TextureSetResource __instance) =>
            {
                return __instance.@Triplanar;
            },
            static (TextureSetResource __instance, bool value) =>
            {
                __instance.@Triplanar = value;
            });
    }
}
