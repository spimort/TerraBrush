#nullable enable

namespace TerraBrush;

partial class Water
{
    public new partial class MethodName : global::Godot.Node3D.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Node3D.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Node3D.PropertyName
    {
        public static global::Godot.StringName @ZonesSize { get; } = global::Godot.StringName.CreateStaticFromAscii("ZonesSize"u8);
        public static global::Godot.StringName @Resolution { get; } = global::Godot.StringName.CreateStaticFromAscii("Resolution"u8);
        public static global::Godot.StringName @TerrainZones { get; } = global::Godot.StringName.CreateStaticFromAscii("TerrainZones"u8);
        public static global::Godot.StringName @WaterFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFactor"u8);
        public static global::Godot.StringName @HeightMapFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightMapFactor"u8);
        public static global::Godot.StringName @WaterInnerOffset { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterInnerOffset"u8);
        public static global::Godot.StringName @WaterColor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterColor"u8);
        public static global::Godot.StringName @FresnelColor { get; } = global::Godot.StringName.CreateStaticFromAscii("FresnelColor"u8);
        public static global::Godot.StringName @Metallic { get; } = global::Godot.StringName.CreateStaticFromAscii("Metallic"u8);
        public static global::Godot.StringName @Roughness { get; } = global::Godot.StringName.CreateStaticFromAscii("Roughness"u8);
        public static global::Godot.StringName @NormalMap { get; } = global::Godot.StringName.CreateStaticFromAscii("NormalMap"u8);
        public static global::Godot.StringName @NormalMap2 { get; } = global::Godot.StringName.CreateStaticFromAscii("NormalMap2"u8);
        public static global::Godot.StringName @TimeScale { get; } = global::Godot.StringName.CreateStaticFromAscii("TimeScale"u8);
        public static global::Godot.StringName @Strength { get; } = global::Godot.StringName.CreateStaticFromAscii("Strength"u8);
        public static global::Godot.StringName @Wave { get; } = global::Godot.StringName.CreateStaticFromAscii("Wave"u8);
        public static global::Godot.StringName @NoiseScale { get; } = global::Godot.StringName.CreateStaticFromAscii("NoiseScale"u8);
        public static global::Godot.StringName @HeightScale { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightScale"u8);
        public static global::Godot.StringName @ColorDeep { get; } = global::Godot.StringName.CreateStaticFromAscii("ColorDeep"u8);
        public static global::Godot.StringName @ColorShallow { get; } = global::Godot.StringName.CreateStaticFromAscii("ColorShallow"u8);
        public static global::Godot.StringName @BeersLaw { get; } = global::Godot.StringName.CreateStaticFromAscii("BeersLaw"u8);
        public static global::Godot.StringName @DepthOffset { get; } = global::Godot.StringName.CreateStaticFromAscii("DepthOffset"u8);
        public static global::Godot.StringName @EdgeScale { get; } = global::Godot.StringName.CreateStaticFromAscii("EdgeScale"u8);
        public static global::Godot.StringName @Near { get; } = global::Godot.StringName.CreateStaticFromAscii("Near"u8);
        public static global::Godot.StringName @Far { get; } = global::Godot.StringName.CreateStaticFromAscii("Far"u8);
        public static global::Godot.StringName @EdgeColor { get; } = global::Godot.StringName.CreateStaticFromAscii("EdgeColor"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
        public static global::Godot.StringName @LODLevels { get; } = global::Godot.StringName.CreateStaticFromAscii("LODLevels"u8);
        public static global::Godot.StringName @LODRowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("LODRowsPerLevel"u8);
        public static global::Godot.StringName @LODInitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("LODInitialCellWidth"u8);
        public static global::Godot.StringName @CustomShader { get; } = global::Godot.StringName.CreateStaticFromAscii("CustomShader"u8);
    }
    public new partial class SignalName : global::Godot.Node3D.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new Water());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (Water __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Resolution;
            },
            static (Water __instance, int value) =>
            {
                __instance.@Resolution = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TerrainZones, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ZonesResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (Water __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (Water __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@WaterFactor;
            },
            static (Water __instance, float value) =>
            {
                __instance.@WaterFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightMapFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@HeightMapFactor;
            },
            static (Water __instance, float value) =>
            {
                __instance.@HeightMapFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterInnerOffset, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@WaterInnerOffset;
            },
            static (Water __instance, float value) =>
            {
                __instance.@WaterInnerOffset = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@WaterColor;
            },
            static (Water __instance, global::Godot.Color value) =>
            {
                __instance.@WaterColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@FresnelColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@FresnelColor;
            },
            static (Water __instance, global::Godot.Color value) =>
            {
                __instance.@FresnelColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Metallic, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Metallic;
            },
            static (Water __instance, float value) =>
            {
                __instance.@Metallic = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Roughness, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Roughness;
            },
            static (Water __instance, float value) =>
            {
                __instance.@Roughness = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NormalMap, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (Water __instance) =>
            {
                return __instance.@NormalMap;
            },
            static (Water __instance, global::Godot.Texture2D value) =>
            {
                __instance.@NormalMap = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NormalMap2, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (Water __instance) =>
            {
                return __instance.@NormalMap2;
            },
            static (Water __instance, global::Godot.Texture2D value) =>
            {
                __instance.@NormalMap2 = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TimeScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@TimeScale;
            },
            static (Water __instance, float value) =>
            {
                __instance.@TimeScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Strength, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Strength;
            },
            static (Water __instance, float value) =>
            {
                __instance.@Strength = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Wave, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (Water __instance) =>
            {
                return __instance.@Wave;
            },
            static (Water __instance, global::Godot.Texture2D value) =>
            {
                __instance.@Wave = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NoiseScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@NoiseScale;
            },
            static (Water __instance, float value) =>
            {
                __instance.@NoiseScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@HeightScale;
            },
            static (Water __instance, float value) =>
            {
                __instance.@HeightScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ColorDeep, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@ColorDeep;
            },
            static (Water __instance, global::Godot.Color value) =>
            {
                __instance.@ColorDeep = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ColorShallow, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@ColorShallow;
            },
            static (Water __instance, global::Godot.Color value) =>
            {
                __instance.@ColorShallow = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@BeersLaw, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@BeersLaw;
            },
            static (Water __instance, float value) =>
            {
                __instance.@BeersLaw = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@DepthOffset, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@DepthOffset;
            },
            static (Water __instance, float value) =>
            {
                __instance.@DepthOffset = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@EdgeScale, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@EdgeScale;
            },
            static (Water __instance, float value) =>
            {
                __instance.@EdgeScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Near, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Near;
            },
            static (Water __instance, float value) =>
            {
                __instance.@Near = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Far, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@Far;
            },
            static (Water __instance, float value) =>
            {
                __instance.@Far = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@EdgeColor, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@EdgeColor;
            },
            static (Water __instance, global::Godot.Color value) =>
            {
                __instance.@EdgeColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (Water __instance, int value) =>
            {
                __instance.@VisualInstanceLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODLevels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@LODLevels;
            },
            static (Water __instance, int value) =>
            {
                __instance.@LODLevels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODRowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@LODRowsPerLevel;
            },
            static (Water __instance, int value) =>
            {
                __instance.@LODRowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODInitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Water __instance) =>
            {
                return __instance.@LODInitialCellWidth;
            },
            static (Water __instance, float value) =>
            {
                __instance.@LODInitialCellWidth = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CustomShader, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ShaderMaterial"u8),
            },
            static (Water __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (Water __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
    }
}
