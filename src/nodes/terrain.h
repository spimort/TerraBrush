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
    GDCLASS(Terrain, Node3D);

private:
    static constexpr const char* HeightMapTextureKey = "HeightMapTexture";
    static constexpr const char* WaterTextureKey = "WaterTexture";
    const float HoleValue = std::numeric_limits<float>::quiet_NaN();

    Clipmap *_clipmap = nullptr;
    StaticBody3D *_terrainCollider = nullptr;

    int _zonesSize = 0;
    int _resolution = 0;
    Ref<ZonesResource> _terrainZones = nullptr;
    float _heightMapFactor = 0;
    Ref<ShaderMaterial> _customShader = nullptr;
    Ref<TextureSetsResource> _textureSets = nullptr;
    int _textureDetail = 0;
    bool _useAntiTile = false;
    bool _nearestTextureFilter = false;
    float _heightBlendFactor = 0;
    AlphaChannelUsage _albedoAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    AlphaChannelUsage _normalAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    bool _useSharpTransitions = false;
    float _waterFactor = 0;
    Ref<Texture2D> _defaultTexture = nullptr;
    int _visualInstanceLayers = 0;
    int _collisionLayers = 0;
    int _collisionMask = 0;
    int _lodLevels = 0;
    int _lodRowsPerLevel = 0;
    float _lodInitialCellWidth = 0;
    bool _collisionOnly = false;
    bool _createCollisionInThread = false;
    bool _showMetaInfo = false;
    TypedArray<Ref<MetaInfoLayerResource>> _metaInfoLayers = TypedArray<Ref<MetaInfoLayerResource>>();

    Ref<Thread> _collisionThread = nullptr;
    CancellationSource _collisionCancellationSource = CancellationSource();

    void terrainTextureUpdated();
    void terrainSplatmapsUpdated();
    void updateCollisionShape();
    void assignCollisionData(const Ref<HeightMapShape3D> &shape, const PackedFloat32Array data);
    void updateTextures();
    float getHeightForZone(Ref<ZoneResource> zone, int x, int y, TypedDictionary<Ref<ZoneResource>, Dictionary> imagesCache);
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

    Clipmap *get_clipmap() const;
    StaticBody3D *get_terrainCollider() const;

    void buildTerrain();
    void terrainUpdated();
    void terrainWaterUpdated();
    Ref<HeightMapShape3D> addZoneCollision(Ref<ZoneResource> zone);
};
#endif