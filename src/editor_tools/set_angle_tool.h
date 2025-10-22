#ifndef SET_ANGLE_TOOL_H
#define SET_ANGLE_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class SetAngleTool : public ToolBase {
    GDCLASS(SetAngleTool, ToolBase);

private:
    const float MaxAngle = 89.9f;

    float _setAngleValue = 0;
    Vector3 _setAngleInitialPoint = Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
    Node3D *_initialPointMesh = nullptr;
    std::unordered_set<Ref<ZoneResource>> _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

protected:
    static void _bind_methods();

    bool getApplyResolution() const override;
    String getToolInfo(TerrainToolType toolType) override;
    bool handleInput(TerrainToolType toolType, Ref<InputEvent> event) override;
    void beginPaint() override;
    void endPaint() override;
    Ref<Image> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;
    void beforeDeselect() override;
    void updateInitialPointMesh();
    void clearInitialPointMesh();

public:
    SetAngleTool();
    ~SetAngleTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    float getSetAngleValue();
    Vector3 getSetAngleInitialPoint();
    void updateSetAngleValue(float value);
    void updateSetAngleInitialPoint(Vector3 value);
};
#endif