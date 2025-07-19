#nullable enable

namespace TerraBrush;

partial class ObjectOctreeLODMeshesDefinitionResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @Meshes { get; } = global::Godot.StringName.CreateStaticFromAscii("Meshes"u8);
        public static global::Godot.StringName @CollisionShape { get; } = global::Godot.StringName.CreateStaticFromAscii("CollisionShape"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectOctreeLODMeshesDefinitionResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Meshes, global::Godot.VariantType.Array)
            {
                Hint = global::Godot.PropertyHint.TypeString,
                HintString = "24/17:ObjectOctreeLODMeshDefinitionResource",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODMeshesDefinitionResource __instance) =>
            {
                return __instance.@Meshes;
            },
            static (ObjectOctreeLODMeshesDefinitionResource __instance, global::Godot.Collections.GodotArray<global::TerraBrush.ObjectOctreeLODMeshDefinitionResource> value) =>
            {
                __instance.@Meshes = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@CollisionShape, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Shape3D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Shape3D"u8),
            },
            static (ObjectOctreeLODMeshesDefinitionResource __instance) =>
            {
                return __instance.@CollisionShape;
            },
            static (ObjectOctreeLODMeshesDefinitionResource __instance, global::Godot.Shape3D value) =>
            {
                __instance.@CollisionShape = value;
            });
    }
}
