#ifndef TERRA_BRUSH_H
#define TERRA_BRUSH_H

#include "editor_resources/texture_set_resource.h"
#include "editor_resources/texture_sets_resource.h"
#include "editor_resources/foliage_resource.h"
#include "editor_resources/object_resource.h"
#include "editor_resources/water_resource.h"
#include "editor_resources/snow_resource.h"
#include "editor_resources/meta_info_layer_resource.h"
#include "nodes/water.h"
#include "nodes/terrain.h"
#include "nodes/snow.h"
#include "misc/enums.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class TerraBrush : public Node3D {
    GDCLASS(TerraBrush, Node3D)

private:
    const int HeightMapFactor = 1;

    Terrain *_terrain;
    Node3D *_foliagesNode;
    Node3D *_objectsContainerNode;
    Node3D *_waterNodeContainer;
    Water *_waterNode;
    Node3D *_snowNodeContainer;
    Snow *_snowNode;

    TypedDictionary<Ref<ImageTexture>, Ref<Image>> _imageTexturesCache;
    Ref<Texture2D> _defaultNoise;

    // General settings
    String _dataPath;
    int _zonesSize;
    int _resolution;
    bool _collisionOnly;
    int _visualInstanceLayers;
    Ref<ShaderMaterial> _customShader;

    // LOD Settings
    int _lodLevels;
    int _lodRowsPerLevel;
    float _lodInitialCellWidth;

    // Collision settings
    bool _createCollisionInThread;
    int _collisionLayers;
    int _collisionMask;

    // Texture settings
    Ref<TextureSetsResource> _textureSets;
    int _textureDetail;
    bool _useAntiTile;
    bool _nearestTextureFilter;
    float _heightBlendFactor;
    AlphaChannelUsage _albedoAlphaChannelUsage;
    AlphaChannelUsage _normalAlphaChannelUsage;
    bool _useSharpTransitions;

    // Foliage settings
    TypedArray<Ref<FoliageResource>> _foliages;

    // Object settings
    int _defaultObjectFrequency;
    ObjectLoadingStrategy _objectLoadingStrategy;
    TypedArray<Ref<ObjectResource>> _objects;

    // Water settings
    Ref<WaterResource> _waterDefinition;

    // Snow settings
    Ref<SnowResource> _snowDefinition;

    // Meta info settings
    bool _showMetaInfo;
    TypedArray<Ref<MetaInfoLayerResource>> _metaInfoLayers;

    // Zones
    Ref<ZonesResource> _terrainZones;

    void loadTerrain();
    void createFoliages();
    void createWater();
    void createSnow();
    void createMetaInfo();
    Ref<Image> getImageFromImageTexture(Ref<ImageTexture> texture);

protected:
    static void _bind_methods();

public:
    TerraBrush();
    ~TerraBrush();

    void _ready() override;
    PackedStringArray _get_configuration_warnings() const override;

    String get_dataPath() const;
    void set_dataPath(const String value);

    int get_zonesSize() const;
    void set_zonesSize(const int value);

    int get_resolution() const;
    void set_resolution(const int value);

    bool get_collisionOnly() const;
    void set_collisionOnly(const bool value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers(const int value);

    Ref<ShaderMaterial> get_customShader() const;
    void set_customShader(const Ref<ShaderMaterial> &value);

    int get_lodLevels() const;
    void set_lodLevels(const int value);

    int get_lodRowsPerLevel() const;
    void set_lodRowsPerLevel(const int value);

    float get_lodInitialCellWidth() const;
    void set_lodInitialCellWidth(const float value);

    bool get_createCollisionInThread() const;
    void set_createCollisionInThread(const bool value);

    int get_collisionLayers() const;
    void set_collisionLayers(const int value);

    int get_collisionMask() const;
    void set_collisionMask(const int value);

    Ref<TextureSetsResource> get_textureSets() const;
    void set_textureSets(const Ref<TextureSetsResource> &value);

    int get_textureDetail() const;
    void set_textureDetail(const int value);

    bool get_useAntiTile() const;
    void set_useAntiTile(const bool value);

    bool get_nearestTextureFilter() const;
    void set_nearestTextureFilter(const bool value);

    float get_heightBlendFactor() const;
    void set_heightBlendFactor(const float value);

    AlphaChannelUsage get_albedoAlphaChannelUsage() const;
    void set_albedoAlphaChannelUsage(const AlphaChannelUsage value);

    AlphaChannelUsage get_normalAlphaChannelUsage() const;
    void set_normalAlphaChannelUsage(const AlphaChannelUsage value);

    bool get_useSharpTransitions() const;
    void set_useSharpTransitions(const bool value);

    TypedArray<Ref<FoliageResource>> get_foliages() const;
    void set_foliages(const TypedArray<Ref<FoliageResource>> value);

    int get_defaultObjectFrequency() const;
    void set_defaultObjectFrequency(const int value);

    ObjectLoadingStrategy get_objectLoadingStrategy() const;
    void set_objectLoadingStrategy(const ObjectLoadingStrategy value);

    TypedArray<Ref<ObjectResource>> get_objects() const;
    void set_objects(const TypedArray<Ref<ObjectResource>> value);

    Ref<WaterResource> get_waterDefinition() const;
    void set_waterDefinition(const Ref<WaterResource> &value);

    Ref<SnowResource> get_snowDefinition() const;
    void set_snowDefinition(const Ref<SnowResource> &value);

    bool get_showMetaInfo() const;
    void set_showMetaInfo(const bool value);

    TypedArray<Ref<MetaInfoLayerResource>> get_metaInfoLayers() const;
    void set_metaInfoLayers(const TypedArray<Ref<MetaInfoLayerResource>> value);

    Ref<ZonesResource> get_terrainZones() const;
    void set_terrainZones(const Ref<ZonesResource> &value);

    void onCreateTerrain();
    void onRemoveTerrain();
    void onUpdateTerrainSettings();
    void clearObjects();
    void createSplatmaps(Ref<ZoneResource> zone);
    void createObjects();
    void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones);
    void updateCameraPosition(Camera3D *viewportCamera);
    void saveResources();
    void addInteractionPoint(float x, float y);
    // Ref<TerrainPositionInformation> getPositionInformation(float x, float y); TODO : GDExtension
    void onLockTerrain();
    void onUnlockTerrain();
    Ref<ZoneResource> addNewZone(Vector2i zonePosition);
};
#endif