using System;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageResource : Resource {
    [Export] public FoliageDefinitionResource Definition { get;set; }

    #region " Deprecated exports, will be removed in the future "
    [ExportGroup("Deprecated (DO NOT USE)")]
    [Export]
    [Obsolete("This property was there before the zones were a thing, will be removed in the future")]
    public ImageTexture Texture { get;set; }
    #endregion
}
