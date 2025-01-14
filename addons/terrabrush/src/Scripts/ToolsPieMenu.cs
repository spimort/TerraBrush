using System;
using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ToolsPieMenu : Control {
    [NodePath] private Control _pieMenu;

    public Action<TerrainToolType> OnToolSelected { get;set; }

    protected override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        foreach (var toolPreview in _pieMenu.GetChildren().Where(control => control is ToolPreview).Cast<ToolPreview>()) {
            toolPreview.OnSelect = () => {
                OnToolSelected.Invoke(toolPreview.ToolType);
            };
        }
    }
}
