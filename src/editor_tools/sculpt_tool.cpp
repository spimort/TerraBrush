#include "sculpt_tool.h"
#include "../misc/setting_contants.h"
#include "editor_resources/zone_resource.h"

#include <functional>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

using namespace godot;

void SculptTool::_bind_methods() {}

SculptTool::SculptTool() {}

SculptTool::~SculptTool() {}

bool SculptTool::getApplyResolution() const {
    return true;
}

void SculptTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SculptingMultiplier(), SettingContants::SculptingMultiplierDefaultValue());
    _smoothingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SmoothingMultiplier(), SettingContants::SmoothingMultiplierDefaultValue());
    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

    _renderingDevice = RenderingServer::get_singleton()->create_local_rendering_device();

    Ref<RDShaderFile> shaderResource = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/ComputeShader/sculpt_compute_shader.glsl");
    Ref<RDShaderSPIRV> shaderSpirv = shaderResource->get_spirv();
    _computeShader = _renderingDevice->shader_create_from_spirv(shaderSpirv);

    Ref<RDTextureFormat> heightmapFormat = memnew(RDTextureFormat);
    heightmapFormat->set_format(RenderingDevice::DATA_FORMAT_R32G32_SFLOAT);
    heightmapFormat->set_width(_terraBrush->get_zonesSize());
    heightmapFormat->set_height(_terraBrush->get_zonesSize());
    heightmapFormat->set_usage_bits(RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT);

    _heightmapTextureParam = _renderingDevice->texture_create(heightmapFormat, memnew(RDTextureView));

    Ref<RDUniform> heightmapUniform = memnew(RDUniform);
    heightmapUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_IMAGE);
    heightmapUniform->set_binding(0);
    heightmapUniform->add_id(_heightmapTextureParam);

    Ref<RDTextureFormat> brushFormat = memnew(RDTextureFormat);
    brushFormat->set_format(RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM);
    brushFormat->set_width(_terraBrush->get_zonesSize());
    brushFormat->set_height(_terraBrush->get_zonesSize());
    brushFormat->set_usage_bits(RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT);

    _brushTextureParam = _renderingDevice->texture_create(brushFormat, memnew(RDTextureView));

    Ref<RDUniform> brushUniform = memnew(RDUniform);
    brushUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_IMAGE);
    brushUniform->set_binding(1);
    brushUniform->add_id(_brushTextureParam);

    _sculptSettingsBufferParam = _renderingDevice->uniform_buffer_create(sizeof(SculptSettings));

    Ref<RDUniform> paintSettingsUniform = memnew(RDUniform);
    paintSettingsUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
    paintSettingsUniform->set_binding(2);
    paintSettingsUniform->add_id(_sculptSettingsBufferParam);

    _uniformSet = _renderingDevice->uniform_set_create(TypedArray<Ref<RDUniform>>::make(heightmapUniform, brushUniform, paintSettingsUniform), _computeShader, 0);

    _pipeline = _renderingDevice->compute_pipeline_create(_computeShader);
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

    if (_renderingDevice != nullptr) {
        _renderingDevice->free_rid(_pipeline);
        _pipeline = RID();

        _renderingDevice->free_rid(_uniformSet);
        _uniformSet = RID();

        _renderingDevice->free_rid(_heightmapTextureParam);
        _heightmapTextureParam = RID();

        _renderingDevice->free_rid(_brushTextureParam);
        _brushTextureParam = RID();

        _renderingDevice->free_rid(_sculptSettingsBufferParam);
        _sculptSettingsBufferParam = RID();

        _renderingDevice->free_rid(_computeShader);
        _computeShader = RID();

        memdelete(_renderingDevice);
        _renderingDevice = nullptr;
    }
}

Ref<Image> SculptTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    return zone->get_heightMapImage();
}

void SculptTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    ToolBase::paint(toolType, brushImage, brushSize, brushStrength, slopeValue, imagePosition);

    Ref<ZoneResource> zone = ((Ref<ZoneResource>) _terraBrush->get_terrainZones()->get_zones()[0]);
    Ref<Image> heightmapImage = zone->get_heightMapImage();

    _renderingDevice->texture_update(_heightmapTextureParam, 0, heightmapImage->get_data());

    // Create a full brush image filled with black
    Ref<Image> fullBrushImage = Image::create_empty(_terraBrush->get_zonesSize(), _terraBrush->get_zonesSize(), false, Image::FORMAT_RGBA8);
    fullBrushImage->fill(Color(0, 0, 0, 0));

    // Draw the brush in to the full brush image
    int startingX = imagePosition.x - (brushSize / 2.0);
    int startingY = imagePosition.y - (brushSize / 2.0);

    fullBrushImage->blend_rect(brushImage, Rect2(0, 0, brushSize, brushSize), Vector2i(startingX, startingY));
    _renderingDevice->texture_update(_brushTextureParam, 0, fullBrushImage->get_data());

    // brushImage->resize(_terraBrush->get_zonesSize(), _terraBrush->get_zonesSize());
    //
    // _renderingDevice->texture_update(_brushTexture, 0, brushImage->get_data());

    SculptSettings params;
    params.brushStrength = brushStrength * _sculptingMultiplier;
    params.add = toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD;

    PackedByteArray bytes;
    bytes.resize(sizeof(SculptSettings));
    memcpy(bytes.ptrw(), &params, sizeof(SculptSettings));

    _renderingDevice->buffer_update(_sculptSettingsBufferParam, 0, sizeof(SculptSettings), bytes);

    int64_t computeList = _renderingDevice->compute_list_begin();
    _renderingDevice->compute_list_bind_compute_pipeline(computeList, _pipeline);
    _renderingDevice->compute_list_bind_uniform_set(computeList, _uniformSet, 0);
    _renderingDevice->compute_list_dispatch(computeList, _terraBrush->get_zonesSize() / 8, _terraBrush->get_zonesSize() / 8, 1);
    _renderingDevice->compute_list_end();

    _renderingDevice->submit();
    _renderingDevice->sync();

    PackedByteArray result = _renderingDevice->texture_get_data(_heightmapTextureParam, 0);

    heightmapImage->set_data(heightmapImage->get_width(), heightmapImage->get_height(), heightmapImage->has_mipmaps(), heightmapImage->get_format(), result);

    _sculptedZones.insert(zone);

    // switch (toolType) {
    //     case TerrainToolType::TERRAINTOOLTYPE_TERRAINSMOOTH:
    //         smooth(brushImage, brushSize, brushStrength, slopeValue, imagePosition);
    //         break;
    //     case TerrainToolType::TERRAINTOOLTYPE_TERRAINFLATTEN:
    //         flatten(brushImage, brushSize, brushStrength, slopeValue, imagePosition);
    //         break;
    //     default:
    //         sculpt(toolType, brushImage, brushSize, brushStrength, slopeValue, imagePosition);
    //         smooth(brushImage, brushSize, 1.0, slopeValue, imagePosition, false);
    //         break;
    // }

    _terraBrush->get_terrainZones()->updateHeightmaps();
}

void SculptTool::sculpt(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        float newValue = pixelBrushStrength * brushStrength * _sculptingMultiplier;

        if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINADD) {
            newValue = currentPixel.r + newValue;
        } else if (toolType == TerrainToolType::TERRAINTOOLTYPE_TERRAINREMOVE) {
            newValue = currentPixel.r - newValue;
        }

        Color newPixel = Color(newValue, currentPixel.g, currentPixel.b, currentPixel.a);
        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newPixel);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));
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
    forEachBrushPixel(brushImage, brushSize, slopeValue, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        std::vector<float> directions = std::vector<float>();

        ImageZoneInfo neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, -1, 0, true);
        if (!neighbourImageZoneInfo.zone.is_null()) {
            directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 1, 0, true);
        if (!neighbourImageZoneInfo.zone.is_null()) {
            directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 0, -1, true);
        if (!neighbourImageZoneInfo.zone.is_null()) {
            directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
        }

        neighbourImageZoneInfo = getImageZoneInfoForPosition(imageZoneInfo.zoneInfo, 0, 1, true);
        if (!neighbourImageZoneInfo.zone.is_null()) {
            directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
        }

        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        directions.push_back(currentPixel.r);

        float average = 0;
        for (float directionValue : directions) {
            average += directionValue;
        }
        average /= directions.size();

        float multiplier = 1.0;
        if (applyMultiplier) {
            multiplier = _smoothingMultiplier;
        }

        float resultValue = Math::lerp(currentPixel.r, average, pixelBrushStrength * brushStrength * multiplier);

        Color newPixel = Color(resultValue, currentPixel.g, currentPixel.b, currentPixel.a);
        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newPixel);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));
}
