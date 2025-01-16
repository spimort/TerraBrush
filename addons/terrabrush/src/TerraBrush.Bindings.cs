using Godot;
using Godot.Bridge;

namespace TerraBrush;

public partial class TerraBrush {
	internal static void BindMethods(ClassDBRegistrationContext context) {
        context.BindConstructor(() => new TerraBrush());

        context.AddPropertyGroup("TerrainSettings");
        context.BindProperty(
            new PropertyInfo((StringName) nameof(ZonesSize), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.ZonesSize;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.ZonesSize = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(Resolution), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.Resolution;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.Resolution = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(CollisionOnly), VariantType.Bool) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.CollisionOnly;
            },
            static (TerraBrush instance, bool value) =>
            {
                instance.CollisionOnly = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(DataPath), VariantType.String) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Dir
            },
            static (TerraBrush instance) =>
            {
                return instance.DataPath;
            },
            static (TerraBrush instance, string value) =>
            {
                instance.DataPath = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(VisualInstanceLayers), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Layers3DRender
            },
            static (TerraBrush instance) =>
            {
                return instance.VisualInstanceLayers;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.VisualInstanceLayers = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(CustomShader), VariantType.Object) {
                Hint = PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = PropertyUsageFlags.Default,
                ClassName = (StringName) "ShaderMaterial"
            },
            static (TerraBrush instance) =>
            {
                return instance.CustomShader;
            },
            static (TerraBrush instance, ShaderMaterial value) =>
            {
                instance.CustomShader = value;
            }
        );

        context.AddPropertyGroup("LOD");
        context.BindProperty(
            new PropertyInfo((StringName) nameof(LODLevels), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.LODLevels;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.LODLevels = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(LODRowsPerLevel), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.LODRowsPerLevel;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.LODRowsPerLevel = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(LODInitialCellWidth), VariantType.Float, VariantTypeMetadata.Single) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.LODInitialCellWidth;
            },
            static (TerraBrush instance, float value) =>
            {
                instance.LODInitialCellWidth = value;
            }
        );

        context.AddPropertyGroup("Collision");
        context.BindProperty(
            new PropertyInfo((StringName) nameof(CreateCollisionInThread), VariantType.Bool) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.CreateCollisionInThread;
            },
            static (TerraBrush instance, bool value) =>
            {
                instance.CreateCollisionInThread = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(CollisionLayers), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Layers3DPhysics
            },
            static (TerraBrush instance) =>
            {
                return instance.CollisionLayers;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.CollisionLayers = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(CollisionMask), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Layers3DPhysics
            },
            static (TerraBrush instance) =>
            {
                return instance.CollisionMask;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.CollisionMask = value;
            }
        );

        context.AddPropertyGroup("Textures");
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(TextureSets), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.TextureSets;
        //     },
        //     static (TerraBrush instance, TextureSetsResource value) =>
        //     {
        //         instance.TextureSets = value;
        //     }
        // );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(TextureDetail), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.TextureDetail;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.TextureDetail = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(UseAntiTile), VariantType.Bool) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.UseAntiTile;
            },
            static (TerraBrush instance, bool value) =>
            {
                instance.UseAntiTile = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(NearestTextureFilter), VariantType.Bool) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.NearestTextureFilter;
            },
            static (TerraBrush instance, bool value) =>
            {
                instance.NearestTextureFilter = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(HeightBlendFactor), VariantType.Float, VariantTypeMetadata.Single) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.HeightBlendFactor;
            },
            static (TerraBrush instance, float value) =>
            {
                instance.HeightBlendFactor = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(AlbedoAlphaChannelUsage), VariantType.Int) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Enum
            },
            static (TerraBrush instance) =>
            {
                return instance.AlbedoAlphaChannelUsage;
            },
            static (TerraBrush instance, AlphaChannelUsage value) =>
            {
                instance.AlbedoAlphaChannelUsage = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(NormalAlphaChannelUsage), VariantType.Int) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Enum
            },
            static (TerraBrush instance) =>
            {
                return instance.NormalAlphaChannelUsage;
            },
            static (TerraBrush instance, AlphaChannelUsage value) =>
            {
                instance.NormalAlphaChannelUsage = value;
            }
        );

        context.AddPropertyGroup("Foliage");
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(Foliages), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.Foliages;
        //     },
        //     static (TerraBrush instance, FoliageResource[] value) =>
        //     {
        //         instance.Foliages = value;
        //     }
        // );

        context.AddPropertyGroup("Objects");
        context.BindProperty(
            new PropertyInfo((StringName) nameof(DefaultObjectFrequency), VariantType.Int, VariantTypeMetadata.Int32) {
                Usage = PropertyUsageFlags.Default,
            },
            static (TerraBrush instance) =>
            {
                return instance.DefaultObjectFrequency;
            },
            static (TerraBrush instance, int value) =>
            {
                instance.DefaultObjectFrequency = value;
            }
        );
        context.BindProperty(
            new PropertyInfo((StringName) nameof(ObjectLoadingStrategy), VariantType.Int) {
                Usage = PropertyUsageFlags.Default,
                Hint = PropertyHint.Enum
            },
            static (TerraBrush instance) =>
            {
                return instance.ObjectLoadingStrategy;
            },
            static (TerraBrush instance, ObjectLoadingStrategy value) =>
            {
                instance.ObjectLoadingStrategy = value;
            }
        );
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(Objects), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.Foliages;
        //     },
        //     static (TerraBrush instance, ObjectResource[] value) =>
        //     {
        //         instance.Foliages = value;
        //     }
        // );

        context.AddPropertyGroup("Water");
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(WaterDefinition), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.WaterDefinition;
        //     },
        //     static (TerraBrush instance, WaterResource value) =>
        //     {
        //         instance.WaterDefinition = value;
        //     }
        // );

        context.AddPropertyGroup("Snow");
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(SnowDefinition), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.SnowDefinition;
        //     },
        //     static (TerraBrush instance, SnowResource value) =>
        //     {
        //         instance.SnowDefinition = value;
        //     }
        // );

        context.AddPropertyGroup("Zones");
        // context.BindProperty(
        //     new PropertyInfo((StringName) nameof(TerrainZones), VariantType.Object) {
        //         Usage = PropertyUsageFlags.Default
        //     },
        //     static (TerraBrush instance) =>
        //     {
        //         return instance.TerrainZones;
        //     },
        //     static (TerraBrush instance, ZonesResource value) =>
        //     {
        //         instance.TerrainZones = value;
        //     }
        // );
    }
}
