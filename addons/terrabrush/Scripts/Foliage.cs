using Godot;

namespace TerraBrush;

[Tool]
public partial class Foliage : Node3D {
    private ShaderMaterial _foliageShader;

    [NodePath] private GpuParticles3D _particles;

    [Export] public int TerrainSize { get;set; }
    [Export] public ImageTexture HeightMapTexture { get;set; }
    [Export] public float HeightMapFactor { get;set; }
    [Export] public ImageTexture FoliageTexture { get;set; }
    [Export] public Mesh Mesh { get;set; }
    [Export] public Vector3 MeshScale { get;set; }
    [Export] public int MaximumRenderDistance { get;set; }
    [Export] public int EditorMaximumRenderDistance { get;set; }
    [Export] public float WindStrength { get;set; }
    [Export] public BaseMaterial3D MeshMaterial { get;set; }
    [Export] public Texture2D NoiseTexture { get;set; }
    [Export] public ImageTexture WaterTexture { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export(PropertyHint.Layers3DRender)] public int VisualInstanceLayers { get;set; }

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        this._foliageShader = (ShaderMaterial) this._particles.ProcessMaterial;
    }

    public override void _Process(double delta) {
        if (!Engine.IsEditorHint()) {
            this._foliageShader.SetShaderParameter("PlayerPosition", this.GetViewport()?.GetCamera3D()?.GlobalPosition ?? Vector3.Zero);
        }
    }

    public void UpdateFoliage() {
        if (_particles == null) {
            return;
        }

        this._particles.Layers = (uint) VisualInstanceLayers;
        this._particles.DrawPass1 = this.Mesh;
        this._particles.MaterialOverride = this.MeshMaterial;

        this._foliageShader.SetShaderParameter("HeightMapSize", this.TerrainSize);
        this._foliageShader.SetShaderParameter("HeightMapTexture", HeightMapTexture);
        this._foliageShader.SetShaderParameter("HeightMapFactor", this.HeightMapFactor);
        this._foliageShader.SetShaderParameter("FoliageTexture", FoliageTexture);
        this._foliageShader.SetShaderParameter("MeshScale", this.MeshScale);
        this._foliageShader.SetShaderParameter("WindStrength", this.WindStrength);
        this._foliageShader.SetShaderParameter("WaterTexture", this.WaterTexture);
        this._foliageShader.SetShaderParameter("WaterFactor", this.WaterFactor);

        if (NoiseTexture != null) {
            var noiseImage = new Image();
            noiseImage.CopyFrom(NoiseTexture.GetImage());
            noiseImage.Resize(this.TerrainSize, this.TerrainSize);

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
        this._foliageShader?.SetShaderParameter("PlayerPosition", viewportCamera.GlobalPosition);
    }

    public void UpdateGroudTexture(Texture2D texture) {
        this._foliageShader.SetShaderParameter("GroudTexture", texture);
    }
}
