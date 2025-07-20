using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class MetaInfoLayer : Resource {
    [BindProperty] public string Name { get;set; }
    [BindProperty] public Color Color { get;set; } = NamedColors.Black;
}
