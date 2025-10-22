#include "water_tool.h"

using namespace godot;

void WaterTool::_bind_methods() {}

WaterTool::WaterTool() {}

WaterTool::~WaterTool() {}

bool WaterTool::getApplyResolution() const {
    return true;
}

void WaterTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void WaterTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

Ref<Image> WaterTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_waterTexture();
}

void WaterTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    // If the water definition is null, or it's texture we exit here
    if (_terraBrush->get_waterDefinition().is_null() || _terraBrush->get_terrainZones()->get_zones().size() == 0 || Ref<ZoneResource>(_terraBrush->get_terrainZones()->get_zones()[0])->get_waterTexture().is_null()) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        Color newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_WATERADD ? Color(1, currentPixel.g, currentPixel.b, 1) : Color(0, 0.5f, 0.5f, 1);
        Color newValue = Color(
            Math::lerp(currentPixel.r, newColor.r, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.g, newColor.g, pixelBrushStrength * brushStrength),
            Math::lerp(currentPixel.b, newColor.b, pixelBrushStrength * brushStrength),
            1
        );
        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);

        _sculptedZones.insert(imageZoneInfo.zone);
    }));

    _terraBrush->get_terrainZones()->updateWaterTextures();
}
