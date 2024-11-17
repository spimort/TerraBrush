#if TOOLS
using Godot;

namespace TerraBrush;

[Tool]
public partial class ImportDialog : Window {
    private TerraBrush _previewTerrain;

    [NodePath] private ImportImageRow _heightmapRow;
    [NodePath] private SpinBox _minHeightSpinBox;
    [NodePath] private SpinBox _maxHeightSpinBox;
    [NodePath] private CheckBox _customMinMaxHeightCheckbox;
	[NodePath] private Button _previewButton;
	[NodePath] private Button _okButton;
	[NodePath] private Button _cancelButton;
	[NodePath] private SubViewport _subViewport;
	[NodePath] private SubViewportContainer _subViewportContainer;

    [Signal]
	public delegate void AcceptedEventHandler(ImporterSettings settings);

	[Signal]
	public delegate void CancelledEventHandler();

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        _previewButton.Pressed += () => {
            _subViewportContainer.Visible = true;

            _previewTerrain?.QueueFree();
            _previewTerrain = new TerraBrush();
            ImporterEngine.ImportTerrain(_previewTerrain, GetImporterSettings());
            _subViewport.AddChild(_previewTerrain);
        };

		_okButton.Pressed += () => {
			EmitSignal(SignalName.Accepted, GetImporterSettings());
		};
		_cancelButton.Pressed += () => {
            EmitSignal(SignalName.Cancelled);
        };
		CloseRequested += () => {
            EmitSignal(SignalName.Cancelled);
        };

        _customMinMaxHeightCheckbox.Pressed += () => {
            if (_customMinMaxHeightCheckbox.ButtonPressed) {
                _minHeightSpinBox.Editable = true;
                _maxHeightSpinBox.Editable = true;
            } else {
                _minHeightSpinBox.Editable = false;
                _maxHeightSpinBox.Editable = false;
            }
        };
    }

    private ImporterSettings GetImporterSettings() {
        return new ImporterSettings {
            Heightmap = _heightmapRow.ImageTexture,
            MinHeight = _customMinMaxHeightCheckbox.ButtonPressed ? (float) _minHeightSpinBox.Value : null,
            MaxHeight = _customMinMaxHeightCheckbox.ButtonPressed ? (float) _maxHeightSpinBox.Value : null
        };
    }
}
#endif
