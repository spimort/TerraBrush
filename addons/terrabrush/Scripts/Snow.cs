using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Snow : Node3D {
    private const float DeCompressSpeed = 0.5f;

    private Dictionary<ImageTexture, Dictionary<Vector2I, float>> _compressedPositions = new();
    private Dictionary<ImageTexture, Image> _imagesCache = new();

    [NodePath] private Clipmap _clipmap;

    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
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
        _clipmap.ZonesSize = ZonesSize;
        _clipmap.TerrainZones = TerrainZones;
        _clipmap.Levels = LODLevels;
        _clipmap.RowsPerLevel = LODRowsPerLevel;
        _clipmap.InitialCellWidth = LODInitialCellWidth;

        _clipmap.CreateMesh();

        _clipmap.Shader.SetShaderParameter("SnowTextures", TerrainZones.SnowTextures);
        _clipmap.Shader.SetShaderParameter("SnowFactor", SnowDefinition.SnowFactor);
        _clipmap.Shader.SetShaderParameter("SnowInnerOffset", SnowDefinition.SnowInnerOffset);
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
            for (var imageIndex = _compressedPositions.Count - 1; imageIndex >= 0; imageIndex--) {
                var imageTexture = _compressedPositions.Keys.ElementAt(imageIndex);
                var points = _compressedPositions[imageTexture];

                var compressedSnowImage = GetImageForTexture(imageTexture);

                for (var i = points.Count - 1; i >= 0; i--) {
                    var position = points.Keys.ElementAt(i);
                    var pixel = compressedSnowImage.GetPixel(position.X, position.Y);
                    var compressionValue = points[position];

                    if (compressionValue < 1) {
                        compressionValue += (float) delta * DeCompressSpeed;

                        if (compressionValue >= 1.0) {
                            compressionValue = 1.0f;
                            points.Remove(position);
                        } else {
                            points[position] = compressionValue;
                        }

                        compressedSnowImage.SetPixel(position.X, position.Y, new Color(pixel.R, pixel.G, pixel.B, compressionValue));
                    }
                }

                imageTexture.Update(compressedSnowImage);

                if (points.Count == 0) {
                    _compressedPositions.Remove(imageTexture);
                }
            }

            TerrainZones.UpdateSnowTextures();
        }
    }

    public void AddCompressedSnow(float x, float y) {
        x = x + (ZonesSize / 2);
        y = y + (ZonesSize / 2);

        var xPosition = (int) Math.Round(x);
        var yPosition = (int) Math.Round(y);

        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(xPosition, yPosition, ZonesSize);
        var zone = TerrainZones.GetZoneForZoneInfo(zoneInfo);

        if (zone != null) {
            var image = GetImageForTexture(zone.SnowTexture);
            var pixelPosition = new Vector2I(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
            var currentPixel = image.GetPixel(pixelPosition.X, pixelPosition.Y);

            if (currentPixel.R > 0) {
                image.SetPixel(pixelPosition.X, pixelPosition.Y, new Color(currentPixel.R, currentPixel.G, currentPixel.B, 0));

                zone.SnowTexture.Update(image);
                TerrainZones.UpdateSnowTextures();

                _compressedPositions.TryGetValue(zone.SnowTexture, out var listOfPoints);
                if (listOfPoints == null) {
                    listOfPoints = new Dictionary<Vector2I, float>();
                    _compressedPositions.Add(zone.SnowTexture, listOfPoints);
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

