#nullable enable

namespace TerraBrush.win.x64;

internal static class ClassDBExtensions
{
    internal static void InitializeUserTypes(global::Godot.Bridge.InitializationLevel level)
    {
        if (level != global::Godot.Bridge.InitializationLevel.Scene)
        {
            return;
        }
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.DockPreviewButton>(global::TerraBrush.DockPreviewButton.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.TerraBrushTool>(global::TerraBrush.TerraBrushTool.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Plugin>(global::TerraBrush.Plugin.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.BrushDecal>(global::TerraBrush.BrushDecal.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.BrushNumericSelector>(global::TerraBrush.BrushNumericSelector.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Clipmap>(global::TerraBrush.Clipmap.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.CustomContentPieMenu>(global::TerraBrush.CustomContentPieMenu.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ToolPreview>(global::TerraBrush.ToolPreview.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.FoliageDefinitionResource>(global::TerraBrush.FoliageDefinitionResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.FoliageResource>(global::TerraBrush.FoliageResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.MetaInfoLayer>(global::TerraBrush.MetaInfoLayer.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectDefinitionResource>(global::TerraBrush.ObjectDefinitionResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectOctreeLODDefinitionResource>(global::TerraBrush.ObjectOctreeLODDefinitionResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectOctreeLODMeshDefinitionResource>(global::TerraBrush.ObjectOctreeLODMeshDefinitionResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectOctreeLODMeshesDefinitionResource>(global::TerraBrush.ObjectOctreeLODMeshesDefinitionResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectResource>(global::TerraBrush.ObjectResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.SnowResource>(global::TerraBrush.SnowResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.TextureSetResource>(global::TerraBrush.TextureSetResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.TextureSetsResource>(global::TerraBrush.TextureSetsResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.WaterResource>(global::TerraBrush.WaterResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ZoneResource>(global::TerraBrush.ZoneResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ZonesResource>(global::TerraBrush.ZonesResource.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Foliage>(global::TerraBrush.Foliage.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ImportDialog>(global::TerraBrush.ImportDialog.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ImportImageRow>(global::TerraBrush.ImportImageRow.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.KeybindSettings>(global::TerraBrush.KeybindSettings.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.KeyListenDialog>(global::TerraBrush.KeyListenDialog.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.NumericSelectorDialog>(global::TerraBrush.NumericSelectorDialog.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Objects>(global::TerraBrush.Objects.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ObjectsOctreeMultiMesh>(global::TerraBrush.ObjectsOctreeMultiMesh.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.PieMenu>(global::TerraBrush.PieMenu.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Snow>(global::TerraBrush.Snow.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Terrain>(global::TerraBrush.Terrain.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.TerrainControlDock>(global::TerraBrush.TerrainControlDock.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterRuntimeClass<global::TerraBrush.TerrainPositionInformation>(global::TerraBrush.TerrainPositionInformation.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterRuntimeClass<global::TerraBrush.TerrainPositionTextureInformation>(global::TerraBrush.TerrainPositionTextureInformation.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ToolsPieMenu>(global::TerraBrush.ToolsPieMenu.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.ToolInfo>(global::TerraBrush.ToolInfo.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.Water>(global::TerraBrush.Water.BindMethods);
        global::Godot.Bridge.GodotRegistry.RegisterClass<global::TerraBrush.TerraBrush>(global::TerraBrush.TerraBrush.BindMethods);
    }
    internal static void DeinitializeUserTypes(global::Godot.Bridge.InitializationLevel level)
    {
        if (level != global::Godot.Bridge.InitializationLevel.Scene)
        {
            return;
        }
    }
}
