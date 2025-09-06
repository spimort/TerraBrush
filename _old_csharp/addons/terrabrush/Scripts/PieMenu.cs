#if TOOLS
using Godot;
using System;
using System.Linq;

namespace TerraBrush;

[Tool]
public partial class PieMenu : Control {
    private float _angleOffset;
    private int _buttonCount;
    private int _startingButtonsIndex;
    private DockPreviewButton _previewDockPreviewOverItem;

    [NodePath] private Control _controlsContainer;
    [NodePath] private AnimationPlayer _animationPlayer;
    [NodePath] private Label _menuLabel;
    [NodePath] private Control _pieBackground;
    [NodePath] private Control _currentAnglePanel;

    [Export] public int ButtonsRadiusRatio { get;set; } = 10;
    [Export] public int ButtonsMinRadius { get;set; } = 150;
    [Export] public string Label { get;set; }

    public override void _Ready() {
        this.RegisterNodePaths();

        UpdateContent();

        _animationPlayer.Play("EnterTree");

        var iconsColor = (Color) ProjectSettings.GetSetting(SettingContants.IconsColor);
        _menuLabel.Set("theme_override_colors/font_outline_color", iconsColor);
        var normalStyle = (StyleBoxFlat) _menuLabel.Get("theme_override_styles/normal");
        normalStyle.BgColor = iconsColor;
    }

    public override void _Process(double delta) {
        base._Process(delta);

        var itemForAngle = GetChild((int) GetMouseOverItemIndex());
        if (itemForAngle is DockPreviewButton dockPreviewItem) {
            _previewDockPreviewOverItem?.ClearMouseOver();

            dockPreviewItem.ShowMouseOver();
            _previewDockPreviewOverItem = dockPreviewItem;

            if (!string.IsNullOrWhiteSpace(dockPreviewItem.TooltipText)) {
                _menuLabel.Text = dockPreviewItem.TooltipText;
            }
        }

        UpdateCurrentAnglePanel();
    }

    public override void _GuiInput(InputEvent @event) {
        base._GuiInput(@event);

        if (@event is InputEventMouseButton inputButton && inputButton.ButtonIndex == MouseButton.Left) {
            var itemForAngle = GetChild((int) GetMouseOverItemIndex());
            if (itemForAngle is DockPreviewButton dockPreviewItem) {
                dockPreviewItem.OnSelect?.Invoke();
            }
        }
    }

    public void UpdateContent() {
        var buttons = GetChildren().Where(control => control != _controlsContainer).Cast<Control>().ToList();
        _buttonCount = buttons.Count;

        if (_buttonCount > 0) {
            _startingButtonsIndex = GetChildren().IndexOf(buttons[0]);
            _angleOffset = (float) (2 * Math.PI / _buttonCount);

            var buttonsRadius = Math.Max(ButtonsMinRadius, ButtonsRadiusRatio * _buttonCount);
            var buttonSize = buttons[0].Size;

            var currentAngle = Mathf.DegToRad(90);
            foreach (var button in buttons) {
                var xPosition = (float) Math.Cos(currentAngle) * buttonsRadius;
                var yPosition = (float) Math.Sin(currentAngle) * buttonsRadius;

                button.SetPosition(new Vector2(xPosition - (buttonSize.X / 2.0f), -yPosition - (buttonSize.Y / 2.0f)));
                currentAngle += _angleOffset;
            }

            var backgroundSize = (int) (buttonsRadius * 2 + buttonSize.X * 2);
            _pieBackground.Size = new Vector2(backgroundSize, backgroundSize);
            _pieBackground.Position = new Vector2(-(backgroundSize / 2), -(backgroundSize / 2));
        }

        _menuLabel.Text = Label;
    }

    private float GetMouseCurrentAngle() {
        var angle = GlobalPosition.AngleToPoint(GetGlobalMousePosition()) + Mathf.DegToRad(90);
        if (angle < 0) {
            angle += 2 * MathF.PI;
        }
        angle = (2 * MathF.PI) - angle;

        return angle;
    }

    private int GetMouseOverItemIndex() {
        var angle = GetMouseCurrentAngle();

        var index = angle / _angleOffset;
        var offsetIndex = index - Mathf.Floor(index);

        if (offsetIndex >= 0.5f) {
		    index += 1;
        }
        index = Mathf.Floor(index);

        if (index > _buttonCount - 1) {
		    index = 0;
        }

        index += _startingButtonsIndex;

        return (int) index;
    }

    private void UpdateCurrentAnglePanel() {
        if (_currentAnglePanel != null) {
            var angle = GlobalPosition.AngleToPoint(GetGlobalMousePosition()) + Mathf.DegToRad(90);
            if (angle < 0) {
                angle += 2 * MathF.PI;
            }
            _currentAnglePanel.Rotation = angle;
        }
    }
}
#endif
