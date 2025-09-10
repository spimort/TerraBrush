#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class CustomContentPieMenu : Control {
    [NodePath] private PieMenu _pieMenu;

    public PieMenu PieMenu => _pieMenu;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }
}
#endif
