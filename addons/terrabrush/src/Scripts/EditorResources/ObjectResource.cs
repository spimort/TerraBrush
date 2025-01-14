using System;
using Godot;

namespace TerraBrush;

public partial class ObjectResource : Resource {
    public ObjectDefinitionResource Definition { get;set; }
    public bool Hide { get;set; }
}
