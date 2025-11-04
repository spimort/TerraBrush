#ifndef OBJECT_TOOL_H
#define OBJECT_TOOL_H

#include "tool_base.h"
#include "../misc/hash_utils.h"

using namespace godot;

class ObjectTool : public ToolBase{
    GDCLASS(ObjectTool, ToolBase);

private:
    int _selectedObjectIndex = -1;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

protected:
    static void _bind_methods();

    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;
    void beginPaint() override;
    void endPaint() override;

public:
    ObjectTool();
    ~ObjectTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedObjectIndex(int value);
};
#endif