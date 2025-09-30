#include "water_flow_tool.h"

using namespace godot;

void WaterFlowTool::_bind_methods() {}

WaterFlowTool::WaterFlowTool() {}

WaterFlowTool::~WaterFlowTool() {}

bool WaterFlowTool::getApplyResolution() {
    return true;
};

Ref<ImageTexture> WaterFlowTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_waterTexture();
};

void WaterFlowTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_terraBrush->get_waterDefinition().is_null()) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        if (currentPixel.r > 0.0) {
            Vector2 direction = _previousWaterMousePosition.direction_to(imagePosition) * -1;

            if (direction == Vector2(0, 0)) {
                direction = _previousWaterMouseDirection;
            } else {
                direction = Vector2(
                    Math::lerp(_previousWaterMouseDirection.x, direction.x, 0.1f),
                    Math::lerp(_previousWaterMouseDirection.y, direction.y, 0.1f)
                );
                _previousWaterMouseDirection = direction;
            }

            Color newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_WATERFLOWADD ? Color(currentPixel.r, Math::remap(direction.x, -1, 1, 0, 1), Math::remap(direction.y, -1, 1, 0, 1), 1) : Color(currentPixel.r, 0.5f, 0.5f, 1);
            Color newValue = Color(
                Math::lerp(currentPixel.r, newColor.r, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.g, newColor.g, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.b, newColor.b, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.a, newColor.a, pixelBrushStrength * brushStrength)
            );
            imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        }
    }));

    _terraBrush->get_terrainZones()->updateWaterTextures();
    _previousWaterMousePosition = imagePosition;
};