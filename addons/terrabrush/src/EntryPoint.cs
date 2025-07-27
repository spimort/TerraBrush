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
        if (level == InitializationLevel.Scene) {
            GD.Print("Initialize TerraBrush Scene");

            using (var _ = new GodotArray<int>()) { }
            using (var _ = new GodotArray<FoliageResource>()) {}
            using (var _ = new GodotArray<ObjectResource>()) {}
            using (var _ = new GodotArray<ObjectOctreeLODMeshDefinitionResource>()) {}
            using (var _ = new GodotArray<ObjectOctreeLODMeshesDefinitionResource>()) {}
            using (var _ = new GodotArray<ObjectOctreeLODDefinitionResource>()) {}
            using (var _ = new GodotArray<PackedScene>()) {}
            using (var _ = new GodotArray<MetaInfoLayer>()) {}
            using (var _ = new GodotArray<TerrainPositionTextureInformation>()) {}
            using (var _ = new GodotArray<Texture2D>()) {}
            using (var _ = new GodotArray<TextureSetResource>()) {}
            using (var _ = new GodotArray<ImageTexture>()) {}
            using (var _ = new GodotArray<ZoneResource>()) {}

            GodotRegistry.RegisterInternalClass<BrushDecal>(BrushDecal.BindMethods);
            GodotRegistry.RegisterInternalClass<BrushNumericSelector>(BrushNumericSelector.BindMethods);
            GodotRegistry.RegisterInternalClass<Clipmap>(Clipmap.BindMethods);
            GodotRegistry.RegisterInternalClass<CustomContentPieMenu>(CustomContentPieMenu.BindMethods);
            GodotRegistry.RegisterInternalClass<Foliage>(Foliage.BindMethods);
            GodotRegistry.RegisterInternalClass<KeybindSettings>(KeybindSettings.BindMethods);
            GodotRegistry.RegisterInternalClass<KeyListenDialog>(KeyListenDialog.BindMethods);
            GodotRegistry.RegisterInternalClass<NumericSelectorDialog>(NumericSelectorDialog.BindMethods);
            GodotRegistry.RegisterInternalClass<Objects>(Objects.BindMethods);
            GodotRegistry.RegisterInternalClass<ObjectsOctreeMultiMesh>(ObjectsOctreeMultiMesh.BindMethods);
            GodotRegistry.RegisterInternalClass<PieMenu>(PieMenu.BindMethods);
            GodotRegistry.RegisterInternalClass<Snow>(Snow.BindMethods);
            GodotRegistry.RegisterInternalClass<Terrain>(Terrain.BindMethods);
            GodotRegistry.RegisterInternalClass<TerrainControlDock>(TerrainControlDock.BindMethods);
            GodotRegistry.RegisterInternalClass<ToolsPieMenu>(ToolsPieMenu.BindMethods);
            GodotRegistry.RegisterInternalClass<Water>(Water.BindMethods);
            GodotRegistry.RegisterInternalClass<DockPreviewButton>(DockPreviewButton.BindMethods);
            GodotRegistry.RegisterInternalClass<ToolPreview>(ToolPreview.BindMethods);
            GodotRegistry.RegisterInternalClass<ImportDialog>(ImportDialog.BindMethods);
            GodotRegistry.RegisterInternalClass<ImportImageRow>(ImportImageRow.BindMethods);
            GodotRegistry.RegisterInternalClass<ToolInfo>(ToolInfo.BindMethods);
            GodotRegistry.RegisterInternalClass<KeybindManager>(KeybindManager.BindMethods);

            GodotRegistry.RegisterRuntimeClass<TerrainPositionInformation>(TerrainPositionInformation.BindMethods);
            GodotRegistry.RegisterRuntimeClass<TerrainPositionTextureInformation>(TerrainPositionTextureInformation.BindMethods);

            GodotRegistry.RegisterClass<FoliageResource>(FoliageResource.BindMethods);
            GodotRegistry.RegisterClass<FoliageDefinitionResource>(FoliageDefinitionResource.BindMethods);
            GodotRegistry.RegisterClass<MetaInfoLayer>(MetaInfoLayer.BindMethods);
            GodotRegistry.RegisterClass<ObjectResource>(ObjectResource.BindMethods);
            GodotRegistry.RegisterClass<ObjectDefinitionResource>(ObjectDefinitionResource.BindMethods);
            GodotRegistry.RegisterClass<ObjectOctreeLODMeshDefinitionResource>(ObjectOctreeLODMeshDefinitionResource.BindMethods);
            GodotRegistry.RegisterClass<ObjectOctreeLODMeshesDefinitionResource>(ObjectOctreeLODMeshesDefinitionResource.BindMethods);
            GodotRegistry.RegisterClass<ObjectOctreeLODDefinitionResource>(ObjectOctreeLODDefinitionResource.BindMethods);
            GodotRegistry.RegisterClass<SnowResource>(SnowResource.BindMethods);
            GodotRegistry.RegisterClass<TextureSetResource>(TextureSetResource.BindMethods);
            GodotRegistry.RegisterClass<TextureSetsResource>(TextureSetsResource.BindMethods);
            GodotRegistry.RegisterClass<WaterResource>(WaterResource.BindMethods);
            GodotRegistry.RegisterClass<ZoneResource>(ZoneResource.BindMethods);
            GodotRegistry.RegisterClass<ZonesResource>(ZonesResource.BindMethods);

            GodotRegistry.RegisterInternalClass<TerraBrushTool>(TerraBrushTool.BindMethods);
            GodotRegistry.RegisterClass<TerraBrush>(TerraBrush.BindMethods);

            return;
        }

        if (level == InitializationLevel.Editor) {
            GD.Print("Initialize TerraBrush Plugin");

            GodotRegistry.RegisterInternalClass<TerraBrushPlugin>(TerraBrushPlugin.BindMethods);
            GodotRegistry.AddEditorPluginByType<TerraBrushPlugin>();
        }
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