using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public static class DialogUtils {
	public static Task<float?> ShowNumericSelector(Node sourceNode, float defaultValue = 0) {
		var completionSource = new TaskCompletionSource<float?>();
		var dialog = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/NumericSelectorDialog.tscn").Instantiate<NumericSelectorDialog>();

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
}
