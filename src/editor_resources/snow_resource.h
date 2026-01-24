#ifndef SNOW_RESOURCE_H
#define SNOW_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class SnowResource : public Resource {
    GDCLASS(SnowResource, Resource);

private:
    float _snowFactor = 0;
    float _snowInnerOffset = 0;
    Ref<Texture2D> _snowColorTexture = nullptr;
    Ref<Texture2D> _snowColorNormal = nullptr;
    Ref<Texture2D> _snowColorRoughness = nullptr;
    float _snowColorDetail = 0;
    Ref<Texture2D> _noise = nullptr;
    float _noiseFactor = 0;
    float _metallic = 0;
    int _visualInstanceLayers = 0;
    Ref<ShaderMaterial> _customShader = nullptr;
    float _decompressSpeed = 0.5f;
    float _decompressOffsetSpeed = 2.0f;

protected:
    static void _bind_methods();

public:
    SnowResource();
    ~SnowResource();

    float get_snowFactor() const;
    void set_snowFactor(const float value);

    float get_snowInnerOffset() const;
    void set_snowInnerOffset(const float value);

    Ref<Texture2D> get_snowColorTexture() const;
    void set_snowColorTexture(const Ref<Texture2D> &value);

    Ref<Texture2D> get_snowColorNormal() const;
    void set_snowColorNormal(const Ref<Texture2D> &value);

    Ref<Texture2D> get_snowColorRoughness() const;
    void set_snowColorRoughness(const Ref<Texture2D> &value);

    float get_snowColorDetail() const;
    void set_snowColorDetail(const float value);

    Ref<Texture2D> get_noise() const;
    void set_noise(const Ref<Texture2D> &value);

    float get_noiseFactor() const;
    void set_noiseFactor(const float value);

    float get_metallic() const;
    void set_metallic(const float value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers(const int value);

    Ref<ShaderMaterial> get_customShader() const;
    void set_customShader(const Ref<ShaderMaterial> &value);

    float get_decompressSpeed() const;
    void set_decompressSpeed(const float value);

    float get_decompressOffsetSpeed() const;
    void set_decompressOffsetSpeed(const float value);
};
#endif