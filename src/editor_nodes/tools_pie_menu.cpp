 #include "tools_pie_menu.h"
 #include "tool_preview.h"
 #include "../terra_brush.h"
 #include "../misc/enums.h"

 #include <godot_cpp/classes/resource_loader.hpp>
 #include <godot_cpp/classes/node.hpp>

 using namespace godot;

void ToolsPieMenu::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onToolSelected", "toolType"), &ToolsPieMenu::onToolSelected);
}

ToolsPieMenu::ToolsPieMenu() {

}

ToolsPieMenu::~ToolsPieMenu() {}

void ToolsPieMenu::_ready() {
    set_anchors_and_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);

    _pieMenu = memnew(PieMenu);
    _pieMenu->set_label("Tools");
    _pieMenu->set_anchors_and_offsets_preset(LayoutPreset::PRESET_TOP_LEFT);

    ToolPreview *toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Raise terrain");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Lower terrain");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_smooth.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Smooth terrain");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_flatten.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Flatten terrain");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSETHEIGHT);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_height.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Set terrain height");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_TERRAINSETANGLE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/map_set_angle.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Set terrain angle");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_PAINT);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/paint.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::Circle);
    toolPreview->set_tooltip_text("Paint texture");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add foliage");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_FOLIAGEREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/foliage_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove foliage");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_OBJECTADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add objects");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_OBJECTREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/object_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove objects");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add water");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/water_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove water");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add water flow");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_WATERFLOWREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/flow_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove water flow");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_SNOWADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add snow");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_SNOWREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/snow_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove snow");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_HOLEADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Add hole");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_HOLEREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/holes_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Remove hole");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_LOCKADD);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_add.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Lock");
    _pieMenu->add_child(toolPreview);

    toolPreview = memnew(ToolPreview);
    toolPreview->set_toolType(TerrainToolType::TERRAINTOOLTYPE_LOCKREMOVE);
    toolPreview->set_buttonImage(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/Icons/lock_remove.png"));
    toolPreview->set_iconType(DockPreviewButton::IconType::CircleWithSign);
    toolPreview->set_tooltip_text("Unlock");
    _pieMenu->add_child(toolPreview);

    _pieMenu->add_child(memnew(Control)); // Splitter

    add_child(_pieMenu); // This is done at the end so the menu calculates the content in the _ready function

    for (int i = 0; i < _pieMenu->get_child_count(); i++) {
        Node *childNode = _pieMenu->get_child(i);

        ToolPreview *toolPreview = Object::cast_to<ToolPreview>(childNode);
        if (toolPreview != nullptr) {
            toolPreview->set_onSelect(Callable(this, "onToolSelected").bind(toolPreview->get_toolType()));
        }
    }
}

void ToolsPieMenu::set_onToolSelected(const Callable value) {
    _onToolSelected = value;
}

void ToolsPieMenu::onToolSelected(const TerrainToolType toolType) {
    if (!_onToolSelected.is_null()) {
        _onToolSelected.call(toolType);
    }
}