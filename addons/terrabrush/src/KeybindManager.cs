#if TOOLS
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Godot;
using Godot.Collections;

namespace TerraBrush;

public partial class KeybindManager : RefCounted {
	// EditorSettings Group: terrabrush
	private const string SettingsGroup = "terrabrush";

	#region InputMap Actions and associated Default Key Events
	private readonly InputEventKey ToolPieKey = new InputEventKey() { Keycode = Key.V };
    private readonly InputEventKey BrushPieKey = new InputEventKey() { Keycode = Key.B };
    private readonly InputEventKey ToolContentKey = new InputEventKey() { Keycode = Key.N };
    private readonly InputEventKey BrushSizeKey = new InputEventKey() { Keycode = Key.G };
    private readonly InputEventKey BrushStrengthKey = new InputEventKey() { Keycode = Key.H };
    private readonly InputEventKey EscapeKey = new InputEventKey() { Keycode = Key.Escape };
    private readonly InputEventKey ToggleAutoAddZonesKey = new InputEventKey() { Keycode = Key.K };
    private readonly InputEventKey LockXAxis = new InputEventKey() { Keycode = Key.X };
    private readonly InputEventKey LockZAxis = new InputEventKey() { Keycode = Key.Z };
    #endregion

    #region Public String Names

    public static class StringNames
    {
	    public static readonly StringName ToolPie = new StringName("terrabrush_tool_pie");
	    public static readonly StringName BrushPie = new StringName("terrabrush_brush_pie");
	    public static readonly StringName ToolContentPie = new StringName("terrabrush_tool_content_pie");
	    public static readonly StringName BrushSizeSelector = new StringName("terrabrush_brush_size_selector");
	    public static readonly StringName BrushStrengthSelector = new StringName("terrabrush_brush_strength_selector");
	    public static readonly StringName EscapeSelector = new StringName("terrabrush_brush_escape_selector");
	    public static readonly StringName ToggleAutoAddZones = new StringName("terrabrush_toggle_auto_add_zones");
	    public static readonly StringName LockXAxis = new StringName("terrabrush_lock_x_axis");
	    public static readonly StringName LockZAxis = new StringName("terrabrush_lock_z_axis");
    }
    #endregion

	private readonly List<StringName> _actionNames;
	private readonly System.Collections.Generic.Dictionary<StringName, InputEventKey> _defaultKeys;

    // Quick access to Action Manes and Default Keys to register with the Godot Editor
    public List<StringName> ActionNames => _actionNames;
    public System.Collections.Generic.Dictionary<StringName, InputEventKey> DefaultKeys => _defaultKeys;

    public KeybindManager() {
	    _actionNames = new List<StringName>() {
		    StringNames.ToolPie,
			StringNames.BrushPie,
			StringNames.ToolContentPie,
		    StringNames.BrushSizeSelector,
			StringNames.BrushStrengthSelector,
			StringNames.EscapeSelector,
			StringNames.ToggleAutoAddZones,
			StringNames.LockXAxis,
			StringNames.LockZAxis,
	    };
	    _defaultKeys = new System.Collections.Generic.Dictionary<StringName, InputEventKey>() {
		    { StringNames.ToolPie, ToolPieKey },
		    { StringNames.BrushPie, BrushPieKey },
		    { StringNames.ToolContentPie, ToolContentKey},
		    { StringNames.BrushSizeSelector, BrushSizeKey },
		    { StringNames.BrushStrengthSelector, BrushStrengthKey },
		    { StringNames.EscapeSelector, EscapeKey },
		    { StringNames.ToggleAutoAddZones, ToggleAutoAddZonesKey },
		    { StringNames.LockXAxis, LockXAxis },
		    { StringNames.LockZAxis, LockZAxis },
	    };
    }

    public static Array<InputEvent> GetBinding(StringName actionName) => InputMap.Singleton.ActionGetEvents(actionName);

    public static string DescribeKey(InputEventKey key) {
	    var desc = new StringBuilder();
	    if (key.CtrlPressed) desc.Append("Ctrl +");
	    if (key.AltPressed) desc.Append("Alt +");
	    desc.Append(key.Keycode.ToString());
	    return desc.ToString();
    }

    /// <summary>
    /// Register's the Action Names with the Godot Editor InputMap.
    /// </summary>
    public void RegisterInputMap() {
	    var im = InputMap.Singleton;
	    foreach (var action in ActionNames) {
			if (!im.HasAction(action)) {
				im.AddAction(action); // Register the Action, as we don't have the Action.
		    }
	    }
    }

    /// <summary>
    /// Load Keybinds from Godot Editor's EditorSettings File, for association with the InputMap.Actions.
    /// </summary>
	public void LoadEditorSettings() {
		var settings = EditorInterface.Singleton.GetEditorSettings();
		var im = InputMap.Singleton;
		var shortcuts = settings.GetSetting("shortcuts").AsGodotArray<Godot.Collections.Dictionary>();
		foreach (var action in ActionNames) {
			var binding = $"{SettingsGroup}/{action}";
			var keyEvents = shortcuts.FirstOrDefault(x => x["name"].AsString() == binding);
			if (keyEvents != null) {
				// Register Saved event in EditorSettings.
				var keys = keyEvents["shortcuts"].AsGodotArray<InputEventKey>();
				foreach (var key in keys) {
					im.ActionAddEvent(action, key);
				}
			} else {
				// Register Default Event
				im.ActionAddEvent(action, DefaultKeys[action]);
			}
		}
	}

    /// <summary>
    /// Saves the Keybinds to Godot Editor's EditorSettings file, for associated InputMap.Actions.
    /// </summary>
	public void SaveEditorSettings() {
		var im = InputMap.Singleton;
		var settings = EditorInterface.Singleton.GetEditorSettings();
		var shortcuts = settings.GetSetting("shortcuts").AsGodotArray<Godot.Collections.Dictionary>();
		foreach (var action in ActionNames) {
			var binding = $"{SettingsGroup}/{action}";
			var keyEvents = shortcuts.FirstOrDefault(x => x["name"].AsString() == binding);
			if (keyEvents == null) {
				var keybind = new Godot.Collections.Dictionary<string, Variant>();
				keybind["name"] = binding;
				keybind["shortcuts"] = im.ActionGetEvents(action);
				shortcuts.Add((Godot.Collections.Dictionary)keybind);
			}
			else {
				keyEvents["shortcuts"] = im.ActionGetEvents(action);
			}
		}

		settings.SetSetting("shortcuts", shortcuts);
	}

	public void UpdateKeybind(StringName action, InputEventKey key) {
		var im = InputMap.Singleton;
		im.ActionEraseEvents(action);
		im.ActionAddEvent(action, key);
	}

	public void ResetKeybind(StringName action) {
		if (!ActionNames.Contains(action)) return;
		var im = InputMap.Singleton;
		im.ActionEraseEvents(action);
		im.ActionAddEvent(action, DefaultKeys[action]);
	}

	public string DescribeKey(StringName action) {
		var key = InputMap.Singleton.ActionGetEvents(action);
		return DescribeKey((InputEventKey)key[0]);
	}
}
#endif
