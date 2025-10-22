#ifndef WATER_FLOW_TOOL_H
#define WATER_FLOW_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class WaterFlowTool : public ToolBase{
    GDCLASS(WaterFlowTool, ToolBase);

private:
    Vector2 _previousWaterMousePosition = Vector2(0, 0);
    Vector2 _previousWaterMouseDirection = Vector2(0, 0);

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    Ref<Image> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    WaterFlowTool();
    ~WaterFlowTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif