#ifndef WATER_H
#define WATER_H

#include "clipmap.h"
#include "../editor_resources/zone_resource.h"
#include "../editor_resources/zones_resource.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Water : public Node3D {
    GDCLASS(Water, Node3D)

private:
    const float RippleResetSpeed = 0.9f;

    TypedDictionary<Ref<ZoneResource>, Dictionary> _ripplePositions;
    TypedDictionary<Ref<ZoneResource>, Ref<Image>> _imagesCache;

    Clipmap *_clipmap;

    int _zonesSize;
    int _resolution;
    Ref<ZonesResource> _terrainZones;
    float _waterFactor;
    float _heightMapFactor;
    float _waterInnerOffset;
    Color _waterColor;
    Color _fresnelColor;
    float _metallic;
    float _roughness;
    Ref<Texture2D> _normalMap;
    Ref<Texture2D> _normalMap2;
    float _timeScale;
    float _strength;
    Ref<Texture2D> _wave;
    float _noiseScale;
    float _heightScale;
    Color _colorDeep;
    Color _colorShallow;
    float _beersLaw;
    float _depthOffset;
    float _edgeScale;
    float _near;
    float _far;
    Color _edgeColor;
    int _visualInstanceLayers;
    int _lodLevels;
    int _lodRowsPerLevel;
    float _lodInitialCellWidth;
    Ref<ShaderMaterial> _customShader;

    Ref<Image> getImageForZone(Ref<ZoneResource> &zone);

protected:
    static void _bind_methods();

public:
    Water();
    ~Water();

    void _ready() override;
    void _physics_process(double delta) override;

    void set_zonesSize(const int value);
    void set_resolution(const int value);
    void set_terrainZones(const Ref<ZonesResource> &value);
    void set_waterFactor(const float value);
    void set_heightMapFactor(const float value);
    void set_waterInnerOffset(const float value);
    void set_waterColor(const Color value);
    void set_fresnelColor(const Color value);
    void set_metallic(const float value);
    void set_roughness(const float value);
    void set_normalMap(const Ref<Texture2D> &value);
    void set_normalMap2(const Ref<Texture2D> &value);
    void set_timeScale(const float value);
    void set_strength(const float value);
    void set_wave(const Ref<Texture2D> &value);
    void set_noiseScale(const float value);
    void set_heightScale(const float value);
    void set_colorDeep(const Color value);
    void set_colorShallow(const Color value);
    void set_beersLaw(const float value);
    void set_depthOffset(const float value);
    void set_edgeScale(const float value);
    void set_near(const float value);
    void set_far(const float value);
    void set_edgeColor(const Color value);
    void set_visualInstanceLayers(const int value);
    void set_lodLevels(const int value);
    void set_lodRowsPerLevel(const int value);
    void set_lodInitialCellWidth(const float value);
    void set_customShader(const Ref<ShaderMaterial> &value);

    void updateWater();
    void addRippleEffect(float x, float y);
};
#endif