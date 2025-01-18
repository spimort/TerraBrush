using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Godot;
using Godot.Bridge;
using ClassDB = Godot.Bridge.ClassDB;

[assembly:DisableGodotEntryPointGeneration]
[assembly:DisableRuntimeMarshalling]

namespace TerraBrush;

public class EntryPoint {
    [UnmanagedCallersOnly(EntryPoint = "terrabrush_init")]
    public static bool ExtensionInit(nint getProcAddress, nint library, nint initialization) {
        GodotBridge.Initialize(getProcAddress, library, initialization, config => {
            config.SetMinimumLibraryInitializationLevel(InitializationLevel.Scene);
            config.RegisterInitializer(InitializeExtension);
            config.RegisterTerminator(DeInitializeExtension);
        });

        return true;
    }

    public static void InitializeExtension(InitializationLevel level) {
        if (level != InitializationLevel.Scene) {
            return;
        }

        GD.Print("Initialize TerraBrush extension");

        ClassDB.RegisterInternalClass<KeybindManager>(KeybindManager.BindMethods);
        ClassDB.RegisterInternalClass<BrushDecal>(BrushDecal.BindMethods);

        ClassDB.RegisterClass<TerraBrush>(TerraBrush.BindMethods);
        // ClassDB.RegisterClass<ZonesResource>(ZonesResource.BindMethods);
        ClassDB.RegisterClass<ZoneResource>(ZoneResource.BindMethods);

        ClassDB.RegisterInternalClass<TerraBrushPlugin>(TerraBrushPlugin.BindMethods);
        EditorPlugins.AddByType<TerraBrushPlugin>();

        GD.Print("Initialized...");
    }

    public static void DeInitializeExtension(InitializationLevel level) {
        if (level != InitializationLevel.Scene) {
            return;
        }

        GD.Print("DeInitialize TerraBrush extension");
    }
}
