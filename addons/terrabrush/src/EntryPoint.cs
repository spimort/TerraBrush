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

        GodotRegistry.RegisterInternalClass<ZoneResource>(ZoneResource.BindMethods);
        GodotRegistry.RegisterInternalClass<FoliageDefinitionResource>(FoliageDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<FoliageResource>(FoliageResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectResource>(ObjectResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODMeshDefinitionResource>(ObjectOctreeLODMeshDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODMeshesDefinitionResource>(ObjectOctreeLODMeshesDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<ObjectOctreeLODDefinitionResource>(ObjectOctreeLODDefinitionResource.BindMethods);
        GodotRegistry.RegisterInternalClass<MetaInfoLayer>(MetaInfoLayer.BindMethods);
        GodotRegistry.RegisterInternalClass<TerraBrushTool>(TerraBrushTool.BindMethods);
        GodotRegistry.RegisterClass<TerraBrush>(TerraBrush.BindMethods);

        // GodotRegistry.AddEditorPluginByType<Plugin>();

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