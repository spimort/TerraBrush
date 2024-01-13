using System;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Snow : Node3D {
    private const float CompressSpeed = 10;
    private const float DeCompressSpeed = 0.05f;

    private ImageTexture _currentCompressedImage;
    private System.Collections.Generic.Dictionary<Vector2I, float> _compressedPositions = new System.Collections.Generic.Dictionary<Vector2I, float>();

    [NodePath] private Clipmap _clipmap;

    [Export] public int TerrainSize { get;set; }
    [Export] public int TerrainSubDivision { get;set; }
    [Export] public Texture2D HeightMapTexture { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public Texture2D SnowTexture { get;set; }
    [Export] public SnowResource SnowDefinition { get;set; }
    [Export] public int LODLevels { get;set; } = 8;
    [Export] public int LODRowsPerLevel { get;set; } = 21;
    [Export] public float LODInitialCellWidth { get;set; } = 1;

    public Clipmap Clipmap => _clipmap;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }

    public void UpdateSnow() {
        if (_clipmap == null) {
            return;
        }

        _clipmap.ClipmapMesh.Layers = (uint) SnowDefinition.VisualInstanceLayers;
        _clipmap.Heightmap = HeightMapTexture;
        _clipmap.HeightmapFactor = HeightMapFactor;
        _clipmap.Levels = LODLevels;
        _clipmap.RowsPerLevel = LODRowsPerLevel;
        _clipmap.InitialCellWidth = LODInitialCellWidth;

        _clipmap.CreateMesh();

        _clipmap.Shader.SetShaderParameter("SnowTexture", SnowTexture);
        _clipmap.Shader.SetShaderParameter("SnowFactor", SnowDefinition.SnowFactor);
        _clipmap.Shader.SetShaderParameter("SnowInnerOffset", SnowDefinition.SnowInnerOffset);
        _clipmap.Shader.SetShaderParameter("HeightMapFactor", HeightMapFactor);
        _clipmap.Shader.SetShaderParameter("SnowColorTexture", SnowDefinition.SnowColorTexture);
        _clipmap.Shader.SetShaderParameter("SnowColorNormal", SnowDefinition.SnowColorNormal);
        _clipmap.Shader.SetShaderParameter("SnowColorRoughness", SnowDefinition.SnowColorRoughness);
        _clipmap.Shader.SetShaderParameter("SnowColorDetail", SnowDefinition.SnowColorDetail);
        _clipmap.Shader.SetShaderParameter("Noise", SnowDefinition.Noise);
        _clipmap.Shader.SetShaderParameter("NoiseFactor", SnowDefinition.NoiseFactor);
        _clipmap.Shader.SetShaderParameter("Metallic", SnowDefinition.Metallic);
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        if (_compressedPositions.Count > 0) {
            var compressedSnowImage = _currentCompressedImage.GetImage();

            for (var i = _compressedPositions.Count - 1; i >= 0; i--) {
                var position = _compressedPositions.Keys.ElementAt(i);
                var pixel = compressedSnowImage.GetPixel(position.X, position.Y);
                if (pixel.G <= 0) {
                    pixel.R += (float) delta * CompressSpeed;

                    if (pixel.R >= 1.0) {
                        pixel.G = 1.0f;
                    }
                } else {
                    pixel.R -= (float) delta * DeCompressSpeed;

                    if (pixel.R <= 0.0) {
                        _compressedPositions.Remove(position);
                        pixel = new Color(0, 0, 0, 0);
                    }
                }

                compressedSnowImage.SetPixel(position.X, position.Y, pixel);
            }

            _currentCompressedImage.Update(compressedSnowImage);
        }
    }

    public void AddCompressedSnow(float x, float y) {
        if (_currentCompressedImage == null) {
            var image = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);
            _currentCompressedImage = ImageTexture.CreateFromImage(image);
            _clipmap.Shader.SetShaderParameter("CompressedSnowTexture", _currentCompressedImage);
        }

        var xPosition = (int) Math.Round(x);
        var yPosition = (int) Math.Round(y);

        var pixelPosition = new Vector2I(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2));
        _compressedPositions[pixelPosition] = 1;
    }
}

