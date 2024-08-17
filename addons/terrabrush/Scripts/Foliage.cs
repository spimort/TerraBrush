using System;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Foliage : Node3D {
    private ShaderMaterial _foliageShader;
    private Vector3 _lastUpdatedPosition = Vector3.Zero;

    [NodePath] private GpuParticles3D _particles;

    [Export] public int FoliageIndex { get;set; }
    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public Mesh Mesh { get;set; }
    [Export] public Vector3 MeshScale { get;set; }
    // [Export] public int MaximumRenderDistance { get;set; }
    // [Export] public int EditorMaximumRenderDistance { get;set; }
    [Export] public int LODLevels { get;set; } = 8;
    [Export] public int LODRowsPerLevel { get;set; } = 21;
    [Export] public float LODInitialCellWidth { get;set; } = 1;
    [Export] public float WindStrength { get;set; }
    [Export] public Material MeshMaterial { get;set; }
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        this._foliageShader = (ShaderMaterial) this._particles.ProcessMaterial;

        UpdateFoliage();
    }

    public override void _Process(double delta) {
        if (!Engine.IsEditorHint()) {
            var position = this.GetViewport()?.GetCamera3D()?.GlobalPosition ?? Vector3.Zero;

            UpdateFoliagePosition(position);
        }
    }

    public void UpdateFoliage() {
        if (_particles == null || TerrainZones == null) {
            return;
        }

        this._particles.Layers = (uint) VisualInstanceLayers;
        this._particles.DrawPass1 = this.Mesh;
        this._particles.MaterialOverride = this.MeshMaterial;

        _foliageShader.SetShaderParameter(StringNames.HeightmapTextures, TerrainZones.HeightmapTextures);
        _foliageShader.SetShaderParameter(StringNames.ZonesSize, (float) ZonesSize);
        _foliageShader.SetShaderParameter(StringNames.NumberOfZones, (float) TerrainZones.Zones.Count());
		_foliageShader.SetShaderParameter(StringNames.ZonesMap, TerrainZones.ZonesMap);

        if (TextureSets?.TextureSets != null) {
            _foliageShader.SetShaderParameter(StringNames.Splatmaps, TerrainZones.SplatmapsTextures);
            _foliageShader.SetShaderParameter(StringNames.Textures, Utils.TexturesToTextureArray(TextureSets.TextureSets.Select(x => x.AlbedoTexture)));
            _foliageShader.SetShaderParameter(StringNames.NumberOfTextures, TextureSets?.TextureSets?.Count() ?? 0);
    		_foliageShader.SetShaderParameter(StringNames.TextureDetail, this.TextureDetail);
        }

        _foliageShader.SetShaderParameter(StringNames.FoliageTextures, TerrainZones.FoliagesTextures[FoliageIndex]);
        _foliageShader.SetShaderParameter(StringNames.MeshScale, this.MeshScale);
        _foliageShader.SetShaderParameter(StringNames.WindStrength, this.WindStrength);

        _foliageShader.SetShaderParameter(StringNames.WaterTextures, TerrainZones.WaterTextures);
        _foliageShader.SetShaderParameter(StringNames.WaterFactor, this.WaterFactor);

        if (NoiseTexture != null) {
            var noiseImage = new Image();
            noiseImage.CopyFrom(NoiseTexture.GetImage());
            noiseImage.Resize(this.ZonesSize, this.ZonesSize);

            this._foliageShader.SetShaderParameter(StringNames.NoiseTexture, ImageTexture.CreateFromImage(noiseImage));
        }

        var numberOfLevels = LODLevels;
        var rowsPerLevel = LODRowsPerLevel;
        if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
            rowsPerLevel += 1;
        }

        var numberOfPointsFirstLevel = (rowsPerLevel - (-1 - rowsPerLevel)) + 2;
        var center = numberOfPointsFirstLevel * numberOfPointsFirstLevel;

        var topBottomLines = (rowsPerLevel + 1) / 2;
        var numberOfPointsOtherLevel = (rowsPerLevel - (-1 - rowsPerLevel)) + 2;
        var topBottom = (numberOfPointsOtherLevel * topBottomLines * 2) * numberOfLevels;

        var numberOfSides = numberOfPointsFirstLevel - (topBottomLines * 2);
        var numberOfSidesPoints = (rowsPerLevel + 1) / 2;
        var sides = numberOfSides * numberOfSidesPoints * 2 * numberOfLevels;

        var numberOfPoints = center + topBottom + sides;
        this._particles.Amount = numberOfPoints;

        _foliageShader.SetShaderParameter(StringNames.InitialCellWidth, LODInitialCellWidth);
        _foliageShader.SetShaderParameter(StringNames.LODRowsPerLevel, LODRowsPerLevel);
        _foliageShader.SetShaderParameter(StringNames.LODLevels, LODLevels);

        _foliageShader.SetShaderParameter(StringNames.NumberOfParticles, _particles.Amount);
    }

    public void UpdateEditorCameraPosition(Camera3D viewportCamera) {
        UpdateFoliagePosition(viewportCamera.GlobalPosition);
    }

    private void UpdateFoliagePosition(Vector3 position) {
        var offset = 0.0f;
        var isEven = ZonesSize % 2 == 0;
        if (isEven) {
            offset = LODInitialCellWidth / 2.0f;
        }

        var xPosition = ((int) Math.Floor(position.X)) + offset;
        var zPosition = ((int) Math.Floor(position.Z)) + offset;

        var maxCellWidth = LODInitialCellWidth * Mathf.Pow(2, LODLevels - 1);

        xPosition -= xPosition % maxCellWidth;
        zPosition -= zPosition % maxCellWidth;

        if (isEven) {
            xPosition -= LODInitialCellWidth / 2.0f;
            zPosition -= LODInitialCellWidth / 2.0f;
        }

        var newPosition = new Vector3(xPosition, 0, zPosition);
        if (newPosition.DistanceTo(_lastUpdatedPosition) > maxCellWidth) {
            this._foliageShader.SetShaderParameter(StringNames.GlobalPosition, newPosition);
            _lastUpdatedPosition = newPosition;
        }
    }
}
