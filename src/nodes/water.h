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
    GDCLASS(Water, Node3D);

private:
    const float RippleResetSpeed = 0.9f;

    TypedDictionary<Ref<ZoneResource>, Dictionary> _ripplePositions = TypedDictionary<Ref<ZoneResource>, Dictionary>();

    Clipmap *_clipmap = nullptr;

    int _zonesSize = 0;
    int _resolution = 0;
    Ref<ZonesResource> _terrainZones = nullptr;
    float _waterFactor = 0;
    float _heightMapFactor = 0;
    float _waterInnerOffset = 0;
    Color _waterColor = Color();
    Color _fresnelColor = Color();
    float _metallic = 0;
    float _roughness = 0;
    Ref<Texture2D> _normalMap = nullptr;
    Ref<Texture2D> _normalMap2 = nullptr;
    float _timeScale = 0;
    float _strength = 0;
    Ref<Texture2D> _wave = nullptr;
    float _noiseScale = 0;
    float _heightScale = 0;
    Color _colorDeep = Color();
    Color _colorShallow = Color();
    float _beersLaw = 0;
    float _depthOffset = 0;
    float _edgeScale = 0;
    float _near = 0;
    float _far = 0;
    Color _edgeColor = Color();
    int _visualInstanceLayers = 0;
    int _lodLevels = 0;
    int _lodRowsPerLevel = 0;
    float _lodInitialCellWidth;
    Ref<ShaderMaterial> _customShader = nullptr;

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

    Clipmap *get_clipmap() const;

    void updateWater();
    void addRippleEffect(float x, float y);
};
#endif