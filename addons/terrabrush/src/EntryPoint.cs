using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Godot;
using Godot.Bridge;
using Godot.Collections;

[assembly:DisableGodotEntryPointGeneration]
[assembly: DisableRuntimeMarshalling]

namespace TerraBrush;

public class EntryPoint {
    [UnmanagedCallersOnly(EntryPoint = "terrabrush_init")]
    public static bool ExtensionInit(nint getProcAddress, nint library, nint initialization)
    {
        GodotBridge.Initialize(getProcAddress, library, initialization, config =>
        {
            config.SetMinimumLibraryInitializationLevel(InitializationLevel.Scene);
            config.RegisterInitializer(InitializeExtension);
            config.RegisterTerminator(DeInitializeExtension);
        });

        return true;
    }

    public static void InitializeExtension(InitializationLevel level)
    {
        if (level != InitializationLevel.Scene)
        {
            return;
        }

        GD.Print("Initialize TestAddon extension");

        _ = new GodotArray<int>();
        _ = new GodotArray<FoliageResource>();
        _ = new GodotArray<ObjectResource>();
        _ = new GodotArray<ObjectOctreeLODMeshDefinitionResource>();
        _ = new GodotArray<ObjectOctreeLODMeshesDefinitionResource>();
        _ = new GodotArray<ObjectOctreeLODDefinitionResource>();
        _ = new GodotArray<PackedScene>();
        _ = new GodotArray<MetaInfoLayer>();
        _ = new GodotArray<TerrainPositionTextureInformation>();
        _ = new GodotArray<Texture2D>();
        _ = new GodotArray<TextureSetResource>();
        _ = new GodotArray<ImageTexture>();
        _ = new GodotArray<ZoneResource>();

        GodotRegistry.RegisterInternalClass<BrushDecal>(BrushDecal.BindMethods);
        GodotRegistry.RegisterInternalClass<BrushNumericSelector>(BrushNumericSelector.BindMethods);
        GodotRegistry.RegisterInternalClass<Clipmap>(Clipmap.BindMethods);
        GodotRegistry.RegisterInternalClass<CustomContentPieMenu>(CustomContentPieMenu.BindMethods);
        GodotRegistry.RegisterInternalClass<Foliage>(Foliage.BindMethods);
        GodotRegistry.RegisterInternalClass<FoliageDefinitionResource>(FoliageDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<FoliageResource>(FoliageResource.BindMethods);
        GodotRegistry.RegisterInternalClass<KeybindSettings>(KeybindSettings.BindMethods);
        GodotRegistry.RegisterInternalClass<KeyListenDialog>(KeyListenDialog.BindMethods);
        GodotRegistry.RegisterInternalClass<NumericSelectorDialog>(NumericSelectorDialog.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectResource>(ObjectResource.BindMethods);
        GodotRegistry.RegisterInternalClass<Objects>(Objects.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectDefinitionResource>(ObjectDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODMeshDefinitionResource>(ObjectOctreeLODMeshDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODMeshesDefinitionResource>(ObjectOctreeLODMeshesDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODDefinitionResource>(ObjectOctreeLODDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectsOctreeMultiMesh>(ObjectsOctreeMultiMesh.BindMethods);
        GodotRegistry.RegisterInternalClass<PieMenu>(PieMenu.BindMethods);
        GodotRegistry.RegisterInternalClass<Snow>(Snow.BindMethods);
        GodotRegistry.RegisterInternalClass<Terrain>(Terrain.BindMethods);
        GodotRegistry.RegisterInternalClass<TerrainControlDock>(TerrainControlDock.BindMethods);
        GodotRegistry.RegisterInternalClass<ToolsPieMenu>(ToolsPieMenu.BindMethods);
        GodotRegistry.RegisterInternalClass<Water>(Water.BindMethods);
        GodotRegistry.RegisterInternalClass<DockPreviewButton>(DockPreviewButton.BindMethods);
        GodotRegistry.RegisterInternalClass<ToolPreview>(ToolPreview.BindMethods);
        GodotRegistry.RegisterInternalClass<MetaInfoLayer>(MetaInfoLayer.BindMethods);
        GodotRegistry.RegisterInternalClass<SnowResource>(SnowResource.BindMethods);
        GodotRegistry.RegisterInternalClass<TextureSetResource>(TextureSetResource.BindMethods);
        GodotRegistry.RegisterInternalClass<TextureSetsResource>(TextureSetsResource.BindMethods);
        GodotRegistry.RegisterInternalClass<WaterResource>(WaterResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ZoneResource>(ZoneResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ZonesResource>(ZonesResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ImportDialog>(ImportDialog.BindMethods);
        GodotRegistry.RegisterInternalClass<ImportImageRow>(ImportImageRow.BindMethods);
        GodotRegistry.RegisterInternalClass<ToolInfo>(ToolInfo.BindMethods);

        GodotRegistry.RegisterRuntimeClass<TerrainPositionInformation>(TerrainPositionInformation.BindMethods);
        GodotRegistry.RegisterRuntimeClass<TerrainPositionTextureInformation>(TerrainPositionTextureInformation.BindMethods);

        GodotRegistry.RegisterInternalClass<TerraBrushTool>(TerraBrushTool.BindMethods);
        GodotRegistry.RegisterClass<TerraBrush>(TerraBrush.BindMethods);

        GodotRegistry.AddEditorPluginByType<Plugin>();

        GD.Print("Initialized...");
    }

    public static void DeInitializeExtension(InitializationLevel level)
    {
        if (level != InitializationLevel.Scene)
        {
            return;
        }

        GD.Print("DeInitialize TestAddon extension");
    }
}