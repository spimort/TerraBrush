using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Snow : Node3D {
    private const float DeCompressSpeed = 0.5f;

    private Dictionary<ZoneResource, Dictionary<Vector2I, float>> _compressedPositions = new();
    private Dictionary<ZoneResource, Image> _imagesCache = new();

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

        UpdateSnow();
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

        if (SnowDefinition.CustomShader == null) {
            _clipmap.Shader = new ShaderMaterial() {
                Shader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader")
            };
        } else {
            _clipmap.Shader = SnowDefinition.CustomShader;
        }

        _clipmap.CreateMesh();

        _clipmap.Shader.SetShaderParameter(StringNames.SnowTextures, TerrainZones.SnowTextures);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowFactor, SnowDefinition.SnowFactor);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowInnerOffset, SnowDefinition.SnowInnerOffset);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowColorTexture, SnowDefinition.SnowColorTexture);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowColorNormal, SnowDefinition.SnowColorNormal);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowColorRoughness, SnowDefinition.SnowColorRoughness);
        _clipmap.Shader.SetShaderParameter(StringNames.SnowColorDetail, SnowDefinition.SnowColorDetail);
        _clipmap.Shader.SetShaderParameter(StringNames.Noise, SnowDefinition.Noise);
        _clipmap.Shader.SetShaderParameter(StringNames.NoiseFactor, SnowDefinition.NoiseFactor);
        _clipmap.Shader.SetShaderParameter(StringNames.Metallic, SnowDefinition.Metallic);
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        if (_compressedPositions.Count > 0) {
            for (var imageIndex = _compressedPositions.Count - 1; imageIndex >= 0; imageIndex--) {
                var zone = _compressedPositions.Keys.ElementAt(imageIndex);
                var points = _compressedPositions[zone];

                var compressedSnowImage = GetImageForZone(zone);

                for (var i = points.Count - 1; i >= 0; i--) {
                    var position = points.Keys.ElementAt(i);
                    var pixel = compressedSnowImage.GetPixel(position.X, position.Y);
                    var compressionValue = points[position];
                    var offsetX = pixel.G;
                    var offsetY = pixel.B;

                    if (compressionValue < 1) {
                        compressionValue += (float) delta * DeCompressSpeed;

                        if (compressionValue >= 1.0) {
                            compressionValue = 1.0f;
                            points.Remove(position);

                            offsetX = 0f;
                            offsetY = 0f;
                        } else {
                            points[position] = compressionValue;
                        }

                        compressedSnowImage.SetPixel(position.X, position.Y, new Color(pixel.R, offsetX, offsetY, compressionValue));
                    }
                }

                zone.SnowTexture.Update(compressedSnowImage);
                TerrainZones.UpdateZoneSnowTexture(zone);

                if (points.Count == 0) {
                    _compressedPositions.Remove(zone);
                }
            }
        }
    }

    public void AddCompressedSnow(float x, float y) {
        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(x, y, ZonesSize);
        var zone = TerrainZones.GetZoneForZoneInfo(zoneInfo);

        if (zone != null) {
            var image = GetImageForZone(zone);
            var pixelPosition = new Vector2I(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
            var currentPixel = image.GetPixel(pixelPosition.X, pixelPosition.Y);

            if (currentPixel.R > 0) {
                var offsetX = x - pixelPosition.X;
                var offsetY = y - pixelPosition.Y;

                image.SetPixel(pixelPosition.X, pixelPosition.Y, new Color(currentPixel.R, offsetX, offsetY, 0));

                zone.SnowTexture.Update(image);
                TerrainZones.UpdateZoneSnowTexture(zone);

                _compressedPositions.TryGetValue(zone, out var listOfPoints);
                if (listOfPoints == null) {
                    listOfPoints = new Dictionary<Vector2I, float>();
                    _compressedPositions.Add(zone, listOfPoints);
                }

                if (!listOfPoints.ContainsKey(pixelPosition)) {
                    listOfPoints.Add(pixelPosition, 0);
                }
            }
        }
    }

    private Image GetImageForZone(ZoneResource zone) {
        _imagesCache.TryGetValue(zone, out var image);
        if (image == null) {
            image = zone.SnowTexture.GetImage();
            _imagesCache.Add(zone, image);
        }

        return image;
    }
}

