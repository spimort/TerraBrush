#include "sculpt_tool.h"
#include "../misc/setting_contants.h"
#include "editor_resources/zone_resource.h"
#include "editor_tools/tool_base.h"
#include "misc/zone_info.h"

#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void SculptTool::_bind_methods() {}

SculptTool::SculptTool() {}

SculptTool::~SculptTool() {}

bool SculptTool::getApplyResolution() const {
    return true;
}

void SculptTool::init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones, int maxBrushSize) {
    ToolBase::init(terraBrush, undoRedo, autoAddZones, maxBrushSize);

    print_line("init compute shader");

    if (_computeShaderExecuter.is_null()) {
        _computeShaderExecuter = memnew(ComputeShaderExecuter);
        _computeShaderExecuter->init();

        _sculptShaderInstance = _computeShaderExecuter->addInstance("res://addons/terrabrush/Resources/ComputeShader/sculpt_compute_shader.glsl");
        _sculptShaderInstance->addReadWriteImageUniform(0, _maxBrushSize, _maxBrushSize, Image::Format::FORMAT_RGF);
        _sculptShaderInstance->addReadOnlyImageUniform(1, _maxBrushSize, _maxBrushSize, Image::Format::FORMAT_RGBA8);
        _sculptShaderInstance->addStructUniform<SculptShaderSettings>(2);
        _sculptShaderInstance->createUniformSet();

        _smoothShaderInstance = _computeShaderExecuter->addInstance("res://addons/terrabrush/Resources/ComputeShader/smooth_compute_shader.glsl");
        _smoothShaderInstance->addReadWriteImageUniform(0, _maxBrushSize, _maxBrushSize, Image::Format::FORMAT_RGF);
        _smoothShaderInstance->addReadOnlyImageUniform(1, _maxBrushSize, _maxBrushSize, Image::Format::FORMAT_RGBA8);
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
    SculptShaderSettings sculptParams;
    sculptParams.brushStrength = brushStrength * _sculptingMultiplier;
    sculptParams.add = toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD ? 1 : 0;
    _sculptShaderInstance->updateStructUniform(2, sculptParams);

    TypedArray<Ref<ZoneResource>> paintedZones = paintComputeShaderWithBrush(_sculptShaderInstance, 0, Image::Format::FORMAT_RGF, 1, brushImage, brushSize, slopeValue, imagePosition);

    for (Ref<ZoneResource> zone : paintedZones) {
        _sculptedZones.insert(zone);
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
    SmoothShaderSettings smoothParams;
    smoothParams.brushStrength = brushStrength;
    if (applyMultiplier) {
        smoothParams.brushStrength *= _smoothingMultiplier;
    }
    _smoothShaderInstance->updateStructUniform(2, smoothParams);

    TypedArray<Ref<ZoneResource>> paintedZones = paintComputeShaderWithBrush(_smoothShaderInstance, 0, Image::Format::FORMAT_RGF, 1, brushImage, brushSize, slopeValue, imagePosition);

    for (Ref<ZoneResource> zone : paintedZones) {
        _sculptedZones.insert(zone);
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
