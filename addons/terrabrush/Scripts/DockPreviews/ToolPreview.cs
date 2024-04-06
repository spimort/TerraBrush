#if TOOLS
using System;
using Godot;

namespace TerraBrush;

[Tool]
public partial class ToolPreview : DockPreviewButton {
    [Export] public TerrainToolType ToolType { get;set; }
}

// public enum IconType {
//     Square = 1,
//     Circle = 2,
//     CircleWithSign = 3
// }

// [Tool]
// public partial class ToolPreview : TextureButton, IDockPreview {
//     private Texture2D _textureNormal;

//     private Color _normalColor;
//     private Color _hoverColor;
//     private Color _pressedColor;

//     [NodePath] private TextureRect _toolIconTextureRect;

//     public Action OnSelect { get;set; }

//     [Export] public Texture2D ToolImage { get;set; }
//     [Export] public TerrainToolType ToolType { get;set; }
//     [Export] public IconType IconType { get;set; } = IconType.Square;

//     [Export] public Texture2D TextureSquare { get;set; }
//     [Export] public Texture2D TextureCircle { get;set; }
//     [Export] public Texture2D TextureCircleWithSign { get;set; }

//     public override void _Ready() {
//         base._Ready();
//         this.RegisterNodePaths();

//         Connect("pressed", new Callable(this, nameof(OnItemSelect)));

//         if (ToolImage != null) {
//             SetTextureImage(ToolImage);
//         }

//         CalculateTextures();
//         TextureNormal = _textureNormal;

//         var iconsColor = (Color) ProjectSettings.GetSetting(SettingContants.IconsColor);

//         _normalColor = iconsColor;
//         _hoverColor = _normalColor.Lightened(0.3f);
//         _pressedColor = _normalColor.Lightened(0.4f);
//     }

//     public override void _Notification(int what) {
//         base._Notification(what);

//         if (what == NotificationDraw) {
//             SetTextureColor(GetDrawMode());
//         }
//     }

//     public void SetTextureImage(Texture2D toolImage) {
//         _toolIconTextureRect.Texture = toolImage;
//     }

//     public void OnItemSelect() {
//         OnSelect?.Invoke();
//     }

//     public void ShowMouseOver() {
//         SetTextureColor(DrawMode.Hover);
//     }

//     public void ClearMouseOver() {
//         SetTextureColor(DrawMode.Normal);
//     }

//     private void CalculateTextures() {
//         switch (IconType) {
//             case IconType.Square:
//                 _textureNormal = TextureSquare;
//                 break;
//             case IconType.Circle:
//                 _textureNormal = TextureCircle;
//                 break;
//             case IconType.CircleWithSign:
//                 _textureNormal = TextureCircleWithSign;
//                 break;
//         }
//     }

//     private void SetTextureColor(DrawMode drawMode) {
//         var color = drawMode switch {
//             DrawMode.Normal => _normalColor,
//             DrawMode.Hover => _hoverColor,
//             DrawMode.Pressed => _pressedColor,
//             DrawMode.HoverPressed => _pressedColor,
//             _ => _normalColor
//         };

//         var tween = CreateTween();
//         tween.TweenProperty(this, "self_modulate", color, 0.1);
//     }
// }
#endif
