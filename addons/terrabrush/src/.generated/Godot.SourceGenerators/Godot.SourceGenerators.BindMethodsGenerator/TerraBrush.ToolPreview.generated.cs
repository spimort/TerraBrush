#nullable enable

namespace TerraBrush;

partial class ToolPreview
{
    public new partial class MethodName : global::TerraBrush.DockPreviewButton.MethodName
    {
    }
    public new partial class ConstantName : global::TerraBrush.DockPreviewButton.ConstantName
    {
    }
    public new partial class PropertyName : global::TerraBrush.DockPreviewButton.PropertyName
    {
        public static global::Godot.StringName @ToolType { get; } = global::Godot.StringName.CreateStaticFromAscii("ToolType"u8);
    }
    public new partial class SignalName : global::TerraBrush.DockPreviewButton.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new ToolPreview());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ToolType, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "None:0,TerrainAdd:1,TerrainRemove:2,TerrainSmooth:3,TerrainFlatten:4,TerrainSetHeight:5,TerrainSetAngle:19,Paint:6,FoliageAdd:7,FoliageRemove:8,ObjectAdd:9,ObjectRemove:10,WaterAdd:11,WaterRemove:12,WaterFlowAdd:13,WaterFlowRemove:14,SnowAdd:15,SnowRemove:16,HoleAdd:17,HoleRemove:18,LockAdd:20,LockRemove:21,MetaInfoAdd:22,MetaInfoRemove:23",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (ToolPreview __instance) =>
            {
                return __instance.@ToolType;
            },
            static (ToolPreview __instance, global::TerraBrush.TerrainToolType value) =>
            {
                __instance.@ToolType = value;
            });
    }
}
