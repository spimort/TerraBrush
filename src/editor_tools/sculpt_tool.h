#ifndef SCULPT_TOOL_H
#define SCULPT_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/compute_shader_executer.h"

#include <godot_cpp/classes/rendering_device.hpp>

using namespace godot;

struct SculptShaderSettings {
    int numberOfHeightmaps;
    float brushStrength;
    int add;
};

struct SmoothShaderSettings {
    int numberOfHeightmaps;
    float brushStrength;
};

class SculptTool : public ToolBase {
    GDCLASS(SculptTool, ToolBase);

private:
    int _sculptingMultiplier = 1;
    int _smoothingMultiplier = 1;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
    Ref<ComputeShaderExecuter> _computeShaderExecuter = nullptr;
    Ref<ComputeShaderInstance> _sculptShaderInstance = nullptr;
    Ref<ComputeShaderInstance> _smoothShaderInstance = nullptr;

    void sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition);
    void flatten(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition);
    void smooth(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition, bool applyMultiplier = true);

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    void beginPaint() override;
    void endPaint() override;
    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;
    void beforeDeselect() override;

public:
    SculptTool();
    ~SculptTool();

    void init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones) override;
    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) override;
};
#endif
