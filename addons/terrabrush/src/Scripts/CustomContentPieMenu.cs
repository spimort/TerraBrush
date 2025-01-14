using Godot;

namespace TerraBrush;

public partial class CustomContentPieMenu : Control {
    [NodePath] private PieMenu _pieMenu;

    public PieMenu PieMenu => _pieMenu;

    protected override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }
}
