using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Foliage : Node3D {
    private ShaderMaterial _foliageShader;

    [NodePath] private GpuParticles3D _particles;

    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public TextureSetsResource TextureSets { get;set;}
	[Export] public int TextureDetail { get;set; } = 1;
    [Export] public Mesh Mesh { get;set; }
    [Export] public Vector3 MeshScale { get;set; }
    [Export] public int MaximumRenderDistance { get;set; }
    [Export] public int EditorMaximumRenderDistance { get;set; }
    [Export] public float WindStrength { get;set; }
    [Export] public Material MeshMaterial { get;set; }
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        this._foliageShader = (ShaderMaterial) this._particles.ProcessMaterial;
    }

    public override void _Process(double delta) {
        if (!Engine.IsEditorHint()) {
            this._foliageShader.SetShaderParameter("GlobalPosition", this.GetViewport()?.GetCamera3D()?.GlobalPosition ?? Vector3.Zero);
        }
    }

    public void UpdateFoliage(int foliageIndex) {
        if (_particles == null) {
            return;
        }

        this._particles.Layers = (uint) VisualInstanceLayers;
        this._particles.DrawPass1 = this.Mesh;
        this._particles.MaterialOverride = this.MeshMaterial;

        _foliageShader.SetShaderParameter("HeightmapTextures", TerrainZones.HeightmapTextures);
        _foliageShader.SetShaderParameter("ZonesSize", (float) ZonesSize);
        _foliageShader.SetShaderParameter("NumberOfZones", (float) TerrainZones.Zones.Count());
		_foliageShader.SetShaderParameter("ZonesMap", TerrainZones.ZonesMap);

        if (TextureSets?.TextureSets != null) {
            _foliageShader.SetShaderParameter("Splatmaps", TerrainZones.SplatmapsTextures);
            _foliageShader.SetShaderParameter("Textures", Utils.TexturesToTextureArray(TextureSets.TextureSets.Select(x => x.AlbedoTexture)));
            _foliageShader.SetShaderParameter("NumberOfTextures", TextureSets?.TextureSets?.Count() ?? 0);
    		_foliageShader.SetShaderParameter("TextureDetail", this.TextureDetail);
        }

        _foliageShader.SetShaderParameter("FoliageTextures", TerrainZones.FoliagesTextures[foliageIndex]);
        _foliageShader.SetShaderParameter("MeshScale", this.MeshScale);
        _foliageShader.SetShaderParameter("WindStrength", this.WindStrength);

        _foliageShader.SetShaderParameter("WaterTextures", TerrainZones.WaterTextures);
        _foliageShader.SetShaderParameter("WaterFactor", this.WaterFactor);

        if (NoiseTexture != null) {
            var noiseImage = new Image();
            noiseImage.CopyFrom(NoiseTexture.GetImage());
            noiseImage.Resize(this.ZonesSize, this.ZonesSize);

            this._foliageShader.SetShaderParameter("NoiseTexture", ImageTexture.CreateFromImage(noiseImage));
        }

        if (Engine.IsEditorHint()) {
            this._particles.Amount = this.EditorMaximumRenderDistance * this.EditorMaximumRenderDistance;

            this._foliageShader.SetShaderParameter("MaximumDistance", this.EditorMaximumRenderDistance);
        } else {
            this._particles.Amount = this.MaximumRenderDistance * this.MaximumRenderDistance;

            this._foliageShader.SetShaderParameter("MaximumDistance", this.MaximumRenderDistance);
        }
    }

    public void UpdateEditorCameraPosition(Camera3D viewportCamera) {
        this._foliageShader?.SetShaderParameter("GlobalPosition", viewportCamera.GlobalPosition);
    }
}
