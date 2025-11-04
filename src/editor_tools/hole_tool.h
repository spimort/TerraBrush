#ifndef HOLE_TOOL_H
#define HOLE_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class HoleTool : public ToolBase{
    GDCLASS(HoleTool, ToolBase);

private:
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    void beginPaint() override;
    void endPaint() override;
    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;

public:
    HoleTool();
    ~HoleTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif