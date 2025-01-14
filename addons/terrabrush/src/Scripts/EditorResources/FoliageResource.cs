using System;
using Godot;

namespace TerraBrush;

public partial class FoliageResource : Resource {
    [Export] public FoliageDefinitionResource Definition { get;set; }
}
