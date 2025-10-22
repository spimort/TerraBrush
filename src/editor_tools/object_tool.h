#ifndef OBJECT_TOOL_H
#define OBJECT_TOOL_H

#include "tool_base.h"
#include "../misc/hash_utils.h"

#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class ObjectTool : public ToolBase{
    GDCLASS(ObjectTool, ToolBase);

private:
    int _selectedObjectIndex = -1;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
    std::unordered_map<Ref<ZoneResource>, Ref<Image>> _heightmapImagesCache = std::unordered_map<Ref<ZoneResource>, Ref<Image>>();
    std::unordered_map<Ref<ZoneResource>, Ref<Image>> _waterImagesCache = std::unordered_map<Ref<ZoneResource>, Ref<Image>>();

protected:
    static void _bind_methods();

    Ref<Image> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;
    void beginPaint() override;
    void endPaint() override;

public:
    ObjectTool();
    ~ObjectTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedObjectIndex(int value);
};
#endif