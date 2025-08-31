#ifndef TERRA_BRUSH_PLUGIN_H
#define TERRA_BRUSH_PLUGIN_H

#include "terra_brush.h"
#include "editor_nodes/terrain_control_dock.h"
#include "editor_nodes/brush_decal.h"
#include "editor_nodes/tool_info.h"
#include "editor_nodes/custom_content_pie_menu.h"
#include "editor_tools/tool_base.h"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_box.hpp>

using namespace godot;

class TerraBrushPlugin : public EditorPlugin {
    GDCLASS(TerraBrushPlugin, EditorPlugin);

private:
    const float InfinityValue = std::numeric_limits<float>::infinity();
    const float UpdateDelay = 0.005f;
    const int ToolInfoOffset = 20;
    static constexpr const char* OverlayActionNameKey = "ActionName";

    Control *_terrainDockContainer;
    TerrainControlDock *_terrainControlDock;
    BrushDecal *_brushDecal;
    TerraBrush *_currentTerraBrushNode;
    ToolInfo *_toolInfo;
    EditorUndoRedoManager *_undoRedo;
    Node *_editorViewportsContainer;
    Array _editorViewports;
    Control *_overlaySelector;
    Button *_updateTerrainSettingsButton;
    CheckBox *_autoAddZonesCheckbox;

    Ref<ToolBase> _currentTool;
    TerrainToolType _currentToolType;
    Ref<Image> _brushImage;
    int _brushSize;
    int _brushStrength;
    bool _isMousePressed;
    Vector3 _mouseHitPosition;
    float _updateTime;
    bool _preventInitialDo;
    bool _autoAddZones;

    void createCustomSetting(String name, Variant defaultValue, GDExtensionVariantType type, PropertyHint hint = PropertyHint::PROPERTY_HINT_NONE, String hintString = "");
    void handleKeyBindings();
    void onUndoTexture(Ref<ImageTexture> imageTexture, PackedByteArray previousImageData);
    void onUndoRedo();
    Vector3 getRayCastWithTerrain(Camera3D *editorCamera);
    Vector3 getMouseClickToZoneHeight(Vector3 from, Vector3 direction);
    void removeDock();
    void onEditTerrainNode(TerraBrush *terraBrush);
    void addDock();
    void onExitEditTerrainNode();
    Node *getEditorViewportsContainer();
    Node *getEditorViewportsContainerRecursive(Node *node);
    Node *getActiveViewport();
    StringName hideOverlaySelector();
    void showToolPieMenu(StringName actionName);
    void showCustomContentPieMenu(String label, Callable addItems);
    void showCurrentToolMenu();
    void showBrushNumericSelector(int minVale, int maxValue, Color widgetColor, int initialValue, Callable onValueSelected, StringName actionName);
    void updateTerrainSettings();
    void updateAutoAddZonesSetting();
    void onPieMenuBrushSelected(const CustomContentPieMenu *customContentPieMenu);
    void onPieMenuBrushIndexSelected(const int brushIndex);
    void onBrushSizeSelected(const int value);
    void onBrushStrengthSelected(const int value);
    void onToolSelected(const TerrainToolType value);

protected:
    static void _bind_methods();

public:
    TerraBrushPlugin();
    ~TerraBrushPlugin();

    void _physics_process(double delta) override;
    void _enter_tree() override;
    void _exit_tree() override;
    void _edit(Object *object) override;
    int _forward_3d_gui_input(Camera3D *viewportCamera, const Ref<InputEvent> &event) override;
    void _save_external_data() override;
    bool _handles(Object *object) const override;
};
#endif