#if TOOLS
using System;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class ToolsPieMenu : Control {
    [NodePath] private Control _pieMenu;

    public Action<TerrainToolType> OnToolSelected { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        foreach (var toolPreview in _pieMenu.GetChildren().Where(control => control is ToolPreview).Cast<ToolPreview>()) {
            toolPreview.OnSelect = () => {
                OnToolSelected.Invoke(toolPreview.ToolType);
            };
        }
    }
}
#endif
