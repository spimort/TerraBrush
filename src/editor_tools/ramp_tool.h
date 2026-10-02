#ifndef RAMP_TOOL_H
#define RAMP_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class RampTool : public ToolBase {
    GDCLASS(RampTool, ToolBase);

private:
    Vector3 _initialPoint = Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
    Node3D *_initialPointMesh = nullptr;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
    bool _preventPaint = false;

    void updateInitialPointMesh();
    void clearInitialPointMesh();
    void forEachRampPixel(int brushSize, Ref<Image> &brushImage, Color targetPixel, Vector2 fromPosition, Vector2 toPosition, std::function<void(Vector2, Color, float, ImageZoneInfo)> callback);

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    String getToolInfo(TerrainToolType toolType) override;
    void beginPaint() override;
    void endPaint() override;
    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;
    void beforeDeselect() override;

public:
    RampTool();
    ~RampTool();

    void init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones) override;

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) override;

    Vector3 getInitialPoint();
    void updateInitialPoint(Vector3 value);
};
#endif
