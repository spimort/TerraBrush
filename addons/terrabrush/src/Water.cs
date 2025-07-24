using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class Water : Node3D {
    private const float RippleResetSpeed = 0.9f;

    private Dictionary<ZoneResource, Dictionary<Vector2I, float>> _ripplePositions = new();
    private Dictionary<ZoneResource, Image> _imagesCache = new();

    [NodePath] private Clipmap _clipmap;

    [BindProperty] public int ZonesSize { get;set; }
    [BindProperty] public int Resolution { get;set; }
    [BindProperty] public ZonesResource TerrainZones { get;set; }
    [BindProperty] public float WaterFactor { get;set; }
    [BindProperty] public float HeightMapFactor { get;set; }
    [BindProperty] public float WaterInnerOffset { get;set; }
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color WaterColor { get;set; }
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color FresnelColor { get;set; }
    [BindProperty(Hint = PropertyHint.Range, HintString = "0,1,0.001")] public float Metallic { get;set; }
    [BindProperty(Hint = PropertyHint.Range, HintString = "0,1,0.001")] public float Roughness { get;set; }
    [BindProperty] public Texture2D NormalMap { get;set; }
    [BindProperty] public Texture2D NormalMap2 { get;set; }
    [BindProperty] public float TimeScale { get;set; } = 0.1f;
    [BindProperty] public float Strength { get;set; } = 0.4f;
    [BindProperty] public Texture2D Wave { get;set; }
    [BindProperty] public float NoiseScale { get;set; }
    [BindProperty] public float HeightScale { get;set; }
    [BindProperty] public Color ColorDeep { get;set; }
    [BindProperty] public Color ColorShallow { get;set; }
    [BindProperty] public float BeersLaw { get;set; }
    [BindProperty] public float DepthOffset { get;set; }
    [BindProperty] public float EdgeScale { get;set; }
    [BindProperty] public float Near { get;set; }
    [BindProperty] public float Far { get;set; }
    [BindProperty(Hint = PropertyHint.ColorNoAlpha)] public Color EdgeColor { get;set; }
    [BindProperty(Hint = PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; } = 1;
    [BindProperty] public int LODLevels { get;set; } = 8;
    [BindProperty] public int LODRowsPerLevel { get;set; } = 21;
    [BindProperty] public float LODInitialCellWidth { get;set; } = 1;
    [BindProperty] public ShaderMaterial CustomShader { get;set; }

    public Clipmap Clipmap => _clipmap;

    protected override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

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
                Shader = ResourceLoaderHelper.Load<Shader>("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader")
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

