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

    Control *_terrainDockContainer = nullptr;
    TerrainControlDock *_terrainControlDock = nullptr;
    BrushDecal *_brushDecal = nullptr;
    TerraBrush *_currentTerraBrushNode = nullptr;
    ToolInfo *_toolInfo = nullptr;
    EditorUndoRedoManager *_undoRedo = nullptr;
    Node *_editorViewportsContainer = nullptr;
    Array _editorViewports = Array();
    Control *_overlaySelector = nullptr;
    Button *_updateTerrainSettingsButton = nullptr;
    CheckBox *_autoAddZonesCheckbox = nullptr;

    Ref<ToolBase> _currentTool = nullptr;
    TerrainToolType _currentToolType = TerrainToolType::TERRAINTOOLTYPE_TERRAINADD;
    Ref<Image> _brushImage = nullptr;
    int _brushSize = 0;
    float _brushStrength = 0;
    int _brushIndex = 0;
    int _textureIndex = 0;
    int _foliageIndex = 0;
    int _objectIndex = 0;
    int _metaInfoLayerIndex = 0;
    bool _isMousePressed = false;
    Vector3 _mouseHitPosition = Vector3();
    float _updateTime = 0;
    bool _preventInitialDo = false;
    bool _autoAddZones = false;

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
    void onDockToolTypeSelected(const TerrainToolType toolType);
    void onDockBrushSelected(const int index, const Ref<Image> &image);
    void onDockBrushSizeChanged(const int value);
    void onDockBrushStrengthChanged(const float value);
    void onDockTextureSelected(const int index);
    void onDockFoliageSelected(const int index);
    void onDockObjectSelected(const int index);
    void onDockMetaInfoSelected(const int index);

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