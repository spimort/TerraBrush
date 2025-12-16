#ifndef TERRAIN_CONTROL_DOCK_H
#define TERRAIN_CONTROL_DOCK_H

#include "../terra_brush.h"
#include "../misc/enums.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/flow_container.hpp>
#include <godot_cpp/classes/slider.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>

using namespace godot;

class TerrainControlDock : public Control {
    GDCLASS(TerrainControlDock, Control);

private:
    int _selectedBrushIndex = 0;
    TerrainToolType _selectedTool = TerrainToolType::TERRAINTOOLTYPE_NONE;
    int _selectedTextureIndex = 0;
    int _selectedFoliageIndex = 0;
    int _selectedObjectIndex = 0;
    int _selectedMetaInfoIndex = 0;
    Color _selectedColor = Color(0, 0, 0, 1.0);

    FlowContainer *_brushesContainer = nullptr;
    FlowContainer *_toolTypesContainer = nullptr;
    FlowContainer *_texturesContainer = nullptr;
    FlowContainer *_foliagesContainer = nullptr;
    FlowContainer *_objectsContainer = nullptr;
    FlowContainer *_metaInfoLayersContainer = nullptr;
    Slider *_brushSizeSlider = nullptr;
    Slider *_brushStrengthSlider = nullptr;
    ColorPickerButton *_colorPickerButton = nullptr;

    TerraBrush *_terraBrush = nullptr;
    EditorResourcePreview *_editorResourcePreview = nullptr;

    void buildLayout();
    void initializeBrushes();
    void initializeToolPreview();
    void initializeTextures();
    void initializeFoliages();
    void initializeObjects();
    void initializeMetaInfoLayers();
    void onBrushSizeValueChange(const float value);
    void onBrushStrengthValueChange(const float value);
    void onSelectedBrushIndexChange(const int index);
    void onSelectedToolTypeChange(const TerrainToolType toolType);
    void onSelectedTextureIndexChange(const int index);
    void onSelectedFoliageIndexChange(const int index);
    void onSelectedObjectIndexChange(const int index);
    void onSelectedMetaInfoIndexChange(const int index);
    void onSelectedColorChange(const Color value);
    void updateSelectedBrush();
    void updateSelectedTerrainTool();
    void upateSelectedTextureSet();
    void updateSelectedFoliage();
    void updateSelectedObject();
    void updateSelectedMetaInfo();
    void updateSelectedColor();

protected:
    static void _bind_methods();

public:
    TerrainControlDock();
    ~TerrainControlDock();

    void _ready() override;

    void set_terraBrush(TerraBrush *value);
    void set_editorResourcePreview(EditorResourcePreview *value);

    void setBrushSize(int value);
    void setBrushStrength(float value);
    void setSelectedBrushIndex(const int index);
    void selectToolType(const TerrainToolType toolType);
    void setSelectedTextureIndex(const int index);
    void setSelectedFoliageIndex(const int index);
    void setSelectedObjectIndex(const int index);
    void setSelectedMetaInfoIndex(const int index);
    void setSelectedColor(const Color value);
};
#endif