#ifndef WATER_TOOL_H
#define WATER_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class WaterTool : public ToolBase{
    GDCLASS(WaterTool, ToolBase);

private:
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

protected:
    static void _bind_methods();

    bool getApplyResolution() override;
    void beginPaint() override;
    void endPaint() override;
    Ref<ImageTexture> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    WaterTool();
    ~WaterTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif