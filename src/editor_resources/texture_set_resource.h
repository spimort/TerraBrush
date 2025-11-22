#ifndef TEXTURE_SET_RESOURCE_H
#define TEXTURE_SET_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class TextureSetResource : public Resource {
    GDCLASS(TextureSetResource, Resource);

private:
    String _name = "";
    Ref<Texture2D> _albedoTexture = nullptr;
    Ref<Texture2D> _normalTexture = nullptr;
    Ref<Texture2D> _roughnessTexture = nullptr;
    Ref<Texture2D> _heightTexture = nullptr;
    int _textureDetail = 0;
    bool _triplanar = false;

protected:
    static void _bind_methods();

public:
    TextureSetResource();
    ~TextureSetResource();

    String get_name() const;
    void set_name(const String value);

    Ref<Texture2D> get_albedoTexture() const;
    void set_albedoTexture(const Ref<Texture2D> &value);

    Ref<Texture2D> get_normalTexture() const;
    void set_normalTexture(const Ref<Texture2D> &value);

    Ref<Texture2D> get_roughnessTexture() const;
    void set_roughnessTexture(const Ref<Texture2D> &value);

    Ref<Texture2D> get_heightTexture() const;
    void set_heightTexture(const Ref<Texture2D> &value);

    int get_textureDetail() const;
    void set_textureDetail(const int value);

    bool get_triplanar() const;
    void set_triplanar(const bool value);
};
#endif