#include "sculpt_tool.h"
#include "../misc/setting_contants.h"

#include <functional>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void SculptTool::_bind_methods() {}

SculptTool::SculptTool() {}

SculptTool::~SculptTool() {}

bool SculptTool::getApplyResolution() {
    return true;
}

void SculptTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SculptingMultiplier());
    _sculptedZones = HashSet<Ref<ZoneResource>>();
}

void SculptTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones.clear();
    _sculptedZones = HashSet<Ref<ZoneResource>>();
}

Ref<ImageTexture> SculptTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_heightMapTexture();
}

void SculptTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    ToolBase::paint(toolType, brushImage, brushSize, brushStrength, imagePosition);

    switch (toolType) {
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH:
            smooth(brushImage, brushSize, brushStrength, imagePosition);
            break;
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN:
            flatten(brushImage, brushSize, brushStrength, imagePosition);
            break;
        default:
            sculpt(toolType, brushImage, brushSize, brushStrength, imagePosition);
            smooth(brushImage, brushSize, brushStrength, imagePosition);
            break;
    }

    _terraBrush->get_terrainZones()->updateHeightmaps();
}

void SculptTool::sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](Ref<ImageZoneInfo> imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo->get_image()->get_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y);
        float newValue = pixelBrushStrength * brushStrength * _sculptingMultiplier;
        if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD) {
            newValue = currentPixel.r + newValue;
        } else if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE) {
            newValue = currentPixel.r - newValue;
        }

        Color newPixel = Color(newValue, currentPixel.g, currentPixel.b, currentPixel.a);
        imageZoneInfo->get_image()->set_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y, newPixel);
        _sculptedZones.insert(imageZoneInfo->get_zone());
    }));
}

void SculptTool::flatten(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    Color smoothValue = Color::named("TRANSPARENT");
    int numberOfSamples = 0;

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](Ref<ImageZoneInfo> imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo->get_image()->get_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y);

        smoothValue += currentPixel;
        numberOfSamples += 1;
    }));

    smoothValue = smoothValue / numberOfSamples;

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](Ref<ImageZoneInfo> imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo->get_image()->get_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y);
        Color newValue = Color(
            Math::lerp(currentPixel.r, smoothValue.r, pixelBrushStrength * brushStrength),
            currentPixel.g,
            currentPixel.b,
            currentPixel.a
        );

        imageZoneInfo->get_image()->set_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y, newValue);
        _sculptedZones.insert(imageZoneInfo->get_zone());
    }));
}

void SculptTool::smooth(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](Ref<ImageZoneInfo> imageZoneInfo, float pixelBrushStrength) {
        TypedArray<float> directions = TypedArray<float>();

        Ref<ImageZoneInfo> neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo->get_zoneInfo(), -1, 0, true);
        if (!neighbourImageZoneInfo.is_null()) {
            directions.append(neighbourImageZoneInfo->get_image()->get_pixel(neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().x, neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo->get_zoneInfo(), 1, 0, true);
        if (!neighbourImageZoneInfo.is_null()) {
            directions.append(neighbourImageZoneInfo->get_image()->get_pixel(neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().x, neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo->get_zoneInfo(), 0, -1, true);
        if (!neighbourImageZoneInfo.is_null()) {
            directions.append(neighbourImageZoneInfo->get_image()->get_pixel(neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().x, neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo->get_zoneInfo(), 0, 1, true);
        if (!neighbourImageZoneInfo.is_null()) {
            directions.append(neighbourImageZoneInfo->get_image()->get_pixel(neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().x, neighbourImageZoneInfo->get_zoneInfo()->get_imagePosition().y).r);
        }

        Color currentPixel = imageZoneInfo->get_image()->get_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y);
        directions.append(currentPixel.r);

        float average = 0;
        for (float directionValue : directions) {
            average += directionValue;
        }
        average /= directions.size();

        float resultValue = Math::lerp(currentPixel.r, average, pixelBrushStrength * brushStrength);

        Color newPixel = Color(resultValue, currentPixel.g, currentPixel.b, currentPixel.a);
        imageZoneInfo->get_image()->set_pixel(imageZoneInfo->get_zoneInfo()->get_imagePosition().x, imageZoneInfo->get_zoneInfo()->get_imagePosition().y, newPixel);
        _sculptedZones.insert(imageZoneInfo->get_zone());
    }));
}