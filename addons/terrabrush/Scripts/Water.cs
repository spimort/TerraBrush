using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Water : Node3D {
    private const float RippleResetSpeed = 0.9f;

    private Dictionary<ImageTexture, Dictionary<Vector2I, float>> _ripplePositions = new();
    private Dictionary<ImageTexture, Image> _imagesCache = new();

    [NodePath] private Clipmap _clipmap;

    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
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
    [Export] public int LODLevels { get;set; } = 8;
    [Export] public int LODRowsPerLevel { get;set; } = 21;
    [Export] public float LODInitialCellWidth { get;set; } = 1;

    public Clipmap Clipmap => _clipmap;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);
        if (_ripplePositions.Count > 0) {
            for (var imageIndex = _ripplePositions.Count - 1; imageIndex >= 0; imageIndex--) {
                var imageTexture = _ripplePositions.Keys.ElementAt(imageIndex);
                var points = _ripplePositions[imageTexture];

                var rippleWaterImage = GetImageForTexture(imageTexture);

                for (var i = points.Count - 1; i >= 0; i--) {
                    var position = points.Keys.ElementAt(i);
                    var pixel = rippleWaterImage.GetPixel(position.X, position.Y);
                    var rippleValue = points[position];

                    if (rippleValue < 1) {
                        rippleValue += (float) delta * RippleResetSpeed;

                        if (rippleValue >= 1.0) {
                            rippleValue = 1.0f;
                            points.Remove(position);
                        } else {
                            points[position] = rippleValue;
                        }

                        rippleWaterImage.SetPixel(position.X, position.Y, new Color(pixel.R, pixel.G, pixel.B, rippleValue));
                    }
                }

                imageTexture.Update(rippleWaterImage);

                if (points.Count == 0) {
                    _ripplePositions.Remove(imageTexture);
                }
            }

            TerrainZones.UpdateWaterTextures();
        }
    }

    public void UpdateWater() {
        if (_clipmap == null) {
            return;
        }

        _clipmap.ClipmapMesh.Layers = (uint) VisualInstanceLayers;
        _clipmap.ZonesSize = ZonesSize;
        _clipmap.TerrainZones = TerrainZones;
        _clipmap.Levels = LODLevels;
        _clipmap.RowsPerLevel = LODRowsPerLevel;
        _clipmap.InitialCellWidth = LODInitialCellWidth;

        _clipmap.CreateMesh();

        _clipmap.Shader.SetShaderParameter("WaterInnerOffset", WaterInnerOffset);
        _clipmap.Shader.SetShaderParameter("WaterTextures", TerrainZones.WaterTextures);
        _clipmap.Shader.SetShaderParameter("WaterFactor", WaterFactor);
        _clipmap.Shader.SetShaderParameter("WaterColor", WaterColor);
        _clipmap.Shader.SetShaderParameter("FresnelColor", FresnelColor);
        _clipmap.Shader.SetShaderParameter("Metallic", Metallic);
        _clipmap.Shader.SetShaderParameter("Roughness", Roughness);
        _clipmap.Shader.SetShaderParameter("NormalMap", NormalMap);
        _clipmap.Shader.SetShaderParameter("NormalMap2", NormalMap2);
        _clipmap.Shader.SetShaderParameter("TimeScale", TimeScale);
        _clipmap.Shader.SetShaderParameter("Strength", Strength);
        _clipmap.Shader.SetShaderParameter("Wave", Wave);
        _clipmap.Shader.SetShaderParameter("NoiseScale", NoiseScale);
        _clipmap.Shader.SetShaderParameter("HeightScale", HeightScale);
        _clipmap.Shader.SetShaderParameter("ColorDeep", ColorDeep);
        _clipmap.Shader.SetShaderParameter("ColorShallow", ColorShallow);
        _clipmap.Shader.SetShaderParameter("BeersLaw", BeersLaw);
        _clipmap.Shader.SetShaderParameter("DepthOffset", DepthOffset);
        _clipmap.Shader.SetShaderParameter("EdgeScale", EdgeScale);
        _clipmap.Shader.SetShaderParameter("Near", Near);
        _clipmap.Shader.SetShaderParameter("Far", Far);
        _clipmap.Shader.SetShaderParameter("EdgeColor", EdgeColor);
    }

    public void AddRippleEffect(float x, float y) {
        var xPosition = (int) Math.Round(x);
        var yPosition = (int) Math.Round(y);

        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(xPosition, yPosition, ZonesSize);
        var zone = TerrainZones.GetZoneForZoneInfo(zoneInfo);

        if (zone != null) {
            var image = GetImageForTexture(zone.WaterTexture);
            var pixelPosition = new Vector2I(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
            var currentPixel = image.GetPixel(pixelPosition.X, pixelPosition.Y);

            if (currentPixel.R > 0) {
                image.SetPixel(pixelPosition.X, pixelPosition.Y, new Color(currentPixel.R, currentPixel.G, currentPixel.B, 0));

                zone.WaterTexture.Update(image);
                TerrainZones.UpdateWaterTextures();

                _ripplePositions.TryGetValue(zone.WaterTexture, out var listOfPoints);
                if (listOfPoints == null) {
                    listOfPoints = new Dictionary<Vector2I, float>();
                    _ripplePositions.Add(zone.WaterTexture, listOfPoints);
                }

                if (!listOfPoints.ContainsKey(pixelPosition)) {
                    listOfPoints.Add(pixelPosition, 0);
                }
            }
        }
    }

    private Image GetImageForTexture(ImageTexture imageTexture) {
        _imagesCache.TryGetValue(imageTexture, out var image);
        if (image == null) {
            image = imageTexture.GetImage();
            _imagesCache.Add(imageTexture, image);
        }

        return image;
    }
}

