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

            Ref<Image> compressedSnowImage = zone->get_snowImage();

            for (int i = points.size() - 1; i >= 0; i--) {
                Vector2i position = points.keys()[i];
                Color pixel = compressedSnowImage->get_pixel(position.x, position.y);
                float compressionValue = points[position];
                float offsetX = pixel.g;
                float offsetY = pixel.b;

                if (compressionValue < 1) {
                    compressionValue += (float) delta * _snowDefinition->get_decompressSpeed();

                    if (compressionValue >= 1.0) {
                        compressionValue = 1.0f;
                        points.erase(position);

                        offsetX = 0;
                        offsetY = 0;
                    } else {
                        points[position] = compressionValue;

                        offsetX -= (float) delta * _snowDefinition->get_decompressOffsetSpeed();
                        offsetY -= (float) delta * _snowDefinition->get_decompressOffsetSpeed();

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

Ref<TextureSetsResource> Snow::get_textureSets() const {
    return _textureSets;
}
void Snow::set_textureSets(const Ref<TextureSetsResource> &value) {
    _textureSets = value;
}

bool Snow::get_chunkMesh() const {
    return _chunkMesh;
}
void Snow::set_chunkMesh(const bool value) {
    _chunkMesh = value;
}

int Snow::get_chunkAABBHeight() const {
    return _chunkAABBHeight;
}
void Snow::set_chunkAABBHeight(const int value) {
    _chunkAABBHeight = value;
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
    if (_clipmap == nullptr || _snowDefinition.is_null() || _snowDefinition->get_textureSetIndex() >= 0 && (_textureSets.is_null() || _textureSets->get_textureSets().size() < _snowDefinition->get_textureSetIndex() + 1)) {
        return;
    }

    _clipmap->set_visualInstanceLayers(_snowDefinition->get_visualInstanceLayers());
    _clipmap->set_zonesSize(_zonesSize);
    _clipmap->set_resolution(_resolution);
    _clipmap->set_terrainZones(_terrainZones);
    _clipmap->set_chunkMesh(_chunkMesh);
    _clipmap->set_chunkAABBHeight(_chunkAABBHeight);
    _clipmap->set_levels(_lodLevels);
    _clipmap->set_rowsPerLevel(_lodRowsPerLevel);
    _clipmap->set_initialCellWidth(_lodInitialCellWidth);

    if (_snowDefinition->get_customShader() == nullptr) {
        Ref<ShaderMaterial> shaderMaterial = memnew(ShaderMaterial);
        shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader.gdshader"));
        _clipmap->set_shader(shaderMaterial);
    } else {
        _clipmap->set_shader(Utils::createCustomShaderCopy(_snowDefinition->get_customShader(), TypedArray<StringName>::make(StringNames::SnowTextures())));
    }

    _clipmap->createMesh();

    Ref<TextureSetResource> textureSet = nullptr;
    if (_snowDefinition->get_textureSetIndex() >= 0){
        textureSet = _textureSets->get_textureSets()[_snowDefinition->get_textureSetIndex()];
    }

    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowTextures(), _terrainZones->get_snowTextures());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowFactor(), _snowDefinition->get_snowFactor());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowInnerOffset(), _snowDefinition->get_snowInnerOffset());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorTexture(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_snowColorTexture() : textureSet->get_albedoTexture());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorNormal(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_snowColorNormal() : textureSet->get_normalTexture());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorRoughness(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_snowColorRoughness() : textureSet->get_roughnessTexture());
    _clipmap->get_shader()->set_shader_parameter(StringNames::SnowColorDetail(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_snowColorDetail() : (textureSet->get_textureDetail() < 0 ? _snowDefinition->get_snowColorDetail() : textureSet->get_textureDetail()));
    _clipmap->get_shader()->set_shader_parameter(StringNames::Metallic(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_metallic() : textureSet->get_metallic());
    _clipmap->get_shader()->set_shader_parameter(StringNames::Specular(), _snowDefinition->get_textureSetIndex() < 0 ? _snowDefinition->get_specular() : textureSet->get_specular());
    _clipmap->get_shader()->set_shader_parameter(StringNames::Noise(), _snowDefinition->get_noise());
    _clipmap->get_shader()->set_shader_parameter(StringNames::NoiseFactor(), _snowDefinition->get_noiseFactor());
}

void Snow::addCompressedSnow(float x, float y) {
    ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(x, y, _zonesSize, _resolution);
    Ref<ZoneResource> zone = _terrainZones->getZoneForZoneInfo(zoneInfo);

    if (!zone.is_null()) {
        Ref<Image> image = zone->get_snowImage();
        Vector2i pixelPosition = Vector2i(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
        Color currentPixel = image->get_pixel(pixelPosition.x, pixelPosition.y);

        if (currentPixel.r > 0) {
            float offsetX = x - pixelPosition.x;
            float offsetY = y - pixelPosition.y;

            float compressionValue = 1.0 - _snowDefinition->get_compressionFactor();

            image->set_pixel(pixelPosition.x, pixelPosition.y, Color(currentPixel.r, offsetX, offsetY, compressionValue));

            _terrainZones->updateZoneSnowTexture(zone);

            TypedDictionary<Vector2i, float> listOfPoints;
            if (_compressedPositions.has(zone)) {
                listOfPoints = TypedDictionary<Vector2i, float>(_compressedPositions[zone]);
            } else {
                listOfPoints = TypedDictionary<Vector2i, float>();
                _compressedPositions[zone] = listOfPoints;
            }

            if (!listOfPoints.has(pixelPosition)) {
                listOfPoints[pixelPosition] = compressionValue;
            }
        }
    }
}
