#ifndef SCULPT_TOOL_H
#define SCULPT_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"

using namespace godot;

class SculptTool : public ToolBase{
    GDCLASS(SculptTool, ToolBase);

private:
    int _sculptingMultiplier = 1;
    HashSet<Ref<ZoneResource>> _sculptedZones = HashSet<Ref<ZoneResource>>();

    void sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition);
    void flatten(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition);
    void smooth(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition);

protected:
    static void _bind_methods();

    bool getApplyResolution() override;
    void beginPaint() override;
    void endPaint() override;
    Ref<ImageTexture> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    SculptTool();
    ~SculptTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;
};
#endif