#include "texture_sets_resource.h"

using namespace godot;

void TextureSetsResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_textureSets"), &TextureSetsResource::get_textureSets);
    ClassDB::bind_method(D_METHOD("set_textureSets", "value"), &TextureSetsResource::set_textureSets);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "textureSets", PROPERTY_HINT_TYPE_STRING, "24/17:TextureSetResource"), "set_textureSets", "get_textureSets");
}

TextureSetsResource::TextureSetsResource() {}

TextureSetsResource::~TextureSetsResource() {}

TypedArray<Ref<TextureSetResource>> TextureSetsResource::get_textureSets() const {
    return _textureSets;
}
void TextureSetsResource::set_textureSets(const TypedArray<Ref<TextureSetResource>> &value) {
    _textureSets = value;
}