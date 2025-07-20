using System;
using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ObjectResource : Resource {
    [BindProperty] public ObjectDefinitionResource Definition { get;set; }
    [BindProperty] public bool Hide { get;set; }
}
