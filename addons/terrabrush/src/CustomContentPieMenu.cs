using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class CustomContentPieMenu : Control {
    private PieMenu _pieMenu;

    public PieMenu PieMenu => _pieMenu;

    protected override void _Ready() {
        base._Ready();

        _pieMenu = new PieMenu() {
            ButtonsMinRadius = 20
        };
        AddChild(_pieMenu);
    }
}
