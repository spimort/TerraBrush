using System;
using System.Linq;
using Godot;

namespace TerraBrush;

[GodotClass(Tool = true)]
public partial class ToolsPieMenu : Control {
    private PieMenu _pieMenu;

    public Action<TerrainToolType> OnToolSelected { get;set; }

    protected override void _Ready() {
        base._Ready();

        _pieMenu = new PieMenu();
        _pieMenu.Label = "Tools";
        AddChild(_pieMenu);

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Raise terrain"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Lower terrain"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainSmooth,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_smooth.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Smooth terrain"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainFlatten,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_flatten.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Flatten terrain"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainSetHeight,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_set_height.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Set terrain height"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.TerrainSetAngle,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/map_set_angle.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Set terrain angle"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.Paint,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/paint.png"),
            IconType = IconType.Circle,
            TooltipText = "Paint texture"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.FoliageAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/foliage_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add foliage"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.FoliageRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/foliage_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove foliage"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.ObjectAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/object_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add objects"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.ObjectRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/object_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove objects"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.WaterAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/water_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add water"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.WaterRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/water_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove water"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.WaterFlowAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/flow_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add water flow"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.WaterFlowRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/flow_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove water flow"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.SnowAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/snow_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add snow"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.SnowRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/snow_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove snow"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.HoleAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/holes_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Add hole"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.HoleRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/holes_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Remove hole"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.LockAdd,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/lock_add.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Lock"
        });
        _pieMenu.AddChild(new ToolPreview() {
            ToolType = TerrainToolType.LockRemove,
            ButtonImage = ResourceLoaderHelper.Load<Texture2D>("res://addons/terrabrush/Assets/Icons/lock_remove.png"),
            IconType = IconType.CircleWithSign,
            TooltipText = "Unlock"
        });

        _pieMenu.AddChild(new Control()); // Splitter

        foreach (var toolPreview in _pieMenu.GetChildren().Where(control => control is ToolPreview).Cast<ToolPreview>()) {
            toolPreview.OnSelect = () => {
                OnToolSelected.Invoke(toolPreview.ToolType);
            };
        }
    }
}
