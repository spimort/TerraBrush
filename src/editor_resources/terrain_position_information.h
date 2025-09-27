#ifndef TERRAIN_POSITION_INFORMATION_H
#define TERRAIN_POSITION_INFORMATION_H

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class TerrainPositionTextureInformation : public RefCounted {
    GDCLASS(TerrainPositionTextureInformation, RefCounted);

private:
    int _index = 0;
    String _name = "";
    float _factor = 0;

protected:
    static void _bind_methods();

public:
    TerrainPositionTextureInformation();
    ~TerrainPositionTextureInformation();

    int get_index() const;
    void set_index(const int value);

    String get_name() const;
    void set_name(const String value);

    float get_factor() const;
    void set_factor(const float value);
};

class TerrainPositionInformation : public RefCounted {
    GDCLASS(TerrainPositionInformation, RefCounted);

private:
    TypedArray<Ref<TerrainPositionTextureInformation>> _textures = TypedArray<Ref<TerrainPositionTextureInformation>>();
    float _waterFactor = 0;
    float _waterDeepness = 0;
    float _snowFactor = 0;
    float _snowHeight = 0;
    int _metaInfoIndex = 0;
    String _metaInfoName = "";

protected:
    static void _bind_methods();

public:
    TerrainPositionInformation();
    ~TerrainPositionInformation();

    TypedArray<Ref<TerrainPositionTextureInformation>> get_textures() const;
    void set_textures(const TypedArray<Ref<TerrainPositionTextureInformation>> value);

    float get_waterFactor() const;
    void set_waterFactor(const float value);

    float get_waterDeepness() const;
    void set_waterDeepness(const float value);

    float get_snowFactor() const;
    void set_snowFactor(const float value);

    float get_snowHeight() const;
    void set_snowHeight(const float value);

    int get_metaInfoIndex() const;
    void set_metaInfoIndex(const int value);

    String get_metaInfoName() const;
    void set_metaInfoName(const String value);
};
#endif