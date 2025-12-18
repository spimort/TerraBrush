#include "texture_set_resource.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/project_settings.hpp>

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

    ClassDB::bind_method(D_METHOD("get_metallic"), &TextureSetResource::get_metallic);
    ClassDB::bind_method(D_METHOD("set_metallic", "value"), &TextureSetResource::set_metallic);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "metallic", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_metallic", "get_metallic");

    ClassDB::bind_method(D_METHOD("get_specular"), &TextureSetResource::get_specular);
    ClassDB::bind_method(D_METHOD("set_specular", "value"), &TextureSetResource::set_specular);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "specular", PROPERTY_HINT_RANGE, "0,1,0.001"), "set_specular", "get_specular");
}

TextureSetResource::TextureSetResource() {
    _textureDetail = -1;
    _name = String("");
    _albedoTexture = Ref<Texture2D>(nullptr);
    _normalTexture = Ref<Texture2D>(nullptr);
    _roughnessTexture = Ref<Texture2D>(nullptr);
    _heightTexture = Ref<Texture2D>(nullptr);
    _triplanar = false;
    _metallic = 0.0;
    _specular = 0.5;
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
    bool autoDetectTextures = ProjectSettings::get_singleton()->get_setting(SettingContants::AutoDetectTextures(), SettingContants::AutoDetectTexturesDefaultValue());
    if (autoDetectTextures && Engine::get_singleton()->is_editor_hint() && !value.is_null() && !value->get_path().is_empty() && (_albedoTexture.is_null() || value->get_path() != _albedoTexture->get_path())) {
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

float TextureSetResource::get_metallic() const {
    return _metallic;
}
void TextureSetResource::set_metallic(const float value) {
    _metallic = value;
}

float TextureSetResource::get_specular() const {
    return _specular;
}
void TextureSetResource::set_specular(const float value) {
    _specular = value;
}