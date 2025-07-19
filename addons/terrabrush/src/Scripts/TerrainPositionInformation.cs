using Godot;

namespace TerraBrush;

[GodotClass]
public partial class TerrainPositionInformation : GodotObject {
    [BindProperty] public TerrainPositionTextureInformation[] Textures { get; set; }
    [BindProperty] public float WaterFactor { get; set; }
    [BindProperty] public float WaterDeepness { get; set; }
    [BindProperty] public float SnowFactor { get; set; }
    [BindProperty] public float SnowHeight { get; set; }
    [BindProperty] public int MetaInfoIndex { get; set; }
    [BindProperty] public string MetaInfoName { get; set; }
}

[GodotClass]
public partial class TerrainPositionTextureInformation : GodotObject {
    [BindProperty] public int Index { get; set; }
    [BindProperty] public string Name { get; set; }
    [BindProperty] public float Factor { get; set; }
}
