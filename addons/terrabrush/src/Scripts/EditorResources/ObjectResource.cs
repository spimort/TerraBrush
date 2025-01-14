using System;
using Godot;

namespace TerraBrush;

[GlobalClass]
public partial class ObjectResource : Resource {
    [Export] public ObjectDefinitionResource Definition { get;set; }
    [Export] public bool Hide { get;set; }
}
