using Godot;
using System;
using System.Threading.Tasks;

namespace TerraBrush;

[Tool]
public partial class NumericSelectorDialog : Window {
	[Signal]
	public delegate void NumericSelectorAcceptedEventHandler(float value);

	[Signal]
	public delegate void NumericSelectorCancelledEventHandler();

	[NodePath] private SpinBox _spinBox;
	[NodePath] private Button _okButton;
	[NodePath] private Button _cancelButton;

	public float? MinValue { get;set; }
	public float? MaxValue { get;set; }

	public override void _Ready() {
		this.RegisterNodePaths();

		_okButton.Pressed += () => {
			SendResult();
		};
		_cancelButton.Pressed += () => EmitSignal(SignalName.NumericSelectorCancelled);
		CloseRequested += () => EmitSignal(SignalName.NumericSelectorCancelled);

		_spinBox.GetLineEdit().GrabFocus();

		if (MinValue.HasValue) {
			_spinBox.MinValue = MinValue.Value;
		}

		if (MaxValue.HasValue) {
			_spinBox.MaxValue = MaxValue.Value;
		}
	}

    public void SetValue(float value) {
		_spinBox.Value = value;
	}

    public override void _UnhandledKeyInput(InputEvent @event) {
        base._Input(@event);

		if (@event is InputEventKey inputEvent && (inputEvent.Keycode == Key.Enter || inputEvent.Keycode == Key.KpEnter)) {
			CallDeferred(nameof(SendResult));
		}
    }

	private void SendResult() {
		EmitSignal(SignalName.NumericSelectorAccepted, (float) Math.Round(_spinBox.Value, 2));
	}
}
