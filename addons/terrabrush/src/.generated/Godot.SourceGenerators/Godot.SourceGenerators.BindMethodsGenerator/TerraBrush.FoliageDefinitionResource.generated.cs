#nullable enable

namespace TerraBrush;

partial class FoliageDefinitionResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Strategy { get; } = global::Godot.StringName.CreateStaticFromAscii("Strategy"u8);
        public static global::Godot.StringName @Mesh { get; } = global::Godot.StringName.CreateStaticFromAscii("Mesh"u8);
        public static global::Godot.StringName @MeshScale { get; } = global::Godot.StringName.CreateStaticFromAscii("MeshScale"u8);
        public static global::Godot.StringName @WindStrength { get; } = global::Godot.StringName.CreateStaticFromAscii("WindStrength"u8);
        public static global::Godot.StringName @NoiseTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("NoiseTexture"u8);
        public static global::Godot.StringName @VisualInstanceLayers { get; } = global::Godot.StringName.CreateStaticFromAscii("VisualInstanceLayers"u8);
        public static global::Godot.StringName @LODLevels { get; } = global::Godot.StringName.CreateStaticFromAscii("LODLevels"u8);
        public static global::Godot.StringName @LODRowsPerLevel { get; } = global::Godot.StringName.CreateStaticFromAscii("LODRowsPerLevel"u8);
        public static global::Godot.StringName @LODInitialCellWidth { get; } = global::Godot.StringName.CreateStaticFromAscii("LODInitialCellWidth"u8);
        public static global::Godot.StringName @Albedo { get; } = global::Godot.StringName.CreateStaticFromAscii("Albedo"u8);
        public static global::Godot.StringName @AlbedoTextures { get; } = global::Godot.StringName.CreateStaticFromAscii("AlbedoTextures"u8);
        public static global::Godot.StringName @NearestTextureFilter { get; } = global::Godot.StringName.CreateStaticFromAscii("NearestTextureFilter"u8);
        public static global::Godot.StringName @UseGroundColor { get; } = global::Godot.StringName.CreateStaticFromAscii("UseGroundColor"u8);
        public static global::Godot.StringName @CastShadow { get; } = global::Godot.StringName.CreateStaticFromAscii("CastShadow"u8);
        public static global::Godot.StringName @UseBrushScale { get; } = global::Godot.StringName.CreateStaticFromAscii("UseBrushScale"u8);
        public static global::Godot.StringName @ScaleNoiseTexture { get; } = global::Godot.StringName.CreateStaticFromAscii("ScaleNoiseTexture"u8);
        public static global::Godot.StringName @RandomPlacementRange { get; } = global::Godot.StringName.CreateStaticFromAscii("RandomPlacementRange"u8);
        public static global::Godot.StringName @ApplyOnTextureIndexes { get; } = global::Godot.StringName.CreateStaticFromAscii("ApplyOnTextureIndexes"u8);
        public static global::Godot.StringName @CustomShader { get; } = global::Godot.StringName.CreateStaticFromAscii("CustomShader"u8);
        public static global::Godot.StringName @MeshMaterial { get; } = global::Godot.StringName.CreateStaticFromAscii("MeshMaterial"u8);
        public static global::Godot.StringName @MaximumRenderDistance { get; } = global::Godot.StringName.CreateStaticFromAscii("MaximumRenderDistance"u8);
        public static global::Godot.StringName @EditorMaximumRenderDistance { get; } = global::Godot.StringName.CreateStaticFromAscii("EditorMaximumRenderDistance"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new FoliageDefinitionResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Strategy, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "MultiMesh:1,GPUParticle:2",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@Strategy;
            },
            static (FoliageDefinitionResource __instance, global::TerraBrush.FoliageStrategy value) =>
            {
                __instance.@Strategy = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Mesh, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Mesh",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Mesh"u8),
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@Mesh;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Mesh value) =>
            {
                __instance.@Mesh = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MeshScale, global::Godot.VariantType.Vector3)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@MeshScale;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Vector3 value) =>
            {
                __instance.@MeshScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@WindStrength, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@WindStrength;
            },
            static (FoliageDefinitionResource __instance, float value) =>
            {
                __instance.@WindStrength = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NoiseTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@NoiseTexture;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@NoiseTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@VisualInstanceLayers, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@VisualInstanceLayers;
            },
            static (FoliageDefinitionResource __instance, int value) =>
            {
                __instance.@VisualInstanceLayers = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODLevels, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@LODLevels;
            },
            static (FoliageDefinitionResource __instance, int value) =>
            {
                __instance.@LODLevels = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODRowsPerLevel, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@LODRowsPerLevel;
            },
            static (FoliageDefinitionResource __instance, int value) =>
            {
                __instance.@LODRowsPerLevel = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@LODInitialCellWidth, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@LODInitialCellWidth;
            },
            static (FoliageDefinitionResource __instance, float value) =>
            {
                __instance.@LODInitialCellWidth = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Albedo, global::Godot.VariantType.Color)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@Albedo;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Color value) =>
            {
                __instance.@Albedo = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AlbedoTextures, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return (global::Godot.Collections.GodotArray<global::Godot.Texture2D>)([.. __instance.@AlbedoTextures]);
            },
            static (FoliageDefinitionResource __instance, global::Godot.Collections.GodotArray<global::Godot.Texture2D> value) =>
            {
                __instance.@AlbedoTextures = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@NearestTextureFilter, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@NearestTextureFilter;
            },
            static (FoliageDefinitionResource __instance, bool value) =>
            {
                __instance.@NearestTextureFilter = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseGroundColor, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@UseGroundColor;
            },
            static (FoliageDefinitionResource __instance, bool value) =>
            {
                __instance.@UseGroundColor = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CastShadow, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@CastShadow;
            },
            static (FoliageDefinitionResource __instance, bool value) =>
            {
                __instance.@CastShadow = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@UseBrushScale, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@UseBrushScale;
            },
            static (FoliageDefinitionResource __instance, bool value) =>
            {
                __instance.@UseBrushScale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ScaleNoiseTexture, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@ScaleNoiseTexture;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Texture2D value) =>
            {
                __instance.@ScaleNoiseTexture = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@RandomPlacementRange, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@RandomPlacementRange;
            },
            static (FoliageDefinitionResource __instance, float value) =>
            {
                __instance.@RandomPlacementRange = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ApplyOnTextureIndexes, global::Godot.VariantType.PackedInt32Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "2/0:",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return (global::Godot.Collections.PackedInt32Array)([.. __instance.@ApplyOnTextureIndexes]);
            },
            static (FoliageDefinitionResource __instance, global::Godot.Collections.PackedInt32Array value) =>
            {
                __instance.@ApplyOnTextureIndexes = [.. value];
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CustomShader, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "ShaderMaterial",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("ShaderMaterial"u8),
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@CustomShader;
            },
            static (FoliageDefinitionResource __instance, global::Godot.ShaderMaterial value) =>
            {
                __instance.@CustomShader = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MeshMaterial, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Material",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Material"u8),
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@MeshMaterial;
            },
            static (FoliageDefinitionResource __instance, global::Godot.Material value) =>
            {
                __instance.@MeshMaterial = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MaximumRenderDistance, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@MaximumRenderDistance;
            },
            static (FoliageDefinitionResource __instance, int value) =>
            {
                __instance.@MaximumRenderDistance = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@EditorMaximumRenderDistance, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (FoliageDefinitionResource __instance) =>
            {
                return __instance.@EditorMaximumRenderDistance;
            },
            static (FoliageDefinitionResource __instance, int value) =>
            {
                __instance.@EditorMaximumRenderDistance = value;
            });
    }
}
