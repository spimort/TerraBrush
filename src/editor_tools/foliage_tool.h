#ifndef FOLIAGE_TOOL_H
#define FOLIAGE_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"

#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class FoliageTool : public ToolBase{
    GDCLASS(FoliageTool, ToolBase);

private:
    int _selectedFoliageIndex = -1;

protected:
    static void _bind_methods();
    Ref<Image> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    FoliageTool();
    ~FoliageTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedFoliageIndex(int value);
};
#endif