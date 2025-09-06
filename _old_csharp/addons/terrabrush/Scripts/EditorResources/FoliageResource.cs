using System;
using Godot;

namespace TerraBrush;

[Tool]
[GlobalClass]
public partial class FoliageResource : Resource {
    [Export] public FoliageDefinitionResource Definition { get;set; }
}
