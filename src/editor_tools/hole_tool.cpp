#include "hole_tool.h"

using namespace godot;

void HoleTool::_bind_methods() {}

HoleTool::HoleTool() {}

HoleTool::~HoleTool() {}

bool HoleTool::getApplyResolution() const {
    return true;
}

void HoleTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void HoleTool::endPaint() {
    ToolBase::endPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

    _terraBrush->clearObjects();
    _terraBrush->createObjects();
}

Ref<ImageTexture> HoleTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_heightMapTexture();
}

void HoleTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        float holeValue = toolType == TerrainToolType::TERRAINTOOLTYPE_HOLEADD ? 1 : 0;

        Color newValue = Color(
            currentPixel.r,
            Math::max(0.0f, Math::min(1.0f, Math::lerp(currentPixel.g, holeValue, (float) Math::ceil(pixelBrushStrength) * (brushStrength * 100.0f)))),
            currentPixel.b,
            currentPixel.a
        );

        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));

    _terraBrush->get_terrainZones()->updateHeightmaps();
}