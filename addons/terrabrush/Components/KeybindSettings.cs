using Godot;
using System.Text;

namespace TerraBrush;

[Tool]
public partial class KeybindSettings : Godot.AcceptDialog
{
	[NodePath("VBoxContainer/Tree")]
	private Tree _keybindList;
	private TreeItem _root;

	private string ProperCase(StringName name)
	{
		var parts = name.ToString().Split("_");
		var newString = new StringBuilder();
		foreach (var part in parts)
		{
			if (part == "terrabrush") continue;
			newString.Append(char.ToUpperInvariant(part[0]) + part[1..]);
			newString.Append(' ');
		}

		return newString.ToString();
	}
	
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		GD.Print("In Ready");
		this.RegisterNodePaths();
		var manager = new KeybindManager();
		_root = _keybindList.CreateItem();
		_keybindList.SetColumnTitle(0,"Name");
		_keybindList.SetColumnTitle(1, "Binding");
		_keybindList.HideRoot = true;
		foreach (var action in manager.ActionNames)
		{
			var iter = _keybindList.CreateItem();
			iter.SetText(0, ProperCase(action));
			var bindings = manager.GetBinding(action);
			var key = (InputEventKey)bindings[0];
			var bindingString = new StringBuilder();
			if (key.CtrlPressed) bindingString.Append("Ctrl+");
			if (key.AltPressed) bindingString.Append("Alt+");
			bindingString.Append(key.Keycode.ToString());
			iter.SetText(1,bindingString.ToString());
		}
	}
}
