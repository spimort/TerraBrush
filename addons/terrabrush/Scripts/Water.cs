using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Water : Node3D {
    private const float RippleRatio = 20;
    private const float RippleResetSpeed = 0.9f;

    private ShaderMaterial _waterShader;
    private ShaderMaterial _rippleShader;
    private ShaderMaterial _rippleBufferShader;
    private ImageTexture _rippleImage;
    private Dictionary<Vector2I, float> _ripplePositions = new Dictionary<Vector2I, float>();

    [NodePath] private MeshInstance3D _waterMesh;
    [NodePath] private SubViewport _rippleViewport;
    [NodePath] private SubViewport _rippleBufferViewport;
    [NodePath] private ColorRect _rippleColorRect;
    [NodePath] private ColorRect _rippleBufferColorRect;

    [Export] public int TerrainSize { get;set; }
    [Export] public int TerrainSubDivision { get;set; }
    [Export] public Texture2D WaterTexture { get;set; }
    [Export] public Texture2D HeightMapTexture { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public float WaterInnerOffset { get;set; }
    [Export(PropertyHint.ColorNoAlpha)] public Color WaterColor { get;set; }
    [Export(PropertyHint.ColorNoAlpha)] public Color FresnelColor { get;set; }
    [Export(PropertyHint.Range, "0,1,0.001")] public float Metallic { get;set; }
    [Export(PropertyHint.Range, "0,1,0.001")] public float Roughness { get;set; }
    [Export] public Texture2D NormalMap { get;set; }
    [Export] public Texture2D NormalMap2 { get;set; }
    [Export] public float TimeScale { get;set; } = 0.1f;
    [Export] public float Strength { get;set; } = 0.4f;
    [Export] public Texture2D Wave { get;set; }
    [Export] public float NoiseScale { get;set; }
    [Export] public float HeightScale { get;set; }
    [Export] public Color ColorDeep { get;set; }
    [Export] public Color ColorShallow { get;set; }
    [Export] public float BeersLaw { get;set; }
    [Export] public float DepthOffset { get;set; }
    [Export] public float EdgeScale { get;set; }
    [Export] public float Near { get;set; }
    [Export] public float Far { get;set; }
    [Export(PropertyHint.ColorNoAlpha)] public Color EdgeColor { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        _waterMesh.Layers = (uint) VisualInstanceLayers;
        _waterShader = (ShaderMaterial) _waterMesh.GetSurfaceOverrideMaterial(0);
        _rippleShader = (ShaderMaterial) _rippleColorRect.Material;
        _rippleBufferShader = (ShaderMaterial) _rippleBufferColorRect.Material;

        if (!Engine.IsEditorHint()) {
            _rippleShader.SetShaderParameter("DoubleBufferTexture", _rippleBufferViewport.GetTexture());
            _rippleBufferShader.SetShaderParameter("RippleTexture", _rippleViewport.GetTexture());
            _waterShader.SetShaderParameter("WaterRippleTexture", _rippleViewport.GetTexture());
        }
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        if (_ripplePositions.Count > 0) {
            var rippleImage = _rippleImage.GetImage();

            for (var i = _ripplePositions.Count - 1; i >= 0; i--) {
                var position = _ripplePositions.Keys.ElementAt(i);
                var value = _ripplePositions.Values.ElementAt(i);

                value -= (float) delta * RippleResetSpeed;
                _ripplePositions[position] = value;

                if (value <= 0.0) {
                    _ripplePositions.Remove(position);
                    rippleImage.SetPixel(position.X, position.Y, Colors.Black);
                }
            }

            _rippleImage.Update(rippleImage);
        }

        if (!Engine.IsEditorHint()) {
            if (_rippleViewport != null) {
                _rippleViewport.RenderTargetUpdateMode = SubViewport.UpdateMode.Once;
            }

            if (_rippleBufferViewport != null) {
                _rippleBufferViewport.RenderTargetUpdateMode = SubViewport.UpdateMode.Once;
            }
        }
    }

    public void UpdateWater() {
        var rippleTextureSize = (int) (TerrainSize * RippleRatio);

        var planeMesh = (PlaneMesh) _waterMesh.Mesh;
        planeMesh.Size = new Vector2I(TerrainSize, TerrainSize);
        planeMesh.SubdivideWidth = TerrainSubDivision;
        planeMesh.SubdivideDepth = TerrainSubDivision;

        _waterShader.SetShaderParameter("HeightMapTexture", HeightMapTexture);
        _waterShader.SetShaderParameter("HeightMapFactor", HeightMapFactor);
        _waterShader.SetShaderParameter("WaterInnerOffset", WaterInnerOffset);
        _waterShader.SetShaderParameter("WaterTexture", WaterTexture);
        _waterShader.SetShaderParameter("WaterFactor", WaterFactor);
        _waterShader.SetShaderParameter("WaterColor", WaterColor);
        _waterShader.SetShaderParameter("FresnelColor", FresnelColor);
        _waterShader.SetShaderParameter("Metallic", Metallic);
        _waterShader.SetShaderParameter("Roughness", Roughness);
        _waterShader.SetShaderParameter("NormalMap", NormalMap);
        _waterShader.SetShaderParameter("NormalMap2", NormalMap2);
        _waterShader.SetShaderParameter("TimeScale", TimeScale);
        _waterShader.SetShaderParameter("Strength", Strength);
        _waterShader.SetShaderParameter("Wave", Wave);
        _waterShader.SetShaderParameter("NoiseScale", NoiseScale);
        _waterShader.SetShaderParameter("HeightScale", HeightScale);
        _waterShader.SetShaderParameter("ColorDeep", ColorDeep);
        _waterShader.SetShaderParameter("ColorShallow", ColorShallow);
        _waterShader.SetShaderParameter("BeersLaw", BeersLaw);
        _waterShader.SetShaderParameter("DepthOffset", DepthOffset);
        _waterShader.SetShaderParameter("EdgeScale", EdgeScale);
        _waterShader.SetShaderParameter("Near", Near);
        _waterShader.SetShaderParameter("Far", Far);
        _waterShader.SetShaderParameter("EdgeColor", EdgeColor);
        _waterShader.SetShaderParameter("WaterRippleTextureSize", rippleTextureSize);

        var newRippleImage = Image.Create(TerrainSize, TerrainSize, false, Image.Format.Rgba8);

        newRippleImage.Fill(Colors.Black);
        _rippleImage = ImageTexture.CreateFromImage(newRippleImage);

        _rippleShader.SetShaderParameter("CollisionTexture", _rippleImage);
        _rippleShader.SetShaderParameter("WaterRippleTextureSize", rippleTextureSize);

        _rippleViewport.Size = new Vector2I(rippleTextureSize, rippleTextureSize);
        _rippleBufferViewport.Size = new Vector2I(rippleTextureSize, rippleTextureSize);
    }

    public void AddRippleEffect(float x, float y) {
        var xPosition = (int) Math.Round(x);
        var yPosition = (int) Math.Round(y);

        var pixelPosition = new Vector2I(xPosition + (TerrainSize / 2), yPosition + (TerrainSize / 2));

        var image = _rippleImage.GetImage();
        image.SetPixel(pixelPosition.X, pixelPosition.Y, Colors.Red);

        _rippleImage.Update(image);
        if (!_ripplePositions.ContainsKey(pixelPosition)) {
            _ripplePositions[pixelPosition] = 1;
        }
    }
}

