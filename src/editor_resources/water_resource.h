#ifndef WATER_RESOURCE_H
#define WATER_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class WaterResource : public Resource {
    GDCLASS(WaterResource, Resource)

private:
    float _waterFactor;
    float _waterInnerOffset;
    Color _waterColor;
    Color _waterFresnelColor;
    float _waterMetallic;
    float _waterRoughness;
    Ref<Texture2D> _waterNormalMap;
    Ref<Texture2D> _waterNormalMap2;
    float _waterTimeScale;
    float _waterStrength;
    Ref<Texture2D> _waterWave;
    float _waterNoiseScale;
    float _waterHeightScale;
    Color _waterColorDeep;
    Color _waterColorShallow;
    float _waterBeersLaw;
    float _waterDepthOffset;
    float _waterEdgeScale;
    float _waterNear;
    float _waterFar;
    Color _waterEdgeColor;
    int _visualInstanceLayers;
    Ref<ShaderMaterial> _customShader;

protected:
    static void _bind_methods();

public:
    WaterResource();
    ~WaterResource();

    float get_waterFactor() const;
    void set_waterFactor(const float value);

    float get_waterInnerOffset() const;
    void set_waterInnerOffset(const float value);

    Color get_waterColor() const;
    void set_waterColor(const Color value);

    Color get_waterFresnelColor() const;
    void set_waterFresnelColor(const Color value);

    float get_waterMetallic() const;
    void set_waterMetallic(const float value);

    float get_waterRoughness() const;
    void set_waterRoughness(const float value);

    Ref<Texture2D> get_waterNormalMap() const;
    void set_waterNormalMap(const Ref<Texture2D> &value);

    Ref<Texture2D> get_waterNormalMap2() const;
    void set_waterNormalMap2(const Ref<Texture2D> &value);

    float get_waterTimeScale() const;
    void set_waterTimeScale(const float value);

    float get_waterStrength() const;
    void set_waterStrength(const float value);

    Ref<Texture2D> get_waterWave() const;
    void set_waterWave(const Ref<Texture2D> &value);

    float get_waterNoiseScale() const;
    void set_waterNoiseScale(const float value);

    float get_waterHeightScale() const;
    void set_waterHeightScale(const float value);

    Color get_waterColorDeep() const;
    void set_waterColorDeep(const Color value);

    Color get_waterColorShallow() const;
    void set_waterColorShallow(const Color value);

    float get_waterBeersLaw() const;
    void set_waterBeersLaw(const float value);

    float get_waterDepthOffset() const;
    void set_waterDepthOffset(const float value);

    float get_waterEdgeScale() const;
    void set_waterEdgeScale(const float value);

    float get_waterNear() const;
    void set_waterNear(const float value);

    float get_waterFar() const;
    void set_waterFar(const float value);

    Color get_waterEdgeColor() const;
    void set_waterEdgeColor(const Color value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers(const int value);

    Ref<ShaderMaterial> get_customShader() const;
    void set_customShader(const Ref<ShaderMaterial> &value);

};
#endif