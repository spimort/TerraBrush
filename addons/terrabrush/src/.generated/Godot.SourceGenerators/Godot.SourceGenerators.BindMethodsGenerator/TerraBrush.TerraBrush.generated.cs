#nullable enable

namespace TerraBrush;

partial class TerraBrush
{
    public new partial class MethodName : global::TerraBrush.TerraBrushTool.MethodName
    {
    }
    public new partial class ConstantName : global::TerraBrush.TerraBrushTool.ConstantName
    {
    }
    public new partial class PropertyName : global::TerraBrush.TerraBrushTool.PropertyName
    {
        public static global::Godot.StringName @ZonesSize { get; } = global::Godot.StringName.CreateStaticFromAscii("ZonesSize"u8);
        public static global::Godot.StringName @Resolution { get; } = global::Godot.StringName.CreateStaticFromAscii("Resolution"u8);
        public static global::Godot.StringName @CollisionOnly { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionOnly"u8);
        public static global::Godot.StringName @DataPath { get; } = global::Godot.StringName.CreateStaticFromAscii("DataPath"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
        public static global::Godot.StringName @CustomShader { get; } = global::Godot.StringName.CreateStaticFromAscii("CustomShader"u8);
        public static global::Godot.StringName @LODLevels { get; } = global::Godot.StringName.CreateStaticFromAscii("LODLevels"u8);
        public static global::Godot.StringName @LODRowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("LODRowsPerLevel"u8);
        public static global::Godot.StringName @LODInitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("LODInitialCellWidth"u8);
        public static global::Godot.StringName @CreateCollisionInThread { get; } = global::Godot.StringName.CreateStaticFromAscii("CreateCollisionInThread"u8);
        public static global::Godot.StringName @CollisionLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionLayers"u8);
        public static global::Godot.StringName @CollisionMask { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionMask"u8);
        public static global::Godot.StringName @TextureSets { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureSets"u8);
        public static global::Godot.StringName @TextureDetail { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureDetail"u8);
        public static global::Godot.StringName @UseAntiTile { get; } = global::Godot.StringName.CreateStaticFromAscii("UseAntiTile"u8);
        public static global::Godot.StringName @NearestTextureFilter { get; } = global::Godot.StringName.CreateStaticFromAscii("NearestTextureFilter"u8);
        public static global::Godot.StringName @HeightBlendFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightBlendFactor"u8);
        public static global::Godot.StringName @AlbedoAlphaChannelUsage { get; } = global::Godot.StringName.CreateStaticFromAscii("AlbedoAlphaChannelUsage"u8);
        public static global::Godot.StringName @NormalAlphaChannelUsage { get; } = global::Godot.StringName.CreateStaticFromAscii("NormalAlphaChannelUsage"u8);
        public static global::Godot.StringName @UseSharpTransitions { get; } = global::Godot.StringName.CreateStaticFromAscii("UseSharpTransitions"u8);
        public static global::Godot.StringName @Foliages { get; } = global::Godot.StringName.CreateStaticFromAscii("Foliages"u8);
        public static global::Godot.StringName @DefaultObjectFrequency { get; } = global::Godot.StringName.CreateStaticFromAscii("DefaultObjectFrequency"u8);
        public static global::Godot.StringName @ObjectLoadingStrategy { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectLoadingStrategy"u8);
        public static global::Godot.StringName @Objects { get; } = global::Godot.StringName.CreateStaticFromAscii("Objects"u8);
        public static global::Godot.StringName @WaterDefinition { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterDefinition"u8);
        public static global::Godot.StringName @SnowDefinition { get; } = global::Godot.StringName.CreateStaticFromAscii("SnowDefinition"u8);
        public static global::Godot.StringName @ShowMetaInfo { get; } = global::Godot.StringName.CreateStaticFromAscii("ShowMetaInfo"u8);
        public static global::Godot.StringName @MetaInfoLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("MetaInfoLayers"u8);
        public static global::Godot.StringName @TerrainZones { get; } = global::Godot.StringName.CreateStaticFromAscii("TerrainZones"u8);
    }
    public new partial class SignalName : global::TerraBrush.TerraBrushTool.SignalName
    {
        public static global::Godot.StringName @TerrainLoaded { get; } = global::Godot.StringName.CreateStaticFromAscii("TerrainLoaded"u8);
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.SetIcon("res://addons/terrabrush/src/icon.png");
        context.BindConstructor(() => new TerraBrush());
        context.AddPropertyGroup("TerrainSettings");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@Resolution;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@Resolution = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionOnly, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@CollisionOnly;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@CollisionOnly = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@DataPath, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@DataPath;
            },
            static (TerraBrush __instance, string value) =>
            {
                __instance.@DataPath = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (TerraBrush __instance, int value) =>
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
            static (TerraBrush __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (TerraBrush __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
        context.AddPropertyGroup("LOD");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODLevels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@LODLevels;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@LODLevels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODRowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@LODRowsPerLevel;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@LODRowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODInitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@LODInitialCellWidth;
            },
            static (TerraBrush __instance, float value) =>
            {
                __instance.@LODInitialCellWidth = value;
            });
        context.AddPropertyGroup("Collisions");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CreateCollisionInThread, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@CreateCollisionInThread;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@CreateCollisionInThread = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@CollisionLayers;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@CollisionLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionMask, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@CollisionMask;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@CollisionMask = value;
            });
        context.AddPropertyGroup("Textures");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureSets, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "TextureSetsResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@TextureSets;
            },
            static (TerraBrush __instance, global::TerraBrush.TextureSetsResource value) =>
            {
                __instance.@TextureSets = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureDetail, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@TextureDetail;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@TextureDetail = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseAntiTile, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@UseAntiTile;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@UseAntiTile = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NearestTextureFilter, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@NearestTextureFilter;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@NearestTextureFilter = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightBlendFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@HeightBlendFactor;
            },
            static (TerraBrush __instance, float value) =>
            {
                __instance.@HeightBlendFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AlbedoAlphaChannelUsage, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "None,Roughness,Height",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@AlbedoAlphaChannelUsage;
            },
            static (TerraBrush __instance, global::TerraBrush.AlphaChannelUsage value) =>
            {
                __instance.@AlbedoAlphaChannelUsage = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NormalAlphaChannelUsage, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "None,Roughness,Height",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@NormalAlphaChannelUsage;
            },
            static (TerraBrush __instance, global::TerraBrush.AlphaChannelUsage value) =>
            {
                __instance.@NormalAlphaChannelUsage = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseSharpTransitions, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@UseSharpTransitions;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@UseSharpTransitions = value;
            });
        context.AddPropertyGroup("Foliage");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Foliages, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:FoliageResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@Foliages;
            },
            static (TerraBrush __instance, global::Godot.Collections.GodotArray<global::TerraBrush.FoliageResource> value) =>
            {
                __instance.@Foliages = value;
            });
        context.AddPropertyGroup("Objects");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@DefaultObjectFrequency, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@DefaultObjectFrequency;
            },
            static (TerraBrush __instance, int value) =>
            {
                __instance.@DefaultObjectFrequency = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectLoadingStrategy, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "ThreadedInEditorOnly:1,Threaded:2,NotThreaded:3",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@ObjectLoadingStrategy;
            },
            static (TerraBrush __instance, global::TerraBrush.ObjectLoadingStrategy value) =>
            {
                __instance.@ObjectLoadingStrategy = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Objects, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ObjectResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@Objects;
            },
            static (TerraBrush __instance, global::Godot.Collections.GodotArray<global::TerraBrush.ObjectResource> value) =>
            {
                __instance.@Objects = value;
            });
        context.AddPropertyGroup("Water");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterDefinition, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "WaterResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@WaterDefinition;
            },
            static (TerraBrush __instance, global::TerraBrush.WaterResource value) =>
            {
                __instance.@WaterDefinition = value;
            });
        context.AddPropertyGroup("Snow");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@SnowDefinition, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "SnowResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@SnowDefinition;
            },
            static (TerraBrush __instance, global::TerraBrush.SnowResource value) =>
            {
                __instance.@SnowDefinition = value;
            });
        context.AddPropertyGroup("Meta");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ShowMetaInfo, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@ShowMetaInfo;
            },
            static (TerraBrush __instance, bool value) =>
            {
                __instance.@ShowMetaInfo = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MetaInfoLayers, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:MetaInfoLayer",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@MetaInfoLayers;
            },
            static (TerraBrush __instance, global::Godot.Collections.GodotArray<global::TerraBrush.MetaInfoLayer> value) =>
            {
                __instance.@MetaInfoLayers = value;
            });
        context.AddPropertyGroup("Zones");
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TerrainZones, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ZonesResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (TerraBrush __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (TerraBrush __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindSignal(new global::Godot.Bridge.SignalInfo(SignalName.@TerrainLoaded));
    }
}
