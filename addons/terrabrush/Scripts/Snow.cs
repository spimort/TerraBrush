using System;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Snow : Node3D {
    private const float CompressSpeed = 10;
    private const float DeCompressSpeed = 0.05f;

    private ShaderMaterial _snowShader;
    private ImageTexture _currentCompressedImage;
    private System.Collections.Generic.Dictionary<Vector2I, float> _compressedPositions = new System.Collections.Generic.Dictionary<Vector2I, float>();

    [NodePath] private MeshInstance3D _snowMesh;

    [Export] public int TerrainSize { get;set; }
    [Export] public int TerrainSubDivision { get;set; }
    [Export] public Texture2D HeightMapTexture { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public Texture2D SnowTexture { get;set; }
    [Export] public SnowResource SnowDefinition { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        _snowShader = (ShaderMaterial) this._snowMesh.GetSurfaceOverrideMaterial(0);
    }

    public void UpdateSnow() {
        _snowMesh.Layers = (uint) SnowDefinition.VisualInstanceLayers;

        var planeMesh = (PlaneMesh) _snowMesh.Mesh;

        planeMesh.Size = new Vector2I(TerrainSize, TerrainSize);
        planeMesh.SubdivideWidth = TerrainSubDivision * 2;
        planeMesh.SubdivideDepth = TerrainSubDivision * 2;

        _snowShader.SetShaderParameter("SnowTexture", SnowTexture);
        _snowShader.SetShaderParameter("HeightMapTexture", HeightMapTexture);
        _snowShader.SetShaderParameter("SnowFactor", SnowDefinition.SnowFactor);
        _snowShader.SetShaderParameter("SnowInnerOffset", SnowDefinition.SnowInnerOffset);
        _snowShader.SetShaderParameter("HeightMapFactor", HeightMapFactor);
        _snowShader.SetShaderParameter("SnowColorTexture", SnowDefinition.SnowColorTexture);
        _snowShader.SetShaderParameter("SnowColorNormal", SnowDefinition.SnowColorNormal);
        _snowShader.SetShaderParameter("SnowColorRoughness", SnowDefinition.SnowColorRoughness);
        _snowShader.SetShaderParameter("SnowColorDetail", SnowDefinition.SnowColorDetail);
        _snowShader.SetShaderParameter("Noise", SnowDefinition.Noise);
        _snowShader.SetShaderParameter("NoiseFactor", SnowDefinition.NoiseFactor);
        _snowShader.SetShaderParameter("Metallic", SnowDefinition.Metallic);
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
            _snowShader.SetShaderParameter("CompressedSnowTexture", _currentCompressedImage);
        }

        var xPosition = (int) Math.Round(x);
        var yPosition = (int) Math.Round(y);

        var pixelPosition = new Vector2I(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2));
        _compressedPositions[pixelPosition] = 1;
    }
}

