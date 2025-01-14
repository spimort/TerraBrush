#if TOOLS
using System;
using System.IO;
using System.Threading.Tasks;
using Godot;
using static Godot.EditorFileDialog;

namespace TerraBrush;

public static class DialogUtils {
    public static Task<string> ShowFileDialog(Node sourceNode, AccessEnum access = AccessEnum.Filesystem, FileModeEnum fileMode = FileModeEnum.OpenFile, string[] filters = null, bool transient = false) {
        var completionSource = new TaskCompletionSource<string>();
        var fileDialog = new EditorFileDialog {
            Access = access,
            FileMode = fileMode,
            Filters = filters,
            Exclusive = true,
            Transient = transient
        };

        fileDialog.Connect("file_selected", Callable.From((string file) => {
            fileDialog.QueueFree();
            completionSource.TrySetResult(file);
        }));

        fileDialog.Connect("dir_selected", Callable.From((string dir) => {
            fileDialog.QueueFree();
            completionSource.TrySetResult(dir);
        }));

        fileDialog.Connect("canceled", Callable.From(() => {
            fileDialog.QueueFree();
            completionSource.TrySetResult(null);
        }));

		fileDialog.Connect("close_requested", Callable.From(() => {
			fileDialog.QueueFree();
			completionSource.TrySetResult(null);
		}));

		sourceNode.AddChild(fileDialog);
		fileDialog.PopupCentered(new Vector2I(800, 600));

		return completionSource.Task;
	}

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

    public static Task<bool> ShowConfirmDialog(Node sourceNode, string title, string content) {
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

        sourceNode.GetTree().Root.AddChild(dialog);

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

	public static Task<ImporterSettings> ShowImportDialog(Node sourceNode, TerraBrushTool originalTerraBursh) {
		var completionSource = new TaskCompletionSource<ImporterSettings?>();
		var dialog = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ImportExport/ImportDialog.tscn").Instantiate<ImportDialog>();
		dialog.OriginialTerraBrush = originalTerraBursh;

		dialog.Accepted += settings => {
			dialog.QueueFree();
			completionSource.TrySetResult(settings);
		};

		dialog.Cancelled += () => {
			dialog.QueueFree();
			completionSource.TrySetResult(null);
		};

		sourceNode.GetTree().Root.AddChild(dialog);
		dialog.PopupCentered();

		return completionSource.Task;
	}
}
#endif
