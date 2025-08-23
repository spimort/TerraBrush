#ifndef TERRAIN_H
#define TERRAIN_H

#include "clipmap.h"
#include "../editor_resources/texture_sets_resource.h"
#include "../editor_resources/meta_info_layer_resource.h"
#include "../misc/enums.h"
#include "../misc/thread_utils.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/height_map_shape3d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Terrain : public Node3D {
    GDCLASS(Terrain, Node3D)

private:
    static constexpr const char* HeightMapTextureKey = "HeightMapTexture";
    static constexpr const char* WaterTextureKey = "WaterTexture";
    const float HoleValue = std::numeric_limits<float>::quiet_NaN();

    Clipmap *_clipmap;
    StaticBody3D *_terrainCollider;

    int _zonesSize;
    int _resolution;
    Ref<ZonesResource> _terrainZones;
    float _heightMapFactor;
    Ref<ShaderMaterial> _customShader;
    Ref<TextureSetsResource> _textureSets;
    int _textureDetail;
    bool _useAntiTile;
    bool _nearestTextureFilter;
    float _heightBlendFactor;
    AlphaChannelUsage _albedoAlphaChannelUsage;
    AlphaChannelUsage _normalAlphaChannelUsage;
    bool _useSharpTransitions;
    float _waterFactor;
    Ref<Texture2D> _defaultTexture;
    int _visualInstanceLayers;
    int _collisionLayers;
    int _collisionMask;
    int _lodLevels;
    int _lodRowsPerLevel;
    float _lodInitialCellWidth;
    bool _collisionOnly;
    bool _createCollisionInThread;
    bool _showMetaInfo;
    TypedArray<Ref<MetaInfoLayerResource>> _metaInfoLayers;

    Ref<Thread> _collisionThread;
    CancellationSource _collisionCancellationSource;

    void terrainTextureUpdated();
    void terrainSplatmapsUpdated();
    void updateCollisionShape();
    void assignCollisionData(const Ref<HeightMapShape3D> &shape, const PackedFloat32Array data);
    void updateTextures();
    float getHeightForZone(Ref<ZoneResource> zone, int x, int y, TypedDictionary<Ref<ZoneResource>, TypedDictionary<String, Ref<Texture2D>>> imagesCache);
    void onUpdateTerrainCollision(const TypedArray<Ref<HeightMapShape3D>> shapes);
    Ref<ZoneResource> getZoneForPosition(int x, int y);

protected:
    static void _bind_methods();
    void _notification(int what);

public:
    Terrain();
    ~Terrain();

    void _ready() override;

    void set_zonesSize(const int value);
    void set_resolution(const int value);
    void set_terrainZones(const Ref<ZonesResource> &value);
    void set_heightMapFactor(const float value);
    void set_customShader(const Ref<ShaderMaterial> &value);
    void set_textureSets(const Ref<TextureSetsResource> &value);
    void set_textureDetail(const int value);
    void set_useAntiTile(const bool value);
    void set_nearestTextureFilter(const bool value);
    void set_heightBlendFactor(const float value);
    void set_albedoAlphaChannelUsage(const AlphaChannelUsage value);
    void set_normalAlphaChannelUsage(const AlphaChannelUsage value);
    void set_useSharpTransitions(const bool value);
    void set_waterFactor(const float value);
    void set_defaultTexture(const Ref<Texture2D> &value);
    void set_visualInstanceLayers(const int value);
    void set_collisionLayers(const int value);
    void set_collisionMask(const int value);
    void set_lodLevels(const int value);
    void set_lodRowsPerLevel(const int value);
    void set_lodInitialCellWidth(const float value);
    void set_collisionOnly(const bool value);
    void set_createCollisionInThread(const bool value);
    void set_showMetaInfo(const bool value);
    void set_metaInfoLayers(const TypedArray<Ref<MetaInfoLayerResource>> value);

    void buildTerrain();
    void terrainUpdated();
    void terrainWaterUpdated();
    Ref<HeightMapShape3D> addZoneCollision(Ref<ZoneResource> zone);
};
#endif