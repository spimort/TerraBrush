using System;
using System.Linq;
using Godot;

namespace TerraBrush;

public enum FoliageStrategy {
    MultiMesh = 1,
    GPUParticle = 2
}

[Tool]
public partial class Foliage : Node3D {
    private ShaderMaterial _foliageShader;
    private Vector3 _lastUpdatedPosition = Vector3.Zero;
    private MultiMeshInstance3D _multiMeshInstance3D;
    private Texture2D _defaultNoise;

    [NodePath] private GpuParticles3D _particles;

    [Export] public int FoliageIndex { get;set; }
    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public float WaterFactor { get;set; }
    [Export] public FoliageDefinitionResource Definition { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        if (Definition == null) {
            return;
        }

        if (Definition.Strategy == FoliageStrategy.MultiMesh)  {
            _multiMeshInstance3D = new MultiMeshInstance3D();
            AddChild(_multiMeshInstance3D);

            ShaderMaterial shaderMaterial;
            if (Definition.CustomShader == null) {
                shaderMaterial = new ShaderMaterial() {
                    Shader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader")
                };
            } else {
                shaderMaterial = Utils.CreateCustomShaderCopy(Definition.CustomShader);
            }

            _multiMeshInstance3D.MaterialOverride = shaderMaterial;
            _foliageShader = shaderMaterial;
        } else {
            this._foliageShader = (ShaderMaterial) this._particles.ProcessMaterial;
        }

        _defaultNoise = ResourceLoader.Load<Texture2D>("res://addons/terrabrush/Resources/DefaultNoise.tres");

        UpdateFoliage();
        UpdateAABB();
        UpdateShaderOffsetPosition();

        SetNotifyTransform(true);
    }

    public override void _Process(double delta) {
        if (!Engine.IsEditorHint()) {
            var position = GetViewport()?.GetCamera3D()?.GlobalPosition ?? Vector3.Zero;

            UpdateFoliagePosition(position);
        }
    }

    public override void _Notification(int what) {
        base._Notification(what);

        if (what == NotificationTransformChanged) {
            UpdateShaderOffsetPosition();
        }
    }

    private async void UpdateFoliage() {
        if ((_particles == null && _multiMeshInstance3D == null) || TerrainZones == null) {
            return;
        }

        var numberOfLevels = Definition.LODLevels;
        var rowsPerLevel = Definition.LODRowsPerLevel;
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

        if (Definition.Strategy == FoliageStrategy.MultiMesh) {
            _particles.Visible = false;

            _multiMeshInstance3D.Layers = (uint) Definition.VisualInstanceLayers;
            _multiMeshInstance3D.Multimesh = new MultiMesh() {
                TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
                Mesh = Definition.Mesh,
                InstanceCount = numberOfPoints,
                Buffer = Enumerable.Range(0, numberOfPoints).Select(_ => {
                    return new float[] {
                        1f, 0f, 0f, 0f,
                        0f, 1f, 0f, 0f,
                        0f, 0f, 1f, 0f
                    };
                }).SelectMany(x => x).ToArray()
            };

            if (Definition.CastShadow) {
                _multiMeshInstance3D.CastShadow = GeometryInstance3D.ShadowCastingSetting.On;
            } else {
                _multiMeshInstance3D.CastShadow = GeometryInstance3D.ShadowCastingSetting.Off;
            }

            _foliageShader.SetShaderParameter(StringNames.InitialCellWidth, Definition.LODInitialCellWidth);
            _foliageShader.SetShaderParameter(StringNames.LODRowsPerLevel, Definition.LODRowsPerLevel);
            _foliageShader.SetShaderParameter(StringNames.LODLevels, Definition.LODLevels);
            _foliageShader.SetShaderParameter(StringNames.FoliageAlbedo, Definition.Albedo);

            if (Definition.AlbedoTextures?.Length > 0) {
                var albedoTextures = new Texture2DArray();
                albedoTextures.CreateFromImages(new Godot.Collections.Array<Image>(Definition.AlbedoTextures.Select(x => x.GetImage())));
                _foliageShader.SetShaderParameter(StringNames.FoliageAlbedoTextures, albedoTextures);
                _foliageShader.SetShaderParameter(StringNames.FoliageNumberOfTexture, Definition.AlbedoTextures.Length);
            }

            _foliageShader.SetShaderParameter(StringNames.UseGroundColor, Definition.UseGroundColor);
            _foliageShader.SetShaderParameter(StringNames.UseBrushScale, Definition.UseBrushScale);
            _foliageShader.SetShaderParameter(StringNames.ScaleNoiseTexture, Definition.ScaleNoiseTexture);
            _foliageShader.SetShaderParameter(StringNames.RandomPlacementRange, Definition.RandomPlacementRange);
            _foliageShader.SetShaderParameter(StringNames.ApplyOnTextureIndexes, Definition.ApplyOnTextureIndexes ?? new int[] {});
            _foliageShader.SetShaderParameter(StringNames.NumberOfTexturesToApplyOn, Definition.ApplyOnTextureIndexes?.Length ?? 0);
        } else {
            _particles.Visible = true;

            this._particles.Layers = (uint) Definition.VisualInstanceLayers;
            this._particles.DrawPass1 = Definition.Mesh;
            this._particles.MaterialOverride = Definition.MeshMaterial;
            this._particles.Amount = numberOfPoints;

            if (Engine.IsEditorHint()) {
                this._particles.Amount = Definition.EditorMaximumRenderDistance * Definition.EditorMaximumRenderDistance;

                this._foliageShader.SetShaderParameter(StringNames.MaximumDistance, Definition.EditorMaximumRenderDistance);
            } else {
                this._particles.Amount = Definition.MaximumRenderDistance * Definition.MaximumRenderDistance;

                this._foliageShader.SetShaderParameter(StringNames.MaximumDistance, Definition.MaximumRenderDistance);
            }
        }

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
        _foliageShader.SetShaderParameter(StringNames.MeshScale, Definition.MeshScale);
        _foliageShader.SetShaderParameter(StringNames.WindStrength, Definition.WindStrength);

        _foliageShader.SetShaderParameter(StringNames.WaterTextures, TerrainZones.WaterTextures);
        _foliageShader.SetShaderParameter(StringNames.WaterFactor, this.WaterFactor);

        var noiseTexture = Definition.NoiseTexture != null ? await Utils.WaitForTextureReady(Definition.NoiseTexture) : _defaultNoise;
        if (noiseTexture != null) {

            var noiseImage = new Image();
            noiseImage.CopyFrom(noiseTexture.GetImage());
            noiseImage.Resize(this.ZonesSize, this.ZonesSize);

            this._foliageShader.SetShaderParameter(StringNames.NoiseTexture, ImageTexture.CreateFromImage(noiseImage));
        }
    }

    public void UpdateEditorCameraPosition(Camera3D viewportCamera) {
        UpdateFoliagePosition(viewportCamera.GlobalPosition);
    }

    private void UpdateFoliagePosition(Vector3 position) {
        var offset = 0.0f;
        var isEven = ZonesSize % 2 == 0;
        if (isEven) {
            offset = Definition.LODInitialCellWidth / 2.0f;
        }

        var xPosition = ((int) Math.Floor(position.X)) + offset;
        var zPosition = ((int) Math.Floor(position.Z)) + offset;

        var maxCellWidth = Definition.LODInitialCellWidth * Mathf.Pow(2, Definition.LODLevels - 1);

        xPosition -= xPosition % maxCellWidth;
        zPosition -= zPosition % maxCellWidth;

        if (isEven) {
            xPosition -= Definition.LODInitialCellWidth / 2.0f;
            zPosition -= Definition.LODInitialCellWidth / 2.0f;
        }

        var newPosition = new Vector3(xPosition, 0, zPosition);
        if (newPosition.DistanceTo(_lastUpdatedPosition) > maxCellWidth) {
            this._foliageShader.SetShaderParameter(StringNames.GlobalPosition, newPosition);
            _lastUpdatedPosition = newPosition;
        }
    }

    public void UpdateAABB() {
        if ((_particles == null && _multiMeshInstance3D == null) || TerrainZones == null) {
            return;
        }

        var zonePositions = TerrainZones.Zones.Select(zone => zone.ZonePosition).ToArray();
        var maxX = zonePositions.Max(x => Math.Abs(x.X)) + 1;
        var maxY = zonePositions.Max(x => Math.Abs(x.Y)) + 1;

        var aabbXSize = Math.Max(maxX * ZonesSize * 2, ZonesSize * 2);
        var aabbYSize = Math.Max(maxY * ZonesSize * 2, ZonesSize * 2);
        var aabbXPoint = -(aabbXSize / 2);
        var aabbYPoint = -(aabbYSize / 2);

        var aabb = new Aabb(new Vector3(aabbXPoint, Math.Max(aabbXPoint, aabbYPoint), aabbYPoint), new Vector3(aabbXSize, Math.Max(aabbXSize, aabbYSize), aabbYSize));
        if (Definition.Strategy == FoliageStrategy.MultiMesh) {
            _multiMeshInstance3D.CustomAabb = aabb;
        } else {
            _particles.CustomAabb = aabb;
        }
    }

    private void UpdateShaderOffsetPosition() {
        _foliageShader.SetShaderParameter(StringNames.OffsetPosition, GlobalPosition);
    }
}
