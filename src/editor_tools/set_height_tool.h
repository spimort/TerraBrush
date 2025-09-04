#ifndef SET_HEIGHT_TOOL_H
#define SET_HEIGHT_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"

using namespace godot;

class SetHeightTool : public ToolBase {
    GDCLASS(SetHeightTool, ToolBase);

private:
    float _setHeightValue = 0;
    HashSet<Ref<ZoneResource>> _sculptedZones = HashSet<Ref<ZoneResource>>();

protected:
    static void _bind_methods();

    bool getApplyResolution() override;
    String getToolInfo(TerrainToolType toolType) override;
    bool handleInput(TerrainToolType toolType, Ref<InputEvent> event) override;
    void beginPaint() override;
    void endPaint() override;
    Ref<ImageTexture> getToolCurrentImageTexture(Ref<ZoneResource> zone) override;

public:
    SetHeightTool();
    ~SetHeightTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    float getSetHeightValue();
    void updateSetHeightValue(float value);
};
#endif