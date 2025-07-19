#nullable enable

namespace TerraBrush;

partial class Terrain
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
        public static global::Godot.StringName @HeightMapFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightMapFactor"u8);
        public static global::Godot.StringName @CustomShader { get; } = global::Godot.StringName.CreateStaticFromAscii("CustomShader"u8);
        public static global::Godot.StringName @TextureSets { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureSets"u8);
        public static global::Godot.StringName @TextureDetail { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureDetail"u8);
        public static global::Godot.StringName @UseAntiTile { get; } = global::Godot.StringName.CreateStaticFromAscii("UseAntiTile"u8);
        public static global::Godot.StringName @NearestTextureFilter { get; } = global::Godot.StringName.CreateStaticFromAscii("NearestTextureFilter"u8);
        public static global::Godot.StringName @HeightBlendFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("HeightBlendFactor"u8);
        public static global::Godot.StringName @AlbedoAlphaChannelUsage { get; } = global::Godot.StringName.CreateStaticFromAscii("AlbedoAlphaChannelUsage"u8);
        public static global::Godot.StringName @NormalAlphaChannelUsage { get; } = global::Godot.StringName.CreateStaticFromAscii("NormalAlphaChannelUsage"u8);
        public static global::Godot.StringName @UseSharpTransitions { get; } = global::Godot.StringName.CreateStaticFromAscii("UseSharpTransitions"u8);
        public static global::Godot.StringName @WaterFactor { get; } = global::Godot.StringName.CreateStaticFromAscii("WaterFactor"u8);
        public static global::Godot.StringName @DefaultTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("DefaultTexture"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
        public static global::Godot.StringName @CollisionLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionLayers"u8);
        public static global::Godot.StringName @CollisionMask { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionMask"u8);
        public static global::Godot.StringName @LODLevels { get; } = global::Godot.StringName.CreateStaticFromAscii("LODLevels"u8);
        public static global::Godot.StringName @LODRowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("LODRowsPerLevel"u8);
        public static global::Godot.StringName @LODInitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("LODInitialCellWidth"u8);
        public static global::Godot.StringName @CollisionOnly { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionOnly"u8);
        public static global::Godot.StringName @CreateCollisionInThread { get; } = global::Godot.StringName.CreateStaticFromAscii("CreateCollisionInThread"u8);
        public static global::Godot.StringName @ShowMetaInfo { get; } = global::Godot.StringName.CreateStaticFromAscii("ShowMetaInfo"u8);
        public static global::Godot.StringName @MetaInfoLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("MetaInfoLayers"u8);
    }
    public new partial class SignalName : global::Godot.Node3D.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new Terrain());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ZonesSize, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@ZonesSize;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@ZonesSize = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Resolution, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@Resolution;
            },
            static (Terrain __instance, int value) =>
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
            static (Terrain __instance) =>
            {
                return __instance.@TerrainZones;
            },
            static (Terrain __instance, global::TerraBrush.ZonesResource value) =>
            {
                __instance.@TerrainZones = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightMapFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@HeightMapFactor;
            },
            static (Terrain __instance, float value) =>
            {
                __instance.@HeightMapFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CustomShader, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ShaderMaterial"u8),
            },
            static (Terrain __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (Terrain __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureSets, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "TextureSetsResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Resource"u8),
            },
            static (Terrain __instance) =>
            {
                return __instance.@TextureSets;
            },
            static (Terrain __instance, global::TerraBrush.TextureSetsResource value) =>
            {
                __instance.@TextureSets = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureDetail, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@TextureDetail;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@TextureDetail = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseAntiTile, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@UseAntiTile;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@UseAntiTile = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NearestTextureFilter, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@NearestTextureFilter;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@NearestTextureFilter = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@HeightBlendFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@HeightBlendFactor;
            },
            static (Terrain __instance, float value) =>
            {
                __instance.@HeightBlendFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AlbedoAlphaChannelUsage, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "None,Roughness,Height",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@AlbedoAlphaChannelUsage;
            },
            static (Terrain __instance, global::TerraBrush.AlphaChannelUsage value) =>
            {
                __instance.@AlbedoAlphaChannelUsage = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NormalAlphaChannelUsage, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "None,Roughness,Height",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@NormalAlphaChannelUsage;
            },
            static (Terrain __instance, global::TerraBrush.AlphaChannelUsage value) =>
            {
                __instance.@NormalAlphaChannelUsage = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseSharpTransitions, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@UseSharpTransitions;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@UseSharpTransitions = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WaterFactor, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@WaterFactor;
            },
            static (Terrain __instance, float value) =>
            {
                __instance.@WaterFactor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@DefaultTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (Terrain __instance) =>
            {
                return __instance.@DefaultTexture;
            },
            static (Terrain __instance, global::Godot.Texture2D value) =>
            {
                __instance.@DefaultTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@VisualInstanceLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@CollisionLayers;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@CollisionLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionMask, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@CollisionMask;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@CollisionMask = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODLevels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@LODLevels;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@LODLevels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODRowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@LODRowsPerLevel;
            },
            static (Terrain __instance, int value) =>
            {
                __instance.@LODRowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODInitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@LODInitialCellWidth;
            },
            static (Terrain __instance, float value) =>
            {
                __instance.@LODInitialCellWidth = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionOnly, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@CollisionOnly;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@CollisionOnly = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CreateCollisionInThread, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@CreateCollisionInThread;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@CreateCollisionInThread = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ShowMetaInfo, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@ShowMetaInfo;
            },
            static (Terrain __instance, bool value) =>
            {
                __instance.@ShowMetaInfo = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MetaInfoLayers, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:MetaInfoLayer",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (Terrain __instance) =>
            {
                return __instance.@MetaInfoLayers;
            },
            static (Terrain __instance, global::Godot.Collections.GodotArray<global::TerraBrush.MetaInfoLayer> value) =>
            {
                __instance.@MetaInfoLayers = value;
            });
    }
}
