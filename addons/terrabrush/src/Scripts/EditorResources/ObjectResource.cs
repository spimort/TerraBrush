using System;
using Godot;

namespace TerraBrush;

public partial class ObjectResource : Resource {
    [Export] public ObjectDefinitionResource Definition { get;set; }
    [Export] public bool Hide { get;set; }
}
