using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ToolInfo : Control {
    private PanelContainer _panelContainer;
    private Label _label;
    private Tween _tween;

    protected override void _Ready() {
        base._Ready();

        _panelContainer = new PanelContainer {
            Size = new Vector2(40, 41),
            MouseFilter = MouseFilterEnum.Ignore,
            Modulate = Color.FromHtml("#ffffff00")
        };
        _panelContainer.Set((StringName)"theme_override_styles/panel", new StyleBoxFlat() {
            BgColor = Color.FromHtml("#272727"),
        });
        AddChild(_panelContainer);

        var marginContainer = new MarginContainer() {
            MouseFilter = MouseFilterEnum.Ignore
        };
        marginContainer.Set((StringName)"theme_override_constants/margin_left", 10);
        marginContainer.Set((StringName)"theme_override_constants/margin_top", 10);
        marginContainer.Set((StringName)"theme_override_constants/margin_right", 10);
        marginContainer.Set((StringName)"theme_override_constants/margin_bottom", 10);
        _panelContainer.AddChild(marginContainer);

        _label = new Label();
        marginContainer.AddChild(_label);

        _tween = CreateTween();
    }

    public void SetText(string text) {
        if (_label.Text != text) {
            _label.Text = text;

            _tween.Stop();

            if (!string.IsNullOrWhiteSpace(text)) {
                _panelContainer.Modulate = Color.FromHtml("#ffffff00");
                _tween.TweenProperty(_panelContainer, (NodePath)"modulate", Color.FromHtml("#ffffff"), 0.1);
                _tween.TweenProperty(_panelContainer, (NodePath)"modulate", Color.FromHtml("#ffffff"), 5.0);
                _tween.TweenProperty(_panelContainer, (NodePath)"modulate", Color.FromHtml("#ffffff00"), 0.1);
            }
        }
    }
}
