#if TOOLS
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class ImportDialog : Window {
    private TerraBrush _previewTerrain;

    public TerraBrushTool OriginialTerraBrush { get;set; }

    [NodePath] private ImportImageRow _heightmapRow;
    [NodePath] private VBoxContainer _splatmapsContainer;
    [NodePath] private VBoxContainer _foliagesContainer;
    [NodePath] private VBoxContainer _objectsContainer;
    [NodePath] private ImportImageRow _waterRow;
    [NodePath] private ImportImageRow _snowRow;
    [NodePath] private SpinBox _heightmapScaleSpinBox;
    [NodePath] private CheckBox _useGreenChannelForHolesCheckbox;
    [NodePath] private CheckBox _scaleToResolutionCheckbox;
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

        var importRowPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ImportExport/ImportImageRow.tscn");

        // Splatmaps
        if (OriginialTerraBrush?.TextureSets?.TextureSets?.Length > 0) {
            var numberOfSplatmaps = Mathf.CeilToInt(OriginialTerraBrush.TextureSets.TextureSets.Length / 4.0f);
            for (int i = 0; i < numberOfSplatmaps; i++) {
                var importRow = importRowPrefab.Instantiate<ImportImageRow>();
                importRow.ImageTypeName = $"Splatmap {i + 1}";
                _splatmapsContainer.AddChild(importRow);
            }

            _splatmapsContainer.Visible = true;
        }

        // Foliages
        if (OriginialTerraBrush?.Foliages?.Length > 0) {
            for (int i = 0; i < OriginialTerraBrush.Foliages.Length; i++) {
                var importRow = importRowPrefab.Instantiate<ImportImageRow>();
                importRow.ImageTypeName = $"Foliage {i + 1}";
                _foliagesContainer.AddChild(importRow);
            }

            _foliagesContainer.Visible = true;
        }

        // Objects
        if (OriginialTerraBrush?.Objects?.Length > 0) {
            for (int i = 0; i < OriginialTerraBrush.Objects.Length; i++) {
                var importRow = importRowPrefab.Instantiate<ImportImageRow>();
                importRow.ImageTypeName = $"Object {i + 1}";
                _objectsContainer.AddChild(importRow);
            }

            _objectsContainer.Visible = true;
        }

        // Water
        if (OriginialTerraBrush?.WaterDefinition != null) {
            _waterRow.Visible = true;
        }

        // Snow
        if (OriginialTerraBrush?.SnowDefinition != null) {
            _snowRow.Visible = true;
        }

        _previewButton.Pressed += () => {
            var settings = GetImporterSettings();
            if (settings.Heightmap == null) {
                return;
            }

            _subViewportContainer.Visible = true;

            _previewTerrain?.QueueFree();
            _previewTerrain = new TerraBrush();
            _previewTerrain.ZonesSize = OriginialTerraBrush.ZonesSize;
            _previewTerrain.Resolution = OriginialTerraBrush.Resolution;
            ImporterEngine.ImportTerrain(_previewTerrain, settings);
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
    }

    private ImporterSettings GetImporterSettings() {
        return new ImporterSettings {
            Heightmap = _heightmapRow.ImageTexture,
            HeightmapScale = (float) _heightmapScaleSpinBox.Value,
            UseGreenChannelForHoles = _useGreenChannelForHolesCheckbox.ButtonPressed,
            ScaleToResolution = _scaleToResolutionCheckbox.ButtonPressed,
            Splatmaps = _splatmapsContainer.GetChildren().ToList()
                .Where(x => x is ImportImageRow importRow && importRow.ImageTexture != null)
                .Cast<ImportImageRow>()
                .Select(x => x.ImageTexture)
                .ToArray(),
            Foliages = _foliagesContainer.GetChildren().ToList()
                .Where(x => x is ImportImageRow importRow && importRow.ImageTexture != null)
                .Cast<ImportImageRow>()
                .Select(x => x.ImageTexture)
                .ToArray(),
            Objects = _objectsContainer.GetChildren().ToList()
                .Where(x => x is ImportImageRow importRow && importRow.ImageTexture != null)
                .Cast<ImportImageRow>()
                .Select(x => x.ImageTexture)
                .ToArray(),
            Water = _waterRow.ImageTexture,
            Snow = _snowRow.ImageTexture
        };
    }
}
#endif
