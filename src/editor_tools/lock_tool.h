#ifndef LOCK_TOOL_H
#define LOCK_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class LockTool : public ToolBase{
    GDCLASS(LockTool, ToolBase);

private:
protected:
    static void _bind_methods();

    Ref<Image> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    LockTool();
    ~LockTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif