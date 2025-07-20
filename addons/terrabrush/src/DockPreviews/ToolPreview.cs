using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ToolPreview : DockPreviewButton {
    [BindProperty] public TerrainToolType ToolType { get;set; }
}
