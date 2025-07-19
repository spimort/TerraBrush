#nullable enable

namespace TerraBrush;

partial class DockPreviewButton
{
    public new partial class MethodName : global::Godot.TextureButton.MethodName
    {
    }
    public new partial class ConstantName : global::Godot.TextureButton.ConstantName
    {
    }
    public new partial class PropertyName : global::Godot.TextureButton.PropertyName
    {
        public static global::Godot.StringName @ButtonImage { get; } = global::Godot.StringName.CreateStaticFromAscii("ButtonImage"u8);
        public static global::Godot.StringName @TextureRect { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureRect"u8);
        public static global::Godot.StringName @MarginContainer { get; } = global::Godot.StringName.CreateStaticFromAscii("MarginContainer"u8);
        public static global::Godot.StringName @Label { get; } = global::Godot.StringName.CreateStaticFromAscii("Label"u8);
        public static global::Godot.StringName @IconType { get; } = global::Godot.StringName.CreateStaticFromAscii("IconType"u8);
        public static global::Godot.StringName @Margin { get; } = global::Godot.StringName.CreateStaticFromAscii("Margin"u8);
        public static global::Godot.StringName @Text { get; } = global::Godot.StringName.CreateStaticFromAscii("Text"u8);
        public static global::Godot.StringName @TextureSquare { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureSquare"u8);
        public static global::Godot.StringName @TextureCircle { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureCircle"u8);
        public static global::Godot.StringName @TextureCircleWithSign { get; } = global::Godot.StringName.CreateStaticFromAscii("TextureCircleWithSign"u8);
    }
    public new partial class SignalName : global::Godot.TextureButton.SignalName
    {
    }
#pragma warning disable CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    internal static void BindMethods(global::Godot.Bridge.ClassRegistrationContext context)
#pragma warning restore CS0108 // Method might already be defined higher in the hierarchy, that's not an issue.
    {
        context.BindConstructor(() => new DockPreviewButton());
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@ButtonImage, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@ButtonImage;
            },
            static (DockPreviewButton __instance, global::Godot.Texture2D value) =>
            {
                __instance.@ButtonImage = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureRect, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.NodeType,
                HintString = "TextureRect",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("TextureRect"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@TextureRect;
            },
            static (DockPreviewButton __instance, global::Godot.TextureRect value) =>
            {
                __instance.@TextureRect = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@MarginContainer, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.NodeType,
                HintString = "MarginContainer",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("MarginContainer"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@MarginContainer;
            },
            static (DockPreviewButton __instance, global::Godot.MarginContainer value) =>
            {
                __instance.@MarginContainer = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Label, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.NodeType,
                HintString = "Label",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Label"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@Label;
            },
            static (DockPreviewButton __instance, global::Godot.Label value) =>
            {
                __instance.@Label = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@IconType, global::Godot.VariantType.Int)
            {
                Hint = global::Godot.PropertyHint.Enum,
                HintString = "Square:1,Circle:2,CircleWithSign:3",
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@IconType;
            },
            static (DockPreviewButton __instance, global::TerraBrush.IconType value) =>
            {
                __instance.@IconType = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Margin, global::Godot.VariantType.Int, global::Godot.Bridge.VariantTypeMetadata.Int32)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@Margin;
            },
            static (DockPreviewButton __instance, int value) =>
            {
                __instance.@Margin = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@Text, global::Godot.VariantType.String)
            {
                Usage = global::Godot.PropertyUsageFlags.Default,
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@Text;
            },
            static (DockPreviewButton __instance, string value) =>
            {
                __instance.@Text = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureSquare, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@TextureSquare;
            },
            static (DockPreviewButton __instance, global::Godot.Texture2D value) =>
            {
                __instance.@TextureSquare = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureCircle, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@TextureCircle;
            },
            static (DockPreviewButton __instance, global::Godot.Texture2D value) =>
            {
                __instance.@TextureCircle = value;
            });
        context.BindProperty(new global::Godot.Bridge.PropertyInfo(PropertyName.@TextureCircleWithSign, global::Godot.VariantType.Object)
            {
                Hint = global::Godot.PropertyHint.ResourceType,
                HintString = "Texture2D",
                Usage = global::Godot.PropertyUsageFlags.Default,
                ClassName = global::Godot.StringName.CreateStaticFromAscii("Texture2D"u8),
            },
            static (DockPreviewButton __instance) =>
            {
                return __instance.@TextureCircleWithSign;
            },
            static (DockPreviewButton __instance, global::Godot.Texture2D value) =>
            {
                __instance.@TextureCircleWithSign = value;
            });
    }
}
