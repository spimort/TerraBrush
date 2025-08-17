#ifndef TERRAIN_CONTROL_DOCK_H
#define TERRAIN_CONTROL_DOCK_H

#include "brush_decal.h"
#include "../terra_brush.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/flow_container.hpp>
#include <godot_cpp/classes/slider.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>

using namespace godot;

class TerrainControlDock : public Control {
    GDCLASS(TerrainControlDock, Control)

private:
    int _selectedBrushIndex;
    TerraBrush::TerrainToolType _selectedTool;
    TerraBrush::TerrainToolType _temporaryTool;
    int _selectedTextureIndex;
    int _selectedFoliageIndex;
    int _selectedObjectIndex;
    int _selectedMetaInfoIndex;

    FlowContainer *_brushesContainer;
    FlowContainer *_toolTypesContainer;
    FlowContainer *_texturesContainer;
    FlowContainer *_foliagesContainer;
    FlowContainer *_objectsContainer;
    FlowContainer *_metaInfoLayersContainer;
    Slider *_brushSizeSlider;
    Slider *_brushStrengthSlider;

    TerraBrush *_terraBrush;
    BrushDecal *_brushDecal;
    EditorResourcePreview *_editorResourcePreview;

    void buildLayout();
    void initializeBrushes();
    void initializeToolPreview();
    void initializeTextures();
    void initializeFoliages();
    void initializeObjects();
    void initializeMetaInfoLayers();
    void onBrushSizeValueChange(const float value);
    void onBrushStrengthValueChange(const float value);
    void updateSelectedBrush();
    void updateSelectedTerrainTool();
    void upateSelectedTextureSet();
    void updateSelectedFoliage();
    void updateSelectedObject();
    void updateSelectedMetaInfo();

protected:
    static void _bind_methods();

public:
    TerrainControlDock();
    ~TerrainControlDock();

    void _ready() override;

    void set_terraBrush(TerraBrush *value);
    void set_brushDecal(BrushDecal *value);
    void set_editorResourcePreview(EditorResourcePreview *value);

    void setBrushSize(int value);
    void setBrushStrength(float value);
    void setSelectedBrushIndex(const int index);
    void selectToolType(const TerraBrush::TerrainToolType toolType);
    void setSelectedTextureIndex(const int index);
    void setSelectedFoliageIndex(const int index);
    void setSelectedObjectIndex(const int index);
    void setSelectedMetaInfoIndex(const int index);
    void setShiftPressed(bool pressed);
};
#endif