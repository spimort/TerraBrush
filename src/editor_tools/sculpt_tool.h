#ifndef SCULPT_TOOL_H
#define SCULPT_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"

#include <godot_cpp/classes/rendering_device.hpp>

using namespace godot;

struct alignas(16) SculptSettings {
    float brushStrength;
    bool add;
};

class SculptTool : public ToolBase {
    GDCLASS(SculptTool, ToolBase);

private:
    int _sculptingMultiplier = 1;
    int _smoothingMultiplier = 1;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
    RenderingDevice *_renderingDevice = nullptr;
    RID _computeShader = RID();
    RID _uniformSet = RID();
    RID _pipeline = RID();
    RID _heightmapTextureParam = RID();
    RID _brushTextureParam = RID();
    RID _sculptSettingsBufferParam = RID();

    void sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition);
    void flatten(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition);
    void smooth(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition, bool applyMultiplier = true);

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    void beginPaint() override;
    void endPaint() override;
    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;

public:
    SculptTool();
    ~SculptTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) override;
};
#endif
