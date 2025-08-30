#ifndef TEXTURE_SETS_RESOURCE_H
#define TEXTURE_SETS_RESOURCE_H

#include "texture_set_resource.h"

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class TextureSetsResource : public Resource {
    GDCLASS(TextureSetsResource, Resource);

private:
    TypedArray<Ref<TextureSetResource>> _textureSets;

protected:
    static void _bind_methods();

public:
    TextureSetsResource();
    ~TextureSetsResource();

    TypedArray<Ref<TextureSetResource>> get_textureSets() const;
    void set_textureSets(const TypedArray<Ref<TextureSetResource>> &value);
};
#endif