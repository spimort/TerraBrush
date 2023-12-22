using Godot;
using System.Text;

namespace TerraBrush;

[Tool]
public partial class KeybindSettings : Godot.AcceptDialog
{
	[NodePath("VBoxContainer/Tree")] private Tree _keybindList;
	private TreeItem _root;
	private KeybindManager _keybindManager;

	private enum ShortcutType {
	    Add = 1,
	    Erase  = 2
	}

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
		var iconTheme = EditorInterface.Singleton.GetEditorTheme();
		_keybindManager = new KeybindManager();
		_root = _keybindList.CreateItem();
		_keybindList.SetColumnTitle(0,"Name");
		_keybindList.SetColumnTitle(1, "Binding");
		_keybindList.HideRoot = true;
		foreach (var action in _keybindManager.ActionNames)
		{
			var iter = _keybindList.CreateItem();
			iter.SetText(0, ProperCase(action));
			iter.SetText(1,_keybindManager.DescribeKey(action));
			
			iter.AddButton(1, iconTheme.GetIcon("Edit", "EditorIcons"), ADD_SHORTCUT);
			iter.AddButton(1, iconTheme.GetIcon("Close", "EditorIcons"), ERASE_SHORTCUT);
			iter.SetMetadata(0,action);
		}
		_keybindList.ButtonClicked += KeybindListOnButtonClicked;
	}

	private void KeybindListOnButtonClicked(TreeItem item, long column, long id, long mousebuttonindex)
	{
		if (column != 1) return;
		if ((MouseButton)mousebuttonindex != MouseButton.Left) return;
		switch (id)
		{
			case ADD_SHORTCUT:
				var dlg = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/KeyListenDialog.tscn")
					.Instantiate<KeyListenDialog>();
				
				dlg.KeyListenAccepted += (key) =>
				{
					var action = item.GetMetadata(0).AsStringName();
					item.SetText(1, _keybindManager.DescribeKey(key));
					_keybindManager.UpdateKeybind(action, key);
					dlg.QueueFree();
				};
				dlg.KeyListenCancelled += () => dlg.QueueFree();
				
				GetTree().Root.AddChild(dlg);
				dlg.PopupCentered();
				// Handle Erase Shortcut
				break;
			case ERASE_SHORTCUT:
				// Handle Add Shortcut
				var action = item.GetMetadata(0).AsStringName();
				_keybindManager.ResetKeybind(action);
				item.SetText(1,_keybindManager.DescribeKey(action));
				break;
		}
	}
}
