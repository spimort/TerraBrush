#include "lock_tool.h"
#include "../misc/zone_utils.h"

using namespace godot;

void LockTool::_bind_methods() {}

LockTool::LockTool() {}

LockTool::~LockTool() {}

Ref<Image> LockTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    if (zone->get_lockTexture().is_null()) {
        zone->set_lockTexture(ZoneUtils::createLockImage(_terraBrush->get_zonesSize(), zone->get_zonePosition()));
    }

    return zone->get_lockTexture();
}

void LockTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        if (pixelBrushStrength > 0.0) {
            Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
            float lockValue = toolType == TerrainToolType::TERRAINTOOLTYPE_LOCKADD ? 1 : 0;

            Color newValue = Color(
                Math::lerp(currentPixel.r, lockValue, pixelBrushStrength * brushStrength),
                currentPixel.g,
                currentPixel.b,
                currentPixel.a
            );

            imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        }
    }), true);

    _terraBrush->get_terrainZones()->updateLockTexture(_terraBrush->get_zonesSize());
}
