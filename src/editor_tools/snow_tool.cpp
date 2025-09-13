#include "snow_tool.h"

using namespace godot;

void SnowTool::_bind_methods() {}

SnowTool::SnowTool() {}

SnowTool::~SnowTool() {}

bool SnowTool::getApplyResolution() {
    return true;
}

Ref<ImageTexture> SnowTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_snowTexture();
}

void SnowTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_terraBrush->get_snowDefinition().is_null()) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        Color newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_SNOWADD ? Color(1, 0, 0, 1) : Color(0, 0, 0, 0);

        Color newValue = Color(
            Math::lerp(currentPixel.r, newColor.r, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.g, newColor.g, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.b, newColor.b, pixelBrushStrength * brushStrength),
            1
        );
        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
    }));

    _terraBrush->get_terrainZones()->updateSnowTextures();
}
