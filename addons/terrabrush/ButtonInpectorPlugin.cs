#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class ButtonInspectorPlugin : EditorInspectorPlugin {
    public const string ButtonInspectorHintString = "ButtonHint";

    public override bool _CanHandle(GodotObject @object) {
        return @object is TerraBrush;
    }

    public override bool _ParseProperty(GodotObject @object, Variant.Type type, string name, PropertyHint hintType, string hintString, PropertyUsageFlags usageFlags, bool wide) {
        if (hintString != null && hintString.StartsWith(ButtonInspectorHintString)) {
            var splitValues = hintString.Split("_");
            if (splitValues.Length == 2) {
                var functionName = splitValues[1];
                AddPropertyEditor(name, new ButtonEditorProperty(name, @object, functionName));
                return true;
            }
        }

        return base._ParseProperty(@object, type, name, hintType, hintString, usageFlags, wide);
    }
}
#endif
