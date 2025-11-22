#ifndef TERRA_BRUSH_EDITOR_H
#define TERRA_BRUSH_EDITOR_H

#include "terra_brush.h"
#include "editor_nodes/brush_decal.h"
#include "editor_nodes/tool_info.h"
#include "editor_nodes/custom_content_pie_menu.h"
#include "editor_tools/tool_base.h"
#include "misc/enums.h"
#include "misc/utils.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

using namespace godot;

class TerraBrushEditor : public Node {
    GDCLASS(TerraBrushEditor, Node);

private:
    const float UpdateDelay = 0.005f;
    const int ToolInfoOffset = 20;
    static constexpr const char* OverlayActionNameKey = "ActionName";

    Node *_containerNode = nullptr;
    TerraBrush *_terraBrushNode = nullptr;

    BrushDecal *_brushDecal = nullptr;
    ToolInfo *_toolInfo = nullptr;
    EditorUndoRedoManager *_undoRedo = nullptr;
    Node *_viewportsContainer = nullptr;
    Array _viewports = Array();
    Control *_overlaySelector = nullptr;

    Ref<ToolBase> _currentTool = nullptr;
    TerrainToolType _currentToolType = TerrainToolType::TERRAINTOOLTYPE_TERRAINADD;
    TerrainToolType _temporaryTool = TerrainToolType::TERRAINTOOLTYPE_NONE;
    Ref<Image> _brushImage = nullptr;
    Ref<Image> _originalBrushImage = nullptr;
    int _brushSize = 0;
    float _brushStrength = 0;
    int _brushIndex = 0;
    int _textureIndex = 0;
    int _foliageIndex = 0;
    int _objectIndex = 0;
    int _metaInfoLayerIndex = 0;
    float _selectedSetHeight = 0;
    float _selectedSetAngle = 0;
    bool _autoAddZones = false;
    Vector3 _selectedSetAngleInitialPoint = Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);

    bool _isMousePressed = false;
    Vector3 _mouseHitPosition = Vector3();
    float _updateTime = 0;
    bool _preventInitialDo = false;

    bool _enabled = false;
    bool _enableOnReady = true;
    bool _allowBuiltInToolSelectors = true;
    int _defaultBrushSize = 100;
    int _maxBrushSize = 200;
    float _defaultBrushStrength = 0.1;

    void onUndoRedo();
    Vector3 getRayCastWithTerrain(Camera3D *camera);
    Vector3 getMouseClickToZoneHeight(Vector3 from, Vector3 direction);
    StringName hideOverlaySelector();
    void showToolPieMenu(Viewport *viewport, StringName actionName);
    void showCustomContentPieMenu(Viewport *viewport, String label, std::function<void(CustomContentPieMenu*)> addItems);
    void showCurrentToolMenu(Viewport *viewport);
    void showBrushNumericSelector(Viewport *viewport, int minVale, int maxValue, Color widgetColor, int initialValue, Callable onValueSelected, StringName actionName);
    void onPieMenuBrushSelected(const CustomContentPieMenu *customContentPieMenu);
    void onPieMenuBrushIndexSelected(const int brushIndex);
    void onBrushSizeSelected(const int value);
    void onBrushStrengthSelected(const int value);
    void onToolSelected(const TerrainToolType value);
    void onCustomContentSelectorTextureSelected(const int index);
    void onCustomContentSelectorFoliageSelected(const int index);
    void onCustomContentSelectorObjectSelected(const int index);
    void onCustomContentSelectorMetaInfoSelected(const int index);
    void updateCurrentTool();
    Ref<ToolBase> getToolForType(TerrainToolType toolType);
    void beforeDeselectTool();
    void updateAutoAddZonesSetting();
    void updateBrushImage();
    void setShiftPressed(const bool pressed);

protected:
    static void _bind_methods();

public:
    TerraBrushEditor();
    ~TerraBrushEditor();

    void _ready() override;
    void _physics_process(double delta) override;
    void _unhandled_input(const Ref<InputEvent> &event) override;

    void set_containerNode(Node *containerNode);
    void set_undoRedo(EditorUndoRedoManager *undoRedo);

    bool get_enabled() const;
    void set_enabled(const bool value);

    bool get_enableOnReady() const;
    void set_enableOnReady(const bool value);

    bool get_allowBuiltInToolSelectors() const;
    void set_allowBuiltInToolSelectors(const bool value);

    int get_defaultBrushSize() const;
    void set_defaultBrushSize(const int value);

    int get_maxBrushSize() const;
    void set_maxBrushSize(const int value);

    float get_defaultBrushStrength() const;
    void set_defaultBrushStrength(const float value);

    TerraBrush *get_terraBrushNode() const;
    void set_terraBrushNode(const TerraBrush *terraBrushNode);

    int get_brushIndex() const;
    void set_brushIndex(const int value);

    int get_brushSize() const;
    void set_brushSize(const int value);

    float get_brushStrength() const;
    void set_brushStrength(const float value);

    TerrainToolType get_selectedToolType() const;
    void set_selectedToolType(const TerrainToolType value);

    int get_selectedTextureIndex() const;
    void set_selectedTextureIndex(const int value);

    int get_selectedFoliageIndex() const;
    void set_selectedFoliageIndex(const int value);

    int get_selectedObjectIndex() const;
    void set_selectedObjectIndex(const int value);

    int get_selectedMetaInfoIndex() const;
    void set_selectedMetaInfoIndex(const int value);

    bool get_autoAddZones() const;
    void set_autoAddZones(const bool value);

    bool onGuiInput(Camera3D *camera, const Ref<InputEvent> &event);
};
#endif