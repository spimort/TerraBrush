#include "snow.h"
#include "../misc/utils.h"
#include "../misc/string_names.h"
#include "../misc/zone_utils.h"

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void Snow::_bind_methods() {

}

Snow::Snow() {
    _compressedPositions = TypedDictionary<Ref<ZoneResource>, Dictionary>();
    _imagesCache = TypedDictionary<Ref<ZoneResource>, Ref<Image>>();

    _lodLevels = 8;
    _lodRowsPerLevel = 21;
    _lodInitialCellWidth = 1;
}

Snow::~Snow() {}

void Snow::_ready() {
    _clipmap = memnew(Clipmap);
    add_child(_clipmap);

    updateSnow();
}

void Snow::_physics_process(const double delta) {
    if (_compressedPositions.size() > 0) {
        for (int imageIndex = _compressedPositions.size() - 1; imageIndex >= 0; imageIndex--) {
            Ref<ZoneResource> zone = _compressedPositions.keys()[imageIndex];
            TypedDictionary<Vector2i, float> points = _compressedPositions[zone];

            Ref<Image> compressedSnowImage = getImageForZone(zone);

            for (int i = points.size() - 1; i >= 0; i--) {
                Vector2i position = points.keys()[i];
                Color pixel = compressedSnowImage->get_pixel(position.x, position.y);
                float compressionValue = points[position];
                float offsetX = pixel.g;
                float offsetY = pixel.b;

                if (compressionValue < 1) {
                    compressionValue += (float) delta * DeCompressSpeed;

                    if (compressionValue >= 1.0) {
                        compressionValue = 1.0f;
                        points.erase(position);

                        offsetX = 0;
                        offsetY = 0;
                    } else {
                        points[position] = compressionValue;

                        offsetX -= (float) delta * DeCompressOffsetSpeed;
                        offsetY -= (float) delta * DeCompressOffsetSpeed;

                        if (offsetX < 0.0) {
                            offsetX = 0.0f;
                        }

                        if (offsetY < 0.0) {
                            offsetY = 0.0f;
                        }
                    }

                    compressedSnowImage->set_pixel(position.x, position.y, Color(pixel.r, offsetX, offsetY, compressionValue));
                }
            }

            _terrainZones->updateZoneSnowTexture(zone);

            if (points.size() == 0) {
                _compressedPositions.erase(zone);
            }
        }
    }
}

int Snow::get_zonesSize() const {
    return _zonesSize;
}
void Snow::set_zonesSize(const int value) {
    _zonesSize = value;
}

int Snow::get_resolution() const {
    return _resolution;
}
void Snow::set_resolution(const int value) {
    _resolution = value;
}

Ref<ZonesResource> Snow::get_terrainZones() const {
    return _terrainZones;
}
void Snow::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

Ref<SnowResource> Snow::get_snowDefinition() const {
    return _snowDefinition;
}
void Snow::set_snowDefinition(const Ref<SnowResource> &value) {
    _snowDefinition = value;
}

int Snow::get_lodLevels() const {
    return _lodLevels;
}
void Snow::set_lodLevels(const int value) {
    _lodLevels = value;
}

int Snow::get_lodRowsPerLevel() const {
    return _lodRowsPerLevel;
}
void Snow::set_lodRowsPerLevel(const int value) {
    _lodRowsPerLevel = value;
}

float Snow::get_lodInitialCellWidth() const {
    return _lodInitialCellWidth;
}
void Snow::set_lodInitialCellWidth(const float value) {
    _lodInitialCellWidth = value;
}

Clipmap *Snow::get_clipmap() const {
    return _clipmap;
}

void Snow::updateSnow() {
    if (_clipmap == nullptr || _snowDefinition.is_null()) {
        return;
    }

    _clipmap->get_clipmapMesh()->set_layer_mask(_snowDefinition->get_visualInstanceLayers());
    _clipmap->set_zonesSize(_zonesSize);
    _clipmap->set_resolution(_resolution);
    _clipmap->set_terrainZones(_terrainZones);
    _clipmap->set_levels(_lodLevels);
    _clipmap->set_rowsPerLevel(_lodRowsPerLevel);
    _clipmap->set_initialCellWidth(_lodInitialCellWidth);

    if (_snowDefinition->get_customShader() == nullptr) {
        Ref<ShaderMaterial> shaderMaterial = memnew(ShaderMaterial);
        shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader"));
        _clipmap->set_shader(shaderMaterial);
    } else {
        _clipmap->set_shader(Utils::createCustomShaderCopy(_snowDefinition->get_customShader()));
    }

    _clipmap->createMesh();

    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowTextures(), _terrainZones->get_snowTextures());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowFactor(), _snowDefinition->get_snowFactor());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowInnerOffset(), _snowDefinition->get_snowInnerOffset());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorTexture(), _snowDefinition->get_snowColorTexture());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorNormal(), _snowDefinition->get_snowColorNormal());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorRoughness(), _snowDefinition->get_snowColorRoughness());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorDetail(), _snowDefinition->get_snowColorDetail());
    _clipmap->get_shader()->set_shader_parameter(StringNames::Noise(), _snowDefinition->get_noise());
    _clipmap->get_shader()->set_shader_parameter(StringNames::NoiseFactor(), _snowDefinition->get_noiseFactor());
    _clipmap->get_shader()->set_shader_parameter(StringNames::Metallic(), _snowDefinition->get_metallic());
}

void Snow::addCompressedSnow(float x, float y) {
    ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(x, y, _zonesSize, _resolution);
    Ref<ZoneResource> zone = _terrainZones->getZoneForZoneInfo(zoneInfo);

    if (!zone.is_null()) {
        Ref<Image> image = getImageForZone(zone);
        Vector2i pixelPosition = Vector2i(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
        Color currentPixel = image->get_pixel(pixelPosition.x, pixelPosition.y);

        if (currentPixel.r > 0) {
            int offsetX = x - pixelPosition.x;
            int offsetY = y - pixelPosition.y;

            image->set_pixel(pixelPosition.x, pixelPosition.y, Color(currentPixel.r, offsetX, offsetY, 0));

            _terrainZones->updateZoneSnowTexture(zone);

            TypedDictionary<Vector2i, float> listOfPoints;
            if (_compressedPositions.has(zone)) {
                listOfPoints = TypedDictionary<Vector2i, float>(_compressedPositions[zone]);
            } else {
                listOfPoints = TypedDictionary<Vector2i, float>();
                _compressedPositions[zone] = listOfPoints;
            }

            if (!listOfPoints.has(pixelPosition)) {
                listOfPoints[pixelPosition] = 0;
            }
        }
    }
}

Ref<Image> Snow::getImageForZone(Ref<ZoneResource> &zone) {
    if (_imagesCache.has(zone)) {
        return _imagesCache[zone];
    } else {
        Ref<Image> image = zone->get_snowImage();
        _imagesCache[zone] = image;

        return image;
    }
}