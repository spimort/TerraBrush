#nullable enable

namespace TerraBrush;

partial class ObjectOctreeLODDefinitionResource
{
    public new partial class MethodName : global::Godot.Resource.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.Resource.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.Resource.PropertyName
    {
        public static global::Godot.StringName @MaxDistance { get; } = global::Godot.StringName.CreateStaticFromAscii("MaxDistance"u8);
        public static global::Godot.StringName @AddCollision { get; } = global::Godot.StringName.CreateStaticFromAscii("AddCollision"u8);
        public static global::Godot.StringName @AutomaticLOD { get; } = global::Godot.StringName.CreateStaticFromAscii("AutomaticLOD"u8);
        public static global::Godot.StringName @AutomaticLODTargetVertices { get; } = global::Godot.StringName.CreateStaticFromAscii("AutomaticLODTargetVertices"u8);
        public static global::Godot.StringName @ObjectFrequency { get; } = global::Godot.StringName.CreateStaticFromAscii("ObjectFrequency"u8);
    }
    public new partial class SignalName : global::Godot.Resource.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ObjectOctreeLODDefinitionResource());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MaxDistance, global::Godot.VariantType.Float, global::Godot.Bridge.VariantTypeMetadata.Single)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODDefinitionResource __instance) =>
            {
                return __instance.@MaxDistance;
            },
            static (ObjectOctreeLODDefinitionResource __instance, float value) =>
            {
                __instance.@MaxDistance = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AddCollision, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODDefinitionResource __instance) =>
            {
                return __instance.@AddCollision;
            },
            static (ObjectOctreeLODDefinitionResource __instance, bool value) =>
            {
                __instance.@AddCollision = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AutomaticLOD, global::Godot.VariantType.Bool)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODDefinitionResource __instance) =>
            {
                return __instance.@AutomaticLOD;
            },
            static (ObjectOctreeLODDefinitionResource __instance, bool value) =>
            {
                __instance.@AutomaticLOD = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@AutomaticLODTargetVertices, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODDefinitionResource __instance) =>
            {
                return __instance.@AutomaticLODTargetVertices;
            },
            static (ObjectOctreeLODDefinitionResource __instance, int value) =>
            {
                __instance.@AutomaticLODTargetVertices = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ObjectFrequency, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ObjectOctreeLODDefinitionResource __instance) =>
            {
                return __instance.@ObjectFrequency;
            },
            static (ObjectOctreeLODDefinitionResource __instance, int value) =>
            {
                __instance.@ObjectFrequency = value;
            });
    }
}
