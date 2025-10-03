#ifndef SNOW_TOOL_H
#define SNOW_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class SnowTool : public ToolBase{
    GDCLASS(SnowTool, ToolBase);

private:
protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    Ref<ImageTexture> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    SnowTool();
    ~SnowTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif