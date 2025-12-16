#ifndef COLOR_TOOL_H
#define COLOR_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class ColorTool : public ToolBase{
    GDCLASS(ColorTool, ToolBase);

private:
    Color _selectedColor = Color(0, 0, 0, 1.0);

protected:
    static void _bind_methods();

    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;

public:
    ColorTool();
    ~ColorTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedColor(Color value);
};
#endif