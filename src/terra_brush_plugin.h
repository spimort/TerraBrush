#ifndef TERRA_BRUSH_PLUGIN_H
#define TERRA_BRUSH_PLUGIN_H

#include "terra_brush.h"
#include "terra_brush_editor.h"
#include "editor_nodes/terrain_control_dock.h"
#include "misc/utils.h"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/menu_button.hpp>

using namespace godot;

class TerraBrushPlugin : public EditorPlugin {
    GDCLASS(TerraBrushPlugin, EditorPlugin);

    enum TerrainMenuButtonAction {
        TERRAINMENUBUTTONACTION_CREATETERRAIN = 1,
        TERRAINMENUBUTTONACTION_REMOVETERRAIN = 2,
        TERRAINMENUBUTTONACTION_UPDATETERRAIN = 3,
        TERRAINMENUBUTTONACTION_IMPORTTERRAIN = 4,
        TERRAINMENUBUTTONACTION_EXPORTTERRAIN = 5,
        TERRAINMENUBUTTONACTION_LOCKALLTERRAIN = 6,
        TERRAINMENUBUTTONACTION_UNLOCKALLTERRAIN = 7,
    };

private:
    TerraBrushEditor *_terraBrushEditor = nullptr;
    TerraBrush *_currentTerraBrushNode = nullptr;
    Control *_terrainDockContainer = nullptr;
    TerrainControlDock *_terrainControlDock = nullptr;
    MenuButton *_terrainMenuButton = nullptr;
    Button *_updateTerrainSettingsButton = nullptr;
    CheckBox *_autoAddZonesCheckbox = nullptr;

    void createCustomSetting(String name, Variant defaultValue, Variant::Type type, PropertyHint hint = PropertyHint::PROPERTY_HINT_NONE, String hintString = "");
    void handleKeyBindings();
    void removeDock();
    void onEditTerrainNode(TerraBrush *terraBrush);
    void addDock();
    void onExitEditTerrainNode();
    void updateTerrainSettings();
    void updateAutoAddZonesSetting();

    void onDockToolTypeSelected(const TerrainToolType toolType);
    void onDockBrushSelected(const int index);
    void onDockBrushSizeChanged(const int value);
    void onDockBrushStrengthChanged(const float value);
    void onDockTextureSelected(const int index);
    void onDockFoliageSelected(const int index);
    void onDockObjectSelected(const int index);
    void onDockMetaInfoSelected(const int index);
    void onDockColorSelected(const Color value);

    void onTerraBrushEditorToolTypeSelected(const TerrainToolType toolType);
    void onTerraBrushEditorBrushSelected(const int index);
    void onTerraBrushEditorBrushSizeChanged(const int value);
    void onTerraBrushEditorBrushStrengthChanged(const float value);
    void onTerraBrushEditorTextureSelected(const int index);
    void onTerraBrushEditorFoliageSelected(const int index);
    void onTerraBrushEditorObjectSelected(const int index);
    void onTerraBrushEditorMetaInfoSelected(const int index);
    void onTerraBrushEditorColorSelected(const Color value);
    void onTerraBrushEditorAutoAddZoneChanged(const bool value);

    void importTerrain();
    void exportTerrain();
    void onTerrainMenuItemPressed(const int id);

protected:
    static void _bind_methods();

public:
    TerraBrushPlugin();
    ~TerraBrushPlugin();

    void _enter_tree() override;
    void _exit_tree() override;
    void _edit(Object *object) override;
    int _forward_3d_gui_input(Camera3D *viewportCamera, const Ref<InputEvent> &event) override;
    void _save_external_data() override;
    bool _handles(Object *object) const override;
};
#endif