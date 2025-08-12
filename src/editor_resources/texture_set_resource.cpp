#include "texture_set_resource.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void TextureSetResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &TextureSetResource::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "value"), &TextureSetResource::set_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

    ClassDB::bind_method(D_METHOD("get_albedoTexture"), &TextureSetResource::get_albedoTexture);
    ClassDB::bind_method(D_METHOD("set_albedoTexture", "value"), &TextureSetResource::set_albedoTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "albedoTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_albedoTexture", "get_albedoTexture");

    ClassDB::bind_method(D_METHOD("get_normalTexture"), &TextureSetResource::get_normalTexture);
    ClassDB::bind_method(D_METHOD("set_normalTexture", "value"), &TextureSetResource::set_normalTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normalTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_normalTexture", "get_normalTexture");

    ClassDB::bind_method(D_METHOD("get_roughnessTexture"), &TextureSetResource::get_roughnessTexture);
    ClassDB::bind_method(D_METHOD("set_roughnessTexture", "value"), &TextureSetResource::set_roughnessTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "roughnessTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_roughnessTexture", "get_roughnessTexture");

    ClassDB::bind_method(D_METHOD("get_heightTexture"), &TextureSetResource::get_heightTexture);
    ClassDB::bind_method(D_METHOD("set_heightTexture", "value"), &TextureSetResource::set_heightTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "heightTexture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_heightTexture", "get_heightTexture");

    ClassDB::bind_method(D_METHOD("get_textureDetail"), &TextureSetResource::get_textureDetail);
    ClassDB::bind_method(D_METHOD("set_textureDetail", "value"), &TextureSetResource::set_textureDetail);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "textureDetail"), "set_textureDetail", "get_textureDetail");

    ClassDB::bind_method(D_METHOD("get_triplanar"), &TextureSetResource::get_triplanar);
    ClassDB::bind_method(D_METHOD("set_triplanar", "value"), &TextureSetResource::set_triplanar);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "triplanar"), "set_triplanar", "get_triplanar");
}

TextureSetResource::TextureSetResource() {
    _textureDetail = -1;
}

TextureSetResource::~TextureSetResource() {}

String TextureSetResource::get_name() const {
    return _name;
}
void TextureSetResource::set_name(const String value) {
    _name = value;
}

Ref<Texture2D> TextureSetResource::get_albedoTexture() const {
    return _albedoTexture;
}
void TextureSetResource::set_albedoTexture(const Ref<Texture2D> &value) {
    if (Engine::get_singleton()->is_editor_hint() && !value.is_null() && !value->get_path().is_empty() && (_albedoTexture.is_null() || value->get_path() != _albedoTexture->get_path())) {
        static const String NormalFilesHint = "normal";
        static const String RoughnessFilesHint = "roughness";
        static const String HeightFilesHint = "height";

        auto directory = value->get_path().get_base_dir();
        auto directoryFiles = DirAccess::get_files_at(directory);
        String normalFile;

        for (auto i = 0; i < directoryFiles.size(); i++) {
            auto currentFileName = directory + "/" + directoryFiles[i];
            if (currentFileName.ends_with(".import")) {
                continue;
            }

            if (currentFileName.containsn(NormalFilesHint)) {
                if (normalFile.is_empty()) {
                    normalFile = currentFileName;
                } else if (currentFileName.containsn("GL") && !normalFile.containsn("GL")) {
                    normalFile = currentFileName;
                }
            } else if (currentFileName.containsn(RoughnessFilesHint) && _roughnessTexture.is_null()) {
                _roughnessTexture = ResourceLoader::get_singleton()->load(currentFileName);
            } else if (currentFileName.containsn(HeightFilesHint) && _heightTexture.is_null()) {
                _heightTexture = ResourceLoader::get_singleton()->load(currentFileName);
            }
        }

        if (!normalFile.is_empty()) {
            _normalTexture = ResourceLoader::get_singleton()->load(normalFile);
        }
    }

    _albedoTexture = value;
}

Ref<Texture2D> TextureSetResource::get_normalTexture() const {
    return _normalTexture;
}
void TextureSetResource::set_normalTexture(const Ref<Texture2D> &value) {
    _normalTexture = value;
}

Ref<Texture2D> TextureSetResource::get_roughnessTexture() const {
    return _roughnessTexture;
}
void TextureSetResource::set_roughnessTexture(const Ref<Texture2D> &value) {
    _roughnessTexture = value;
}

Ref<Texture2D> TextureSetResource::get_heightTexture() const {
    return _heightTexture;
}
void TextureSetResource::set_heightTexture(const Ref<Texture2D> &value) {
    _heightTexture = value;
}

int TextureSetResource::get_textureDetail() const {
    return _textureDetail;
}
void TextureSetResource::set_textureDetail(const int value) {
    _textureDetail = value;
}

bool TextureSetResource::get_triplanar() const {
    return _triplanar;
}
void TextureSetResource::set_triplanar(const bool value) {
    _triplanar = value;
}