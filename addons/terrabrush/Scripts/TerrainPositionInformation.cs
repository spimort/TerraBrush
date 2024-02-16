using Godot;

namespace TerraBrush;

public partial class TerrainPositionInformation : GodotObject {
    [Export] public TerrainPositionTextureInformation[] Textures { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export] public float WaterDeepness { get;set; }
    [Export] public float SnowFactor { get;set; }
    [Export] public float SnowHeight { get;set; }
}

public partial class TerrainPositionTextureInformation : GodotObject {
    [Export] public int Index { get;set; }
    [Export] public string Name { get;set; }
    [Export] public float Factor { get;set; }
}
