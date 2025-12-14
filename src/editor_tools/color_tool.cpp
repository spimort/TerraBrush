#include "color_tool.h"
#include "../misc/zone_utils.h"

using namespace godot;

void ColorTool::_bind_methods() {}

ColorTool::ColorTool() {}

ColorTool::~ColorTool() {}

Ref<Image> ColorTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    if (zone->get_colorImage().is_null()) {
        zone->set_colorImage(ZoneUtils::createColorImage(_terraBrush->get_zonesSize(), zone->get_zonePosition(), _terraBrush->get_dataPath()));
    }

    return zone->get_colorImage();
}

void ColorTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        if (pixelBrushStrength > 0.0) {
            Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
            Color newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_COLORADD ? _selectedColor : Color(0, 0, 0, 0);

            Color newValue = Color(
                Math::lerp(currentPixel.r, newColor.r, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.g, newColor.g, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.b, newColor.b, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.a, newColor.a, pixelBrushStrength * brushStrength)
            );

            imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        }
    }), true);

    _terraBrush->get_terrainZones()->updateColorTextures(_terraBrush->get_zonesSize());
}

void ColorTool::updateSelectedColor(Color value) {
    _selectedColor = value;
}
