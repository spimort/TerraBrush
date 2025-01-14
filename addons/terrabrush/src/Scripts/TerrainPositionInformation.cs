using Godot;

namespace TerraBrush;

public partial class TerrainPositionInformation : GodotObject {
    public TerrainPositionTextureInformation[] Textures { get;set; }
    public float WaterFactor { get;set; }
    public float WaterDeepness { get;set; }
    public float SnowFactor { get;set; }
    public float SnowHeight { get;set; }
}

public partial class TerrainPositionTextureInformation : GodotObject {
    public int Index { get;set; }
    public string Name { get;set; }
    public float Factor { get;set; }
}
