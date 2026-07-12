#include "sculpt_tool.h"
#include "../misc/setting_contants.h"
#include "editor_resources/zone_resource.h"

#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void SculptTool::_bind_methods() {}

SculptTool::SculptTool() {}

SculptTool::~SculptTool() {}

bool SculptTool::getApplyResolution() const {
    return true;
}

void SculptTool::init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones) {
    ToolBase::init(terraBrush, undoRedo, autoAddZones);

    print_line("init compute shader");

    if (_computeShaderExecuter.is_null()) {
        _computeShaderExecuter = memnew(ComputeShaderExecuter);
        _computeShaderExecuter->init();

        _sculptShaderInstance = _computeShaderExecuter->addInstance("res://addons/terrabrush/Resources/ComputeShader/sculpt_compute_shader.glsl");
        _sculptShaderInstance->addReadWriteImagesUniform(0, terraBrush->get_zonesSize(), terraBrush->get_zonesSize(), Image::Format::FORMAT_RGF);
        _sculptShaderInstance->addReadWriteImageUniform(1, terraBrush->get_zonesSize(), terraBrush->get_zonesSize(), Image::Format::FORMAT_RGBA8);
        _sculptShaderInstance->addStructUniform<SculptShaderSettings>(2);
        _sculptShaderInstance->createUniformSet();

        _smoothShaderInstance = _computeShaderExecuter->addInstance("res://addons/terrabrush/Resources/ComputeShader/smooth_compute_shader.glsl");
        _smoothShaderInstance->addReadWriteImagesUniform(0, terraBrush->get_zonesSize(), terraBrush->get_zonesSize(), Image::Format::FORMAT_RGF);
        _smoothShaderInstance->addReadWriteImageUniform(1, terraBrush->get_zonesSize(), terraBrush->get_zonesSize(), Image::Format::FORMAT_RGBA8);
        _smoothShaderInstance->addStructUniform<SmoothShaderSettings>(2);
        _smoothShaderInstance->createUniformSet();
    }
}

void SculptTool::beforeDeselect() {
    print_line("destroy compute shader");

    if (!_computeShaderExecuter.is_null()) {
        _computeShaderExecuter->freeShaderObjects();
        _computeShaderExecuter = nullptr;
    }
}

void SculptTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SculptingMultiplier(), SettingContants::SculptingMultiplierDefaultValue());
    _smoothingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SmoothingMultiplier(), SettingContants::SmoothingMultiplierDefaultValue());
    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void SculptTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones.clear();
    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

Ref<Image> SculptTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    return zone->get_heightMapImage();
}

void SculptTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    ToolBase::paint(toolType, brushImage, brushSize, brushStrength, slopeValue, imagePosition);

    switch (toolType) {
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH:
            smooth(brushImage, brushSize, brushStrength, slopeValue, imagePosition);
            break;
        case TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN:
            flatten(brushImage, brushSize, brushStrength, slopeValue, imagePosition);
            break;
        default:
            sculpt(toolType, brushImage, brushSize, brushStrength, slopeValue, imagePosition);
            smooth(brushImage, brushSize, 1.0, slopeValue, imagePosition, false);
            break;
    }

    _terraBrush->get_terrainZones()->updateHeightmaps();
}

void SculptTool::sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    TypedArray<Ref<ZoneResource>> zones = _terraBrush->get_terrainZones()->get_zones();

    TypedArray<Ref<Image>> heightmaps = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : zones) {
        heightmaps.append(zone->get_heightMapImage());
    }

    _sculptShaderInstance->updateImagesUniform(0, heightmaps);

    int startingX = imagePosition.x - (brushSize / 2.0);
    int startingY = imagePosition.y - (brushSize / 2.0);
    Ref<Image> fullBrushImage = Image::create_empty(_terraBrush->get_zonesSize(), _terraBrush->get_zonesSize(), false, Image::FORMAT_RGBA8);
    fullBrushImage->fill(Color(0, 0, 0, 0));
    fullBrushImage->blend_rect(brushImage, Rect2(0, 0, brushSize, brushSize), Vector2i(startingX, startingY));
    _sculptShaderInstance->updateImageUniform(1, fullBrushImage);

    SculptShaderSettings sculptParams;
    sculptParams.numberOfHeightmaps = heightmaps.size();
    sculptParams.brushStrength = brushStrength * _sculptingMultiplier;
    sculptParams.add = toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD ? 1 : 0;
    _sculptShaderInstance->updateStructUniform(2, sculptParams);

    TypedArray<PackedByteArray> results = _sculptShaderInstance->getImagesResult(0, heightmaps.size());
    for (int i = 0; i < results.size(); i++) {
        Ref<Image> heightmapImage = ((Ref<ZoneResource>)zones[i])->get_heightMapImage();
        heightmapImage->set_data(heightmapImage->get_width(), heightmapImage->get_height(), heightmapImage->has_mipmaps(), heightmapImage->get_format(), results[i]);
    }

    // forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
    //     Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
    //     float newValue = pixelBrushStrength * brushStrength * _sculptingMultiplier;

    //     if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD) {
    //         newValue = currentPixel.r + newValue;
    //     } else if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE) {
    //         newValue = currentPixel.r - newValue;
    //     }

    //     Color newPixel = Color(newValue, currentPixel.g, currentPixel.b, currentPixel.a);
    //     imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newPixel);
    //     _sculptedZones.insert(imageZoneInfo.zone);
    // }));
}

void SculptTool::flatten(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    Color smoothValue = Color(1, 1, 1, 0); // Transparent color
    int numberOfSamples = 0;

    forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        smoothValue += currentPixel;
        numberOfSamples += 1;
    }));

    smoothValue = smoothValue / numberOfSamples;

    forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        Color newValue = Color(
            Math::lerp(currentPixel.r, smoothValue.r, pixelBrushStrength * brushStrength),
            currentPixel.g,
            currentPixel.b,
            currentPixel.a
        );

        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));
}

void SculptTool::smooth(Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition, bool applyMultiplier) {
    TypedArray<Ref<ZoneResource>> zones = _terraBrush->get_terrainZones()->get_zones();

    TypedArray<Ref<Image>> heightmaps = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : zones) {
        heightmaps.append(zone->get_heightMapImage());
    }
    _smoothShaderInstance->updateImagesUniform(0, heightmaps);

    int startingX = imagePosition.x - (brushSize / 2.0);
    int startingY = imagePosition.y - (brushSize / 2.0);
    Ref<Image> fullBrushImage = Image::create_empty(_terraBrush->get_zonesSize(), _terraBrush->get_zonesSize(), false, Image::FORMAT_RGBA8);
    fullBrushImage->fill(Color(0, 0, 0, 0));
    fullBrushImage->blend_rect(brushImage, Rect2(0, 0, brushSize, brushSize), Vector2i(startingX, startingY));
    _smoothShaderInstance->updateImageUniform(1, fullBrushImage);

    SmoothShaderSettings smoothParams;
    smoothParams.numberOfHeightmaps = heightmaps.size();
    smoothParams.brushStrength = brushStrength;
    _smoothShaderInstance->updateStructUniform(2, smoothParams);

    TypedArray<PackedByteArray> results = _sculptShaderInstance->getImagesResult(0, heightmaps.size());
    for (int i = 0; i < results.size(); i++) {
        Ref<Image> heightmapImage = ((Ref<ZoneResource>)zones[i])->get_heightMapImage();
        heightmapImage->set_data(heightmapImage->get_width(), heightmapImage->get_height(), heightmapImage->has_mipmaps(), heightmapImage->get_format(), results[i]);
    }
    // forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
    //     std::vector<float> directions = std::vector<float>();

    //     ImageZoneInfo neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, -1, 0, true);
    //     if (!neighbourImageZoneInfo.zone.is_null()) {
    //         directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
    //     }

    //     neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 1, 0, true);
    //     if (!neighbourImageZoneInfo.zone.is_null()) {
    //         directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
    //     }

    //     neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 0, -1, true);
    //     if (!neighbourImageZoneInfo.zone.is_null()) {
    //         directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
    //     }

    //     neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 0, 1, true);
    //     if (!neighbourImageZoneInfo.zone.is_null()) {
    //         directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
    //     }

    //     Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
    //     directions.push_back(currentPixel.r);

    //     float average = 0;
    //     for (float directionValue : directions) {
    //         average += directionValue;
    //     }
    //     average /= directions.size();

    //     float multiplier = 1.0;
    //     if (applyMultiplier) {
    //         multiplier = _smoothingMultiplier;
    //     }

    //     float resultValue = Math::lerp(currentPixel.r, average, pixelBrushStrength * brushStrength * multiplier);

    //     Color newPixel = Color(resultValue, currentPixel.g, currentPixel.b, currentPixel.a);
    //     imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newPixel);
    //     _sculptedZones.insert(imageZoneInfo.zone);
    // }));
}
