#include "meta_info_tool.h"

using namespace godot;

void MetaInfoTool::_bind_methods() {}

MetaInfoTool::MetaInfoTool() {}

MetaInfoTool::~MetaInfoTool() {}

Ref<Image> MetaInfoTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    return zone->get_metaInfoImage();
}

void MetaInfoTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_selectedMetaInfoIndex < 0) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        if (pixelBrushStrength > 0.0) {
            int layerValue = toolType == TerrainToolType::TERRAINTOOLTYPE_METAINFOADD ? _selectedMetaInfoIndex : -1;

            int newValue = layerValue;
            imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, Color(layerValue, 0, 0, 0));
        }
    }), true);

    _terraBrush->get_terrainZones()->updateMetaInfoTextures();
}

void MetaInfoTool::updateSelectedMetaInfoIndex(int value) {
    _selectedMetaInfoIndex = value;
}

