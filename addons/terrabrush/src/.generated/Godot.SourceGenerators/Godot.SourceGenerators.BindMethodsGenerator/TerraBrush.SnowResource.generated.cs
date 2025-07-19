#nullable enable

namespace TerraBrush;

partial class SnowResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @SnowFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowFactor"u8);
        public static global::Godot.StringName @SnowInnerOffset { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowInnerOffset"u8);
        public static global::Godot.StringName @SnowColorTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowColorTexture"u8);
        public static global::Godot.StringName @SnowColorNormal { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowColorNormal"u8);
        public static global::Godot.StringName @SnowColorRoughness { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowColorRoughness"u8);
        public static global::Godot.StringName @SnowColorDetail { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowColorDetail"u8);
        public static global::Godot.StringName @Noise { get; } = global::Godot.StringName.CreateStaticFromAscii("Noise"u8);
        public static global::Godot.StringName @NoiseFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("NoiseFactor"u8);
        public static global::Godot.StringName @Metallic { get; } = global::Godot.StringName.CreateStaticFromAscii("Metallic"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
        public static global::Godot.StringName @CustomShader { get; } = global::Godot.StringName.CreateStaticFromAscii("CustomShader"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new SnowResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowFactor;
            },
            static (SnowResource __instance, float value) =>
            {
                __instance.@SnowFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowInnerOffset, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowInnerOffset;
            },
            static (SnowResource __instance, float value) =>
            {
                __instance.@SnowInnerOffset = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowColorTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowColorTexture;
            },
            static (SnowResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@SnowColorTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowColorNormal, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowColorNormal;
            },
            static (SnowResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@SnowColorNormal = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowColorRoughness, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowColorRoughness;
            },
            static (SnowResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@SnowColorRoughness = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowColorDetail, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@SnowColorDetail;
            },
            static (SnowResource __instance, float value) =>
            {
                __instance.@SnowColorDetail = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Noise, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (SnowResource __instance) =>
            {
                return __instance.@Noise;
            },
            static (SnowResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@Noise = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NoiseFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@NoiseFactor;
            },
            static (SnowResource __instance, float value) =>
            {
                __instance.@NoiseFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Metallic, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@Metallic;
            },
            static (SnowResource __instance, float value) =>
            {
                __instance.@Metallic = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (SnowResource __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (SnowResource __instance, int value) =>
            {
                __instance.@VisualInstanceLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CustomShader, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ShaderMaterial"u8),
            },
            static (SnowResource __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (SnowResource __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
    }
}
