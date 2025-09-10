#ifndef WATER_RESOURCE_H
#define WATER_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class WaterResource : public Resource {
    GDCLASS(WaterResource, Resource);

private:
    float _waterFactor = 0;
    float _waterInnerOffset = 0;
    Color _waterColor = Color();
    Color _waterFresnelColor = Color();
    float _waterMetallic = 0;
    float _waterRoughness = 0;
    Ref<Texture2D> _waterNormalMap = nullptr;
    Ref<Texture2D> _waterNormalMap2 = nullptr;
    float _waterTimeScale = 0;
    float _waterStrength = 0;
    Ref<Texture2D> _waterWave = nullptr;
    float _waterNoiseScale = 0;
    float _waterHeightScale = 0;
    Color _waterColorDeep = Color();
    Color _waterColorShallow = Color();
    float _waterBeersLaw = 0;
    float _waterDepthOffset = 0;
    float _waterEdgeScale = 0;
    float _waterNear = 0;
    float _waterFar = 0;
    Color _waterEdgeColor = Color();
    int _visualInstanceLayers = 0;
    Ref<ShaderMaterial> _customShader = nullptr;

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