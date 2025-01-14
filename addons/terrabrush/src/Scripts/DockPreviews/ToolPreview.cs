using Godot;

namespace TerraBrush;

public partial class ToolPreview : DockPreviewButton {
    [Export] public TerrainToolType ToolType { get;set; }
}
