using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class ZoneResource : Resource {
    [Export] public Vector2I ZonePosition { get;set; }
    [Export] public ImageTexture HeightMapTexture { get;set; }
    [Export] public ImageTexture[] SplatmapsTexture { get;set; }
    [Export] public ImageTexture[] FoliagesTexture { get;set; }
    [Export] public ImageTexture[] ObjectsTexture { get;set; }
    [Export] public ImageTexture WaterTexture { get;set; }
    [Export] public ImageTexture SnowTexture { get;set; }
}
