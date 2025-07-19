#nullable enable

namespace TerraBrush;

partial class WaterResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @WaterFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFactor"u8);
        public static global::Godot.StringName @WaterInnerOffset { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterInnerOffset"u8);
        public static global::Godot.StringName @WaterColor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterColor"u8);
        public static global::Godot.StringName @WaterFresnelColor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFresnelColor"u8);
        public static global::Godot.StringName @WaterMetallic { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterMetallic"u8);
        public static global::Godot.StringName @WaterRoughness { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterRoughness"u8);
        public static global::Godot.StringName @WaterNormalMap { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterNormalMap"u8);
        public static global::Godot.StringName @WaterNormalMap2 { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterNormalMap2"u8);
        public static global::Godot.StringName @WaterTimeScale { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterTimeScale"u8);
        public static global::Godot.StringName @WaterStrength { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterStrength"u8);
        public static global::Godot.StringName @WaterWave { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterWave"u8);
        public static global::Godot.StringName @WaterNoiseScale { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterNoiseScale"u8);
        public static global::Godot.StringName @WaterHeightScale { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterHeightScale"u8);
        public static global::Godot.StringName @WaterColorDeep { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterColorDeep"u8);
        public static global::Godot.StringName @WaterColorShallow { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterColorShallow"u8);
        public static global::Godot.StringName @WaterBeersLaw { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterBeersLaw"u8);
        public static global::Godot.StringName @WaterDepthOffset { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterDepthOffset"u8);
        public static global::Godot.StringName @WaterEdgeScale { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterEdgeScale"u8);
        public static global::Godot.StringName @WaterNear { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterNear"u8);
        public static global::Godot.StringName @WaterFar { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFar"u8);
        public static global::Godot.StringName @WaterEdgeColor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterEdgeColor"u8);
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
        context.BindConstructor(() => new WaterResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterFactor;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterInnerOffset, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterInnerOffset;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterInnerOffset = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterColor;
            },
            static (WaterResource __instance, global::Godot.Color value) =>
            {
                __instance.@WaterColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFresnelColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterFresnelColor;
            },
            static (WaterResource __instance, global::Godot.Color value) =>
            {
                __instance.@WaterFresnelColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterMetallic, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterMetallic;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterMetallic = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterRoughness, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterRoughness;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterRoughness = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterNormalMap, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterNormalMap;
            },
            static (WaterResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@WaterNormalMap = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterNormalMap2, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterNormalMap2;
            },
            static (WaterResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@WaterNormalMap2 = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterTimeScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterTimeScale;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterTimeScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterStrength, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterStrength;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterStrength = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterWave, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterWave;
            },
            static (WaterResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@WaterWave = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterNoiseScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterNoiseScale;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterNoiseScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterHeightScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterHeightScale;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterHeightScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterColorDeep, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterColorDeep;
            },
            static (WaterResource __instance, global::Godot.Color value) =>
            {
                __instance.@WaterColorDeep = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterColorShallow, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterColorShallow;
            },
            static (WaterResource __instance, global::Godot.Color value) =>
            {
                __instance.@WaterColorShallow = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterBeersLaw, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterBeersLaw;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterBeersLaw = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterDepthOffset, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterDepthOffset;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterDepthOffset = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterEdgeScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterEdgeScale;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterEdgeScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterNear, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterNear;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterNear = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFar, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterFar;
            },
            static (WaterResource __instance, float value) =>
            {
                __instance.@WaterFar = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterEdgeColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@WaterEdgeColor;
            },
            static (WaterResource __instance, global::Godot.Color value) =>
            {
                __instance.@WaterEdgeColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (WaterResource __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (WaterResource __instance, int value) =>
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
            static (WaterResource __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (WaterResource __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
    }
}
