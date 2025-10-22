#include "water.h"
#include "../misc/utils.h"
#include "../misc/string_names.h"
#include "../misc/zone_utils.h"
#include "../misc/zone_info.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Water::_bind_methods() {

}

Water::Water() {
    _ripplePositions = TypedDictionary<Ref<ZoneResource>, Dictionary>();
    _imagesCache = TypedDictionary<Ref<ZoneResource>, Ref<Image>>();

    _timeScale = 0.1;
    _strength = 0.4;
    _visualInstanceLayers = 1;
    _lodLevels = 8;
    _lodRowsPerLevel = 21;
    _lodInitialCellWidth = 1;
}

Water::~Water() {}

void Water::_ready() {
    _clipmap = memnew(Clipmap);
    add_child(_clipmap);

    updateWater();
}

void Water::_physics_process(double delta) {
    if (_ripplePositions.size() > 0) {
        for (int imageIndex = _ripplePositions.size() - 1; imageIndex >= 0; imageIndex--) {
            Ref<ZoneResource> zone = _ripplePositions.keys()[imageIndex];
            TypedDictionary<Vector2i, float> points = _ripplePositions[zone];

            Ref<Image> rippleWaterImage = getImageForZone(zone);

            for (int i = points.size() - 1; i >= 0; i--) {
                Vector2i position = points.keys()[i];
                Color pixel = rippleWaterImage->get_pixel(position.x, position.y);
                float rippleValue = points[position];

                if (rippleValue < 1.0) {
                    rippleValue += (float) delta * RippleResetSpeed;

                    if (rippleValue >= 1.0) {
                        rippleValue = 1.0f;
                        points.erase(position);
                    } else {
                        points[position] = rippleValue;
                    }

                    rippleWaterImage->set_pixel(position.x, position.y, Color(pixel.r, pixel.g, pixel.b, rippleValue));
                }
            }

            _terrainZones->updateZoneWaterTexture(zone);

            if (points.size() == 0) {
                _ripplePositions.erase(zone);
            }
        }
    }
}

void Water::set_zonesSize(const int value) {
    _zonesSize = value;
}

void Water::set_resolution(const int value) {
    _resolution = value;
}

void Water::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void Water::set_waterFactor(const float value) {
    _waterFactor = value;
}

void Water::set_heightMapFactor(const float value) {
    _heightMapFactor = value;
}

void Water::set_waterInnerOffset(const float value) {
    _waterInnerOffset = value;
}

void Water::set_waterColor(const Color value) {
    _waterColor = value;
}

void Water::set_fresnelColor(const Color value) {
    _fresnelColor = value;
}

void Water::set_metallic(const float value) {
    _metallic = value;
}

void Water::set_roughness(const float value) {
    _roughness = value;
}

void Water::set_normalMap(const Ref<Texture2D> &value) {
    _normalMap = value;
}

void Water::set_normalMap2(const Ref<Texture2D> &value) {
    _normalMap2 = value;
}

void Water::set_timeScale(const float value) {
    _timeScale = value;
}

void Water::set_strength(const float value) {
    _strength = value;
}

void Water::set_wave(const Ref<Texture2D> &value) {
    _wave = value;
}

void Water::set_noiseScale(const float value) {
    _noiseScale = value;
}

void Water::set_heightScale(const float value) {
    _heightScale = value;
}

void Water::set_colorDeep(const Color value) {
    _colorDeep = value;
}

void Water::set_colorShallow(const Color value) {
    _colorShallow = value;
}

void Water::set_beersLaw(const float value) {
    _beersLaw = value;
}

void Water::set_depthOffset(const float value) {
    _depthOffset = value;
}

void Water::set_edgeScale(const float value) {
    _edgeScale = value;
}

void Water::set_near(const float value) {
    _near = value;
}

void Water::set_far(const float value) {
    _far = value;
}

void Water::set_edgeColor(const Color value) {
    _edgeColor = value;
}

void Water::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

void Water::set_lodLevels(const int value) {
    _lodLevels = value;
}

void Water::set_lodRowsPerLevel(const int value) {
    _lodRowsPerLevel = value;
}

void Water::set_lodInitialCellWidth(const float value) {
    _lodInitialCellWidth = value;
}

void Water::set_customShader(const Ref<ShaderMaterial> &value) {

    _customShader = value;
}

Clipmap *Water::get_clipmap() const {
    return _clipmap;
}

void Water::updateWater() {
    if (_clipmap == nullptr) {
        return;
    }

    _clipmap->get_clipmapMesh()->set_layer_mask(_visualInstanceLayers);
    _clipmap->set_zonesSize(_zonesSize);
    _clipmap->set_resolution(_resolution);
    _clipmap->set_terrainZones(_terrainZones);
    _clipmap->set_levels(_lodLevels);
    _clipmap->set_rowsPerLevel(_lodRowsPerLevel);
    _clipmap->set_initialCellWidth(_lodInitialCellWidth);

    if (_customShader.is_null()) {
        Ref<ShaderMaterial> shaderMaterial = memnew(ShaderMaterial);
        shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/water_clipmap_shader.gdshader"));
        _clipmap->set_shader(shaderMaterial);
    } else {
        _clipmap->get_shader() = Utils::createCustomShaderCopy(_customShader);
    }

    _clipmap->createMesh();

    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterInnerOffset(), _waterInnerOffset);
    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterTextures(), _terrainZones->get_waterTextures());
    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterFactor(), _waterFactor);
    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterColor(), _waterColor);
    _clipmap->get_shader()->set_shader_parameter(StringNames::FresnelColor(), _fresnelColor);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Metallic(), _metallic);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Roughness(), _roughness);
    _clipmap->get_shader()->set_shader_parameter(StringNames::NormalMap(), _normalMap);
    _clipmap->get_shader()->set_shader_parameter(StringNames::NormalMap2(), _normalMap2);
    _clipmap->get_shader()->set_shader_parameter(StringNames::TimeScale(), _timeScale);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Strength(), _strength);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Wave(), _wave);
    _clipmap->get_shader()->set_shader_parameter(StringNames::NoiseScale(), _noiseScale);
    _clipmap->get_shader()->set_shader_parameter(StringNames::HeightScale(), _heightScale);
    _clipmap->get_shader()->set_shader_parameter(StringNames::ColorDeep(), _colorDeep);
    _clipmap->get_shader()->set_shader_parameter(StringNames::ColorShallow(), _colorShallow);
    _clipmap->get_shader()->set_shader_parameter(StringNames::BeersLaw(), _beersLaw);
    _clipmap->get_shader()->set_shader_parameter(StringNames::DepthOffset(), _depthOffset);
    _clipmap->get_shader()->set_shader_parameter(StringNames::EdgeScale(), _edgeScale);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Near(), _near);
    _clipmap->get_shader()->set_shader_parameter(StringNames::Far(), _far);
    _clipmap->get_shader()->set_shader_parameter(StringNames::EdgeColor(), _edgeColor);

    // This is for compatibility with Godot 4.2
    Dictionary engineVersion = Engine::get_singleton()->get_version_info();
    int major = (int) engineVersion["major"];
    int minor = (int) engineVersion["minor"];
    if (major == 4 && minor < 3) {
        _clipmap->get_shader()->set_shader_parameter(StringNames::InvertedZ(), false);
    }
}

void Water::addRippleEffect(float x, float y) {
    ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(x, y, _zonesSize, _resolution);
    Ref<ZoneResource> zone = _terrainZones->getZoneForZoneInfo(zoneInfo);

    if (!zone.is_null()) {
        Ref<Image> image = getImageForZone(zone);
        Vector2i pixelPosition = Vector2i(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
        Color currentPixel = image->get_pixel(pixelPosition.x, pixelPosition.y);

        if (currentPixel.r > 0) {
            image->set_pixel(pixelPosition.x, pixelPosition.y, Color(currentPixel.r, currentPixel.g, currentPixel.b, 0));

            _terrainZones->updateZoneWaterTexture(zone);

            TypedDictionary<Vector2i, float> listOfPoints;
            if (_ripplePositions.has(zone)) {
                listOfPoints = TypedDictionary<Vector2i, float>(_ripplePositions[zone]);
            } else {
                listOfPoints = TypedDictionary<Vector2i, float>();
                _ripplePositions[zone] = listOfPoints;
            }

            if (!listOfPoints.has(pixelPosition)) {
                listOfPoints[pixelPosition] = 0;
            }
        }
    }
}

Ref<Image> Water::getImageForZone(Ref<ZoneResource> &zone) {
    if (_imagesCache.has(zone)) {
        return _imagesCache[zone];
    } else {
        Ref<Image> image = zone->get_waterTexture();
        _imagesCache[zone] = image;

        return image;
    }
}