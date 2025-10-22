#ifndef TEXTURE_TOOL_H
#define TEXTURE_TOOL_H

#include "tool_base.h"
#include "../misc/hash_utils.h"

#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class TextureTool : public ToolBase{
    GDCLASS(TextureTool, ToolBase);

private:
    int _selectedTextureIndex = -1;

protected:
    static void _bind_methods();

    void beginPaint() override;
    void endPaint() override;

public:
    TextureTool();
    ~TextureTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedTextureIndex(int value);
};
#endif