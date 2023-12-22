#nullable enable
using Godot;
using System;
using System.Text;
using TerraBrush;

namespace TerraBrush;

[Tool]
public partial class KeyListenDialog : Window
{
	[Signal]
	public delegate void KeyListenAcceptedEventHandler(InputEventKey key);

	[Signal]
	public delegate void KeyListenCancelledEventHandler();
	
	[NodePath("VBoxContainer/KeyDisplay")] private Label _keyDisplay;
	[NodePath("VBoxContainer/HBoxContainer/OkButton")] private Button _okButton;
	[NodePath("VBoxContainer/HBoxContainer/CancelButton")] private Button _cancelButton;

	private InputEventKey? _eventKey;

	private KeybindManager _keybindManager;
	
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		this.RegisterNodePaths();
		_okButton.Pressed += () => EmitSignal(SignalName.KeyListenAccepted, _eventKey);
		_cancelButton.Pressed += () => EmitSignal(SignalName.KeyListenCancelled);
		_keybindManager = new KeybindManager();
	}

	public override void _Input(InputEvent @event)
	{
		if (@event is InputEventKey iek)
		{
			_eventKey = iek;
			_okButton.Disabled = false;
			_keyDisplay.Text = _keybindManager.DescribeKey(iek);
		}
	}
}
