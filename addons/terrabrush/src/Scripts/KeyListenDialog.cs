#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class KeyListenDialog : Window {
	[Signal]
	public delegate void KeyListenAcceptedEventHandler(InputEventKey key);

	[Signal]
	public delegate void KeyListenCancelledEventHandler();

	[NodePath] private Label _keyDisplay;
	[NodePath] private Button _okButton;
	[NodePath] private Button _cancelButton;

	private InputEventKey _eventKey;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready() {
		this.RegisterNodePaths();
		_okButton.Pressed += () => {
			if (_eventKey == null) {
				_okButton.Disabled = true;
				return;
			}

			EmitSignal(SignalName.KeyListenAccepted, _eventKey);
		};
		_cancelButton.Pressed += () => EmitSignal(SignalName.KeyListenCancelled);
	}

	public override void _Input(InputEvent @event) {
		if (@event is InputEventKey iek) {
			_eventKey = iek;
			_okButton.Disabled = false;
			_keyDisplay.Text = KeybindManager.DescribeKey(iek);
		}
	}
}
#endif
