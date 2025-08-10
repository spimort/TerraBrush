#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class ToolInfo : Control {
    [NodePath] private Label _label;
    [NodePath] private AnimationPlayer _animationPlayer;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }

    public void SetText(string text) {
        if (_label.Text != text) {
            _label.Text = text;

            _animationPlayer.Stop();
            if (string.IsNullOrWhiteSpace(text)) {
                _animationPlayer.Play("RESET");
            } else {
                _animationPlayer.Play("Show");
            }
        }
    }
}
#endif
