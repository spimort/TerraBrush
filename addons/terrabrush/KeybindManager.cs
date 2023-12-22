using System.Collections.Generic;
using System.Linq;
using Godot;
using Godot.Collections;

namespace TerraBrush;

public partial class KeybindManager : RefCounted
{
	// EditorSettings Group: terrabrush
	private const string SettingsGroup = "terrabrush";
	
	#region InputMap Actions and associated Default Key Events
    private readonly StringName ToolPie = new StringName("terrabrush_tool_pie");
    private readonly InputEventKey ToolPieKey = new InputEventKey() { Keycode = Key.V };
	
    private readonly StringName BrushPie = new StringName("terrabrush_brush_pie");
    private readonly InputEventKey BrushPieKey = new InputEventKey() { Keycode = Key.B };
	
    private readonly StringName ToolContentPie = new StringName("terrabrush_tool_content_pie");
    private readonly InputEventKey ToolContentKey = new InputEventKey() { Keycode = Key.N };
	
    private readonly StringName BrushSizeSelector = new StringName("terrabrush_brush_size_selector");
    private readonly InputEventKey BrushSizeKey = new InputEventKey() { Keycode = Key.G };
	
    private readonly StringName BrushStrengthSelector = new StringName("terrabrush_brush_strength_selector");
    private readonly InputEventKey BrushStrengthKey = new InputEventKey() { Keycode = Key.H };
	
    private readonly StringName EscapeSelector = new StringName("terrabrush_brush_escape_selector");
    private readonly InputEventKey EscapeKey = new InputEventKey() { Keycode = Key.Escape };
    #endregion
    
    // Quick access to Action Manes and Default Keys to register with the Godot Editor
    public readonly List<StringName> ActionNames;
    public readonly System.Collections.Generic.Dictionary<StringName, InputEventKey> DefaultKeys;
    
    public KeybindManager()
    {
	    ActionNames = new List<StringName>()
	    {
		    ToolPie, BrushPie, ToolContentPie, BrushSizeSelector, BrushStrengthSelector, EscapeSelector
	    };
	    DefaultKeys = new System.Collections.Generic.Dictionary<StringName, InputEventKey>()
	    {
		    { ToolPie, ToolPieKey },
		    { BrushPie, BrushPieKey },
		    { ToolContentPie, ToolContentKey},
		    { BrushSizeSelector, BrushSizeKey },
		    { BrushStrengthSelector, BrushStrengthKey },
		    { EscapeSelector, EscapeKey }
	    };
    }

    public Array<InputEvent> GetBinding(StringName actionName) => InputMap.Singleton.ActionGetEvents(actionName);

    /// <summary>
    /// Register's the Action Names with the Godot Editor InputMap.
    /// </summary>
    public void RegisterInputMap()
    {
	    var im = InputMap.Singleton;
	    foreach (var action in ActionNames)
	    {
		    if (im.HasAction(action)) continue;	// We have the Action
			im.AddAction(action); // Register the Action, as we don't have the Action.
	    }
    }
    
    
    /// <summary>
    /// Load Keybinds from Godot Editor's EditorSettings File, for association with the InputMap.Actions.
    /// </summary>
	public void LoadEditorSettings()
	{
		var settings = EditorInterface.Singleton.GetEditorSettings();
		var im = InputMap.Singleton;
		var shortcuts = settings.GetSetting("shortcuts").AsGodotArray<Godot.Collections.Dictionary>();
		foreach (var action in ActionNames)
		{
			var binding = $"{SettingsGroup}/{action.ToString()}";
			if (shortcuts.Any(x => x["name"].AsString() == binding))
			{
				// Register Saved event in EditorSettings.
				var keyEvents = shortcuts.FirstOrDefault(x => x["name"].AsString() == binding);
				if (keyEvents is null) continue;
				var keys = keyEvents["shortcuts"].AsGodotArray<InputEventKey>();
				foreach (var key in keys)
				{
					im.ActionAddEvent(action, key);
				}
			}
			else
			{
				// Register Default Event
				im.ActionAddEvent(action, DefaultKeys[action]);
			}
		}
	}

    /// <summary>
    /// Saves the Keybinds to Godot Editor's EditorSettings file, for associated InputMap.Actions.
    /// </summary>
	public void SaveEditorSettings()
	{
		var im = InputMap.Singleton;
		var settings = EditorInterface.Singleton.GetEditorSettings();
		var shortcuts = settings.GetSetting("shortcuts").AsGodotArray<Godot.Collections.Dictionary>();
		foreach (var action in ActionNames)
		{
			var binding = $"{SettingsGroup}/{action.ToString()}";
			var keyEvents = shortcuts.FirstOrDefault(x => x["name"].AsString() == binding);
			if (keyEvents == null)
			{
				var keybind = new Godot.Collections.Dictionary<string, Variant>();
				keybind["name"] = binding;
				keybind["shortcuts"] = im.ActionGetEvents(action);
				shortcuts.Add((Godot.Collections.Dictionary)keybind);
			}
			else
			{
				keyEvents["shortcuts"] = im.ActionGetEvents(action);
			}
		}

		settings.SetSetting("shortcuts", shortcuts);
	}
}