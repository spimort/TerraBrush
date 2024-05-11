using System;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public static class DialogUtils {
	public static Task<float?> ShowNumericSelector(Node sourceNode, float defaultValue = 0, float? minValue = null, float? maxValue = null) {
		var completionSource = new TaskCompletionSource<float?>();
		var dialog = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/NumericSelectorDialog.tscn").Instantiate<NumericSelectorDialog>();
        dialog.MinValue = minValue;
        dialog.MaxValue = maxValue;

		dialog.NumericSelectorAccepted += (value) => {
			dialog.QueueFree();
			completionSource.TrySetResult(value);
		};

		dialog.NumericSelectorCancelled += () => {
			dialog.QueueFree();
			completionSource.TrySetResult(null);
		};

		sourceNode.GetTree().Root.AddChild(dialog);
		dialog.PopupCentered();

        dialog.SetValue(defaultValue);

		return completionSource.Task;
	}

    public static Task<bool> ShowConfirmDialog(Node soruceNode, string title, string content) {
        var completionSource = new TaskCompletionSource<bool>();

        var dialog = new ConfirmationDialog() {
            CancelButtonText = "Cancel",
            OkButtonText = "OK",
            DialogText = content,
            Title = title,
            Exclusive = true,
            Unresizable = true,
            Size = new Vector2I(300, 90),
        };

        soruceNode.GetTree().Root.AddChild(dialog);

        dialog.PopupCentered();

        Action onOkButton = null;
        onOkButton = () => {
            completionSource.TrySetResult(true);
            dialog.GetOkButton().Pressed -= onOkButton;
        };

        Action onCancelButton = null;
        onCancelButton = () => {
            completionSource.TrySetResult(false);
            dialog.GetCancelButton().Pressed -= onCancelButton;
        };

        dialog.GetOkButton().Pressed += onOkButton;
        dialog.GetCancelButton().Pressed += onCancelButton;

        return completionSource.Task;
	}
}
