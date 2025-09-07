#include "foliage_tool.h"

using namespace godot;

void FoliageTool::_bind_methods() {}

FoliageTool::FoliageTool() {}

FoliageTool::~FoliageTool() {}

Ref<ImageTexture> FoliageTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_foliagesTexture()[_selectedFoliageIndex];
}

void FoliageTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_selectedFoliageIndex < 0) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        Color newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_FOLIAGEADD ? Color(1, 0, 0, 1) : Color(0, 0, 0, 0);

        Color newValue = Color(
            Math::lerp(currentPixel.r, newColor.r, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.g, newColor.g, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.b, newColor.b, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.a, newColor.a, pixelBrushStrength * brushStrength)
        );
        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
    }));

    _terraBrush->get_terrainZones()->updateFoliagesTextures(_selectedFoliageIndex);
}

void FoliageTool::updateSelectedFoliageIndex(int value) {
    _selectedFoliageIndex = value;
}