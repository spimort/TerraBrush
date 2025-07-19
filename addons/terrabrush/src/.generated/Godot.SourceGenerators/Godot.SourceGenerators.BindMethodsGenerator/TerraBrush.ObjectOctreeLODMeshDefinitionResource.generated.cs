#nullable enable

namespace TerraBrush;

partial class ObjectOctreeLODMeshDefinitionResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Mesh { get; } = global::Godot.StringName.CreateStaticFromAscii("Mesh"u8);
        public static global::Godot.StringName @MaterialOverride { get; } = global::Godot.StringName.CreateStaticFromAscii("MaterialOverride"u8);
        public static global::Godot.StringName @Scale { get; } = global::Godot.StringName.CreateStaticFromAscii("Scale"u8);
        public static global::Godot.StringName @CastShadow { get; } = global::Godot.StringName.CreateStaticFromAscii("CastShadow"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectOctreeLODMeshDefinitionResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Mesh, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Mesh",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Mesh"u8),
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance) =>
            {
                return __instance.@Mesh;
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance, global::Godot.Mesh value) =>
            {
                __instance.@Mesh = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MaterialOverride, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Material",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Material"u8),
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance) =>
            {
                return __instance.@MaterialOverride;
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance, global::Godot.Material value) =>
            {
                __instance.@MaterialOverride = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Scale, global::Godot.VariantType.Vector3)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance) =>
            {
                return __instance.@Scale;
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance, global::Godot.Vector3 value) =>
            {
                __instance.@Scale = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CastShadow, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "Off,On,DoubleSided,ShadowsOnly",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance) =>
            {
                return __instance.@CastShadow;
            },
            static (ObjectOctreeLODMeshDefinitionResource __instance, global::Godot.GeometryInstance3D.ShadowCastingSetting value) =>
            {
                __instance.@CastShadow = value;
            });
    }
}
