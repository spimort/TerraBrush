using System;
using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class FoliageResource : Resource {
    [BindProperty] public FoliageDefinitionResource Definition { get;set; }
}
