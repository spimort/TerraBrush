#include "entry_point.h"

#include "terra_brush.h"

#include "editor_nodes/brush_decal.h"
#include "editor_nodes/brush_numeric_selector.h"
#include "editor_nodes/custom_content_pie_menu.h"
#include "editor_nodes/dock_preview_button.h"
#include "editor_nodes/pie_menu.h"
#include "editor_nodes/tool_preview.h"
#include "editor_nodes/tools_pie_menu.h"
#include "editor_nodes/terrain_control_dock.h"
#include "editor_nodes/tool_info.h"

#include "editor_resources/foliage_resource.h"
#include "editor_resources/foliage_definition_resource.h"
#include "editor_resources/meta_info_layer_resource.h"
#include "editor_resources/object_resource.h"
#include "editor_resources/object_definition_resource.h"
#include "editor_resources/object_octree_lod_definition_resource.h"
#include "editor_resources/object_octree_lod_mesh_definition_resource.h"
#include "editor_resources/object_octree_lod_meshes_definition_resource.h"
#include "editor_resources/snow_resource.h"
#include "editor_resources/texture_set_resource.h"
#include "editor_resources/texture_sets_resource.h"
#include "editor_resources/water_resource.h"
#include "editor_resources/zone_resource.h"
#include "editor_resources/zones_resource.h"

#include "misc/keybind_manager.h"

#include "nodes/clipmap.h"
#include "nodes/foliage.h"
#include "nodes/snow.h"
#include "nodes/water.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_terrabrush_module(ModuleInitializationLevel level) {
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // GDREGISTER_CLASS(TerraBrush);


    GDREGISTER_INTERNAL_CLASS(BrushDecal);
    GDREGISTER_INTERNAL_CLASS(BrushNumericSelector);
    GDREGISTER_INTERNAL_CLASS(Clipmap);
    GDREGISTER_INTERNAL_CLASS(CustomContentPieMenu);
    GDREGISTER_INTERNAL_CLASS(Foliage);
    // GodotRegistry.RegisterInternalClass<KeybindSettings>(KeybindSettings.BindMethods);
    // GodotRegistry.RegisterInternalClass<KeyListenDialog>(KeyListenDialog.BindMethods);
    // GodotRegistry.RegisterInternalClass<NumericSelectorDialog>(NumericSelectorDialog.BindMethods);
    // GodotRegistry.RegisterInternalClass<Objects>(Objects.BindMethods);
    // GodotRegistry.RegisterInternalClass<ObjectsOctreeMultiMesh>(ObjectsOctreeMultiMesh.BindMethods);
    GDREGISTER_INTERNAL_CLASS(PieMenu);
    GDREGISTER_INTERNAL_CLASS(Snow);
    // GodotRegistry.RegisterInternalClass<Terrain>(Terrain.BindMethods);
    GDREGISTER_INTERNAL_CLASS(TerrainControlDock);
    GDREGISTER_INTERNAL_CLASS(ToolsPieMenu);
    GDREGISTER_INTERNAL_CLASS(Water);
    GDREGISTER_INTERNAL_CLASS(DockPreviewButton);
    GDREGISTER_INTERNAL_CLASS(ToolPreview);
    // GodotRegistry.RegisterInternalClass<ImportDialog>(ImportDialog.BindMethods);
    // GodotRegistry.RegisterInternalClass<ImportImageRow>(ImportImageRow.BindMethods);
    GDREGISTER_INTERNAL_CLASS(ToolInfo);
    GDREGISTER_INTERNAL_CLASS(KeybindManager);

    // GodotRegistry.RegisterRuntimeClass<TerrainPositionInformation>(TerrainPositionInformation.BindMethods);
    // GodotRegistry.RegisterRuntimeClass<TerrainPositionTextureInformation>(TerrainPositionTextureInformation.BindMethods);

    GDREGISTER_CLASS(FoliageResource);
    GDREGISTER_CLASS(FoliageDefinitionResource);
    GDREGISTER_CLASS(MetaInfoLayerResource);
    GDREGISTER_CLASS(ObjectResource);
    GDREGISTER_CLASS(ObjectDefinitionResource);
    GDREGISTER_CLASS(ObjectOctreeLODDefinitionResource);
    GDREGISTER_CLASS(ObjectOctreeLODMeshDefinitionResource);
    GDREGISTER_CLASS(ObjectOctreeLODMeshesDefinitionResource);
    GDREGISTER_CLASS(SnowResource);
    GDREGISTER_CLASS(TextureSetResource);
    GDREGISTER_CLASS(TextureSetsResource);
    GDREGISTER_CLASS(WaterResource);
    GDREGISTER_CLASS(ZoneResource);
    GDREGISTER_CLASS(ZonesResource);

    // GodotRegistry.RegisterInternalClass<TerraBrushTool>(TerraBrushTool.BindMethods);
    // GodotRegistry.RegisterClass<TerraBrush>(TerraBrush.BindMethods);

    print_line("Init TerraBrush");
}

void uninitialize_terrabrush_module(ModuleInitializationLevel level) {
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    print_line("DeInit TerraBrush");
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT terrabrush_library_init(GDExtensionInterfaceGetProcAddress get_proc_address, const GDExtensionClassLibraryPtr library, GDExtensionInitialization *initialization) {
    godot::GDExtensionBinding::InitObject init_obj(get_proc_address, library, initialization);

    init_obj.register_initializer(initialize_terrabrush_module);
    init_obj.register_terminator(uninitialize_terrabrush_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}