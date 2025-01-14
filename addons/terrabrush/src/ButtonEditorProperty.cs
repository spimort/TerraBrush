using Godot;

namespace TerraBrush;

public partial class ButtonEditorProperty : EditorProperty {
    private GodotObject _originalObject;
    private string _functionName;

    public ButtonEditorProperty() {}

    public ButtonEditorProperty(string propertyName, GodotObject originalObject, string functionName) {
        _originalObject = originalObject;
        _functionName = functionName;

        var button = new Button() {
            Name = (StringName)propertyName,
            Text = propertyName
        };
        AddChild(button);

        button.Connect((StringName)"pressed", new Callable(this, (StringName)nameof(OnButtonClick)));
    }

    private void OnButtonClick() {
        var type = _originalObject.GetType();
        var methodInfo = type.GetMethod(_functionName);

        methodInfo?.Invoke(_originalObject, null);
    }
}
