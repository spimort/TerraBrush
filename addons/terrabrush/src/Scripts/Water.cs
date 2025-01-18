using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[GodotClass]
public partial class Water : Node3D {
    private const float RippleResetSpeed = 0.9f;

    private Dictionary<ZoneResource, Dictionary<Vector2I, float>> _ripplePositions = new();
    private Dictionary<ZoneResource, Image> _imagesCache = new();

    private Clipmap _clipmap;

    public int ZonesSize { get;set; }
    public int Resolution { get;set; }
    public ZonesResource TerrainZones { get;set; }
    public float WaterFactor { get;set; }
    public float HeightMapFactor { get;set; }
    public float WaterInnerOffset { get;set; }
    public Color WaterColor { get;set; }
    public Color FresnelColor { get;set; }
    public float Metallic { get;set; }
    public float Roughness { get;set; }
    public Texture2D NormalMap { get;set; }
    public Texture2D NormalMap2 { get;set; }
    public float TimeScale { get;set; } = 0.1f;
    public float Strength { get;set; } = 0.4f;
    public Texture2D Wave { get;set; }
    public float NoiseScale { get;set; }
    public float HeightScale { get;set; }
    public Color ColorDeep { get;set; }
    public Color ColorShallow { get;set; }
    public float BeersLaw { get;set; }
    public float DepthOffset { get;set; }
    public float EdgeScale { get;set; }
    public float Near { get;set; }
    public float Far { get;set; }
    public Color EdgeColor { get;set; }
    public int VisualInstanceLayers { get;set; } = 1;
    public int LODLevels { get;set; } = 8;
    public int LODRowsPerLevel { get;set; } = 21;
    public float LODInitialCellWidth { get;set; } = 1;
    public ShaderMaterial CustomShader { get;set; }

    public Clipmap Clipmap => _clipmap;

    protected override void _Ready() {
        base._Ready();

        _clipmap = new Clipmap();
        AddChild(_clipmap);

        UpdateWater();
    }

    protected override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);
        if (_ripplePositions.Count > 0) {
            for (var imageIndex = _ripplePositions.Count - 1; imageIndex >= 0; imageIndex--) {
                var zone = _ripplePositions.Keys.ElementAt(imageIndex);
                var points = _ripplePositions[zone];

                var rippleWaterImage = GetImageForZone(zone);

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

                zone.WaterTexture.Update(rippleWaterImage);
                TerrainZones.UpdateZoneWaterTexture(zone);

                if (points.Count == 0) {
                    _ripplePositions.Remove(zone);
                }
            }
        }
    }

    public void UpdateWater() {
        if (_clipmap == null) {
            return;
        }

        _clipmap.ClipmapMesh.Layers = (uint) VisualInstanceLayers;
        _clipmap.ZonesSize = ZonesSize;
        _clipmap.Resolution = Resolution;
        _clipmap.TerrainZones = TerrainZones;
        _clipmap.Levels = LODLevels;
        _clipmap.RowsPerLevel = LODRowsPerLevel;
        _clipmap.InitialCellWidth = LODInitialCellWidth;

        if (CustomShader == null) {
            _clipmap.Shader = new ShaderMaterial() {
                Shader = (Shader) ResourceLoader.Singleton.Load("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader")
            };
        } else {
            _clipmap.Shader = Utils.CreateCustomShaderCopy(CustomShader);
        }

        _clipmap.CreateMesh();

        _clipmap.Shader.SetShaderParameter(StringNames.WaterInnerOffset, WaterInnerOffset);
        _clipmap.Shader.SetShaderParameter(StringNames.WaterTextures, TerrainZones.WaterTextures);
        _clipmap.Shader.SetShaderParameter(StringNames.WaterFactor, WaterFactor);
        _clipmap.Shader.SetShaderParameter(StringNames.WaterColor, WaterColor);
        _clipmap.Shader.SetShaderParameter(StringNames.FresnelColor, FresnelColor);
        _clipmap.Shader.SetShaderParameter(StringNames.Metallic, Metallic);
        _clipmap.Shader.SetShaderParameter(StringNames.Roughness, Roughness);
        _clipmap.Shader.SetShaderParameter(StringNames.NormalMap, NormalMap);
        _clipmap.Shader.SetShaderParameter(StringNames.NormalMap2, NormalMap2);
        _clipmap.Shader.SetShaderParameter(StringNames.TimeScale, TimeScale);
        _clipmap.Shader.SetShaderParameter(StringNames.Strength, Strength);
        _clipmap.Shader.SetShaderParameter(StringNames.Wave, Wave);
        _clipmap.Shader.SetShaderParameter(StringNames.NoiseScale, NoiseScale);
        _clipmap.Shader.SetShaderParameter(StringNames.HeightScale, HeightScale);
        _clipmap.Shader.SetShaderParameter(StringNames.ColorDeep, ColorDeep);
        _clipmap.Shader.SetShaderParameter(StringNames.ColorShallow, ColorShallow);
        _clipmap.Shader.SetShaderParameter(StringNames.BeersLaw, BeersLaw);
        _clipmap.Shader.SetShaderParameter(StringNames.DepthOffset, DepthOffset);
        _clipmap.Shader.SetShaderParameter(StringNames.EdgeScale, EdgeScale);
        _clipmap.Shader.SetShaderParameter(StringNames.Near, Near);
        _clipmap.Shader.SetShaderParameter(StringNames.Far, Far);
        _clipmap.Shader.SetShaderParameter(StringNames.EdgeColor, EdgeColor);

        // This is for compatibility with Godot 4.2
        var engineVersion = Engine.Singleton.GetVersionInfo();
        var major = (int) engineVersion["major"];
        var minor = (int) engineVersion["minor"];
        if (major == 4 && minor < 3) {
            _clipmap.Shader.SetShaderParameter(StringNames.InvertedZ, false);
        }
    }

    public void AddRippleEffect(float x, float y) {
        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(x, y, ZonesSize, Resolution);
        var zone = TerrainZones.GetZoneForZoneInfo(zoneInfo);

        if (zone != null) {
            var image = GetImageForZone(zone);
            var pixelPosition = new Vector2I(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
            var currentPixel = image.GetPixel(pixelPosition.X, pixelPosition.Y);

            if (currentPixel.R > 0) {
                image.SetPixel(pixelPosition.X, pixelPosition.Y, new Color(currentPixel.R, currentPixel.G, currentPixel.B, 0));

                zone.WaterTexture.Update(image);
                TerrainZones.UpdateZoneWaterTexture(zone);

                _ripplePositions.TryGetValue(zone, out var listOfPoints);
                if (listOfPoints == null) {
                    listOfPoints = new Dictionary<Vector2I, float>();
                    _ripplePositions.Add(zone, listOfPoints);
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
            image = zone.WaterTexture.GetImage();
            _imagesCache.Add(zone, image);
        }

        return image;
    }
}

