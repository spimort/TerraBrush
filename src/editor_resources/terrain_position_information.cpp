#include "terrain_position_information.h"

using namespace godot;

// TerrainPositionTextureInformation

void TerrainPositionTextureInformation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_index"), &TerrainPositionTextureInformation::get_index);
    ClassDB::bind_method(D_METHOD("set_index", "value"), &TerrainPositionTextureInformation::set_index);

    ClassDB::bind_method(D_METHOD("get_name"), &TerrainPositionTextureInformation::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "value"), &TerrainPositionTextureInformation::set_name);

    ClassDB::bind_method(D_METHOD("get_factor"), &TerrainPositionTextureInformation::get_factor);
    ClassDB::bind_method(D_METHOD("set_factor", "value"), &TerrainPositionTextureInformation::set_factor);
}

TerrainPositionTextureInformation::TerrainPositionTextureInformation () {}

TerrainPositionTextureInformation::~TerrainPositionTextureInformation() {}

int TerrainPositionTextureInformation::get_index() const {
    return _index;
}
void TerrainPositionTextureInformation::set_index(const int value) {
    _index = value;
}

String TerrainPositionTextureInformation::get_name() const {
    return _name;
}
void TerrainPositionTextureInformation::set_name(const String value) {
    _name = value;
}

float TerrainPositionTextureInformation::get_factor() const {
    return _factor;
}
void TerrainPositionTextureInformation::set_factor(const float value) {
    _factor = value;
}

// TerrainPositionInformation

void TerrainPositionInformation::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_textures"), &TerrainPositionInformation::get_textures);
    ClassDB::bind_method(D_METHOD("set_textures", "value"), &TerrainPositionInformation::set_textures);

    ClassDB::bind_method(D_METHOD("get_waterFactor"), &TerrainPositionInformation::get_waterFactor);
    ClassDB::bind_method(D_METHOD("set_waterFactor", "value"), &TerrainPositionInformation::set_waterFactor);

    ClassDB::bind_method(D_METHOD("get_waterDeepness"), &TerrainPositionInformation::get_waterDeepness);
    ClassDB::bind_method(D_METHOD("set_waterDeepness", "value"), &TerrainPositionInformation::set_waterDeepness);

    ClassDB::bind_method(D_METHOD("get_snowFactor"), &TerrainPositionInformation::get_snowFactor);
    ClassDB::bind_method(D_METHOD("set_snowFactor", "value"), &TerrainPositionInformation::set_snowFactor);

    ClassDB::bind_method(D_METHOD("get_snowHeight"), &TerrainPositionInformation::get_snowHeight);
    ClassDB::bind_method(D_METHOD("set_snowHeight", "value"), &TerrainPositionInformation::set_snowHeight);

    ClassDB::bind_method(D_METHOD("get_metaInfoIndex"), &TerrainPositionInformation::get_metaInfoIndex);
    ClassDB::bind_method(D_METHOD("set_metaInfoIndex", "value"), &TerrainPositionInformation::set_metaInfoIndex);

    ClassDB::bind_method(D_METHOD("get_metaInfoName"), &TerrainPositionInformation::get_metaInfoName);
    ClassDB::bind_method(D_METHOD("set_metaInfoName", "value"), &TerrainPositionInformation::set_metaInfoName);
}

TerrainPositionInformation::TerrainPositionInformation() {}

TerrainPositionInformation::~TerrainPositionInformation() {}

TypedArray<Ref<TerrainPositionTextureInformation>> TerrainPositionInformation::get_textures() const {
    return _textures;
}
void TerrainPositionInformation::set_textures(const TypedArray<Ref<TerrainPositionTextureInformation>> value) {
    _textures = value;
}

float TerrainPositionInformation::get_waterFactor() const {
    return _waterFactor;
}
void TerrainPositionInformation::set_waterFactor(const float value) {
    _waterFactor = value;
}

float TerrainPositionInformation::get_waterDeepness() const {
    return _waterDeepness;
}
void TerrainPositionInformation::set_waterDeepness(const float value) {
    _waterDeepness = value;
}

float TerrainPositionInformation::get_snowFactor() const {
    return _snowFactor;
}
void TerrainPositionInformation::set_snowFactor(const float value) {
    _snowFactor = value;
}

float TerrainPositionInformation::get_snowHeight() const {
    return _snowHeight;
}
void TerrainPositionInformation::set_snowHeight(const float value) {
    _snowHeight = value;
}

int TerrainPositionInformation::get_metaInfoIndex() const {
    return _metaInfoIndex;
}
void TerrainPositionInformation::set_metaInfoIndex(const int value) {
    _metaInfoIndex = value;
}

String TerrainPositionInformation::get_metaInfoName() const {
    return _metaInfoName;
}
void TerrainPositionInformation::set_metaInfoName(const String value) {
    _metaInfoName = value;
}