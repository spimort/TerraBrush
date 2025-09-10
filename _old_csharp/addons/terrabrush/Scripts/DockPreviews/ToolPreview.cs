#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class ToolPreview : DockPreviewButton {
    [Export] public TerrainToolType ToolType { get;set; }
}
#endif
