#include "entry_point.h"

#include "terra_brush.h"
#include "terra_brush_plugin.h"

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

#include "editor_tools/tool_base.h"
#include "editor_tools/sculpt_tool.h"
#include "editor_tools/set_height_tool.h"
#include "editor_tools/set_angle_tool.h"
#include "editor_tools/texture_tool.h"
#include "editor_tools/foliage_tool.h"

#include "misc/keybind_manager.h"

#include "octree/point_octree.h"
#include "octree/point_octree_bounding_box.h"
#include "octree/point_octree_node.h"
#include "octree/point_octree_object.h"
#include "octree/point_octree_ray.h"

#include "nodes/clipmap.h"
#include "nodes/foliage.h"
#include "nodes/objects.h"
#include "nodes/objects_octree_multi_mesh.h"
#include "nodes/objects_base.h"
#include "nodes/snow.h"
#include "nodes/terrain.h"
#include "nodes/water.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>

using namespace godot;

void initialize_terrabrush_module(ModuleInitializationLevel level) {
    if (level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        // Octree stuff
        GDREGISTER_INTERNAL_CLASS(PointOctree);
        GDREGISTER_INTERNAL_CLASS(PointOctreeBoundingBox);
        GDREGISTER_INTERNAL_CLASS(PointOctreeNode);
        GDREGISTER_INTERNAL_CLASS(PointOctreeObject);
        GDREGISTER_INTERNAL_CLASS(PointOctreeRay);
        GDREGISTER_INTERNAL_CLASS(ObjectsOctreeNodeInfo);

        // Internal nodes
        GDREGISTER_INTERNAL_CLASS(BrushDecal);
        GDREGISTER_INTERNAL_CLASS(BrushNumericSelector);
        GDREGISTER_INTERNAL_CLASS(Clipmap);
        GDREGISTER_INTERNAL_CLASS(CustomContentPieMenu);
        GDREGISTER_INTERNAL_CLASS(Foliage);
        // GodotRegistry.RegisterInternalClass<KeybindSettings>(KeybindSettings.BindMethods);
        // GodotRegistry.RegisterInternalClass<KeyListenDialog>(KeyListenDialog.BindMethods);
        // GodotRegistry.RegisterInternalClass<NumericSelectorDialog>(NumericSelectorDialog.BindMethods);
        GDREGISTER_INTERNAL_CLASS(ObjectsBase);
        GDREGISTER_INTERNAL_CLASS(Objects);
        GDREGISTER_INTERNAL_CLASS(ObjectsOctreeMultiMesh);
        GDREGISTER_INTERNAL_CLASS(PieMenu);
        GDREGISTER_INTERNAL_CLASS(Snow);
        GDREGISTER_INTERNAL_CLASS(Terrain);
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

        // Resources
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

        // Public nodes
        GDREGISTER_CLASS(TerraBrush);

        print_line("Init TerraBrush scene plugin");
        return;
    }

    if (level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        // Editor tools
        GDREGISTER_INTERNAL_CLASS(ToolBase);
        GDREGISTER_INTERNAL_CLASS(SculptTool);
        GDREGISTER_INTERNAL_CLASS(SetHeightTool);
        GDREGISTER_INTERNAL_CLASS(SetAngleTool);
        GDREGISTER_INTERNAL_CLASS(TextureTool);
        GDREGISTER_INTERNAL_CLASS(FoliageTool);

        GDREGISTER_INTERNAL_CLASS(TerraBrushPlugin);
        EditorPlugins::add_by_type<TerraBrushPlugin>();

        print_line("Init TerraBrush editor plugin");
        return;
    }
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