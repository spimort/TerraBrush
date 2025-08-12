#ifndef SNOW_RESOURCE_H
#define SNOW_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class SnowResource : public Resource {
    GDCLASS(SnowResource, Resource)

private:
    float _snowFactor;
    float _snowInnerOffset;
    Ref<Texture2D> _snowColorTexture;
    Ref<Texture2D> _snowColorNormal;
    Ref<Texture2D> _snowColorRoughness;
    float _snowColorDetail;
    Ref<Texture2D> _noise;
    float _noiseFactor;
    float _metallic;
    int _visualInstanceLayers;
    Ref<ShaderMaterial> _customShader;

protected:
    static void _bind_methods();

public:
    SnowResource();
    ~SnowResource();

    float get_snowFactor() const;
    void set_snowFactor (const float value);

    float get_snowInnerOffset() const;
    void set_snowInnerOffset (const float value);

    Ref<Texture2D> get_snowColorTexture() const;
    void set_snowColorTexture (const Ref<Texture2D> &value);

    Ref<Texture2D> get_snowColorNormal() const;
    void set_snowColorNormal (const Ref<Texture2D> &value);

    Ref<Texture2D> get_snowColorRoughness() const;
    void set_snowColorRoughness (const Ref<Texture2D> &value);

    float get_snowColorDetail() const;
    void set_snowColorDetail (const float value);

    Ref<Texture2D> get_noise() const;
    void set_noise (const Ref<Texture2D> &value);

    float get_noiseFactor() const;
    void set_noiseFactor (const float value);

    float get_metallic() const;
    void set_metallic (const float value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers (const int value);

    Ref<ShaderMaterial> get_customShader() const;
    void set_customShader (const Ref<ShaderMaterial> &value);
};
#endif