#ifndef TERRA_BRUSH_H
#define TERRA_BRUSH_H

#include "editor_resources/texture_set_resource.h"
#include "editor_resources/texture_sets_resource.h"
#include "editor_resources/foliage_resource.h"
#include "editor_resources/object_resource.h"
#include "editor_resources/water_resource.h"
#include "editor_resources/snow_resource.h"
#include "editor_resources/meta_info_layer_resource.h"
#include "editor_resources/terrain_position_information.h"

#include "nodes/water.h"
#include "nodes/terrain.h"
#include "nodes/snow.h"
#include "misc/enums.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class TerraBrush : public Node3D {
    GDCLASS(TerraBrush, Node3D);

private:
    const int HeightMapFactor = 1;

    String _version = "0.14.0-alpha";

    Terrain *_terrain = nullptr;
    Node3D *_foliagesNode = nullptr;
    Node3D *_objectsContainerNode = nullptr;
    Node3D *_waterNodeContainer = nullptr;
    Water *_waterNode = nullptr;
    Node3D *_snowNodeContainer = nullptr;
    Snow *_snowNode = nullptr;

    TypedDictionary<Ref<ImageTexture>, Ref<Image>> _imageTexturesCache = TypedDictionary<Ref<ImageTexture>, Ref<Image>>();
    Ref<Texture2D> _defaultNoise = nullptr;

    // General settings
    String _dataPath = "";
    int _zonesSize = 0;
    int _resolution = 0;
    bool _collisionOnly = false;
    int _visualInstanceLayers = 0;
    Ref<ShaderMaterial> _customShader = nullptr;

    // LOD Settings
    int _lodLevels = 0;
    int _lodRowsPerLevel = 0;
    float _lodInitialCellWidth = 0;

    // Collision settings
    bool _createCollisionInThread = false;
    int _collisionLayers = 0;
    int _collisionMask = 0;

    // Texture settings
    Ref<TextureSetsResource> _textureSets = nullptr;
    int _textureDetail = 0;
    bool _useAntiTile = false;
    bool _nearestTextureFilter = false;
    float _heightBlendFactor = 0;
    AlphaChannelUsage _albedoAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    AlphaChannelUsage _normalAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    bool _useSharpTransitions = false;

    // Foliage settings
    TypedArray<Ref<FoliageResource>> _foliages = TypedArray<Ref<FoliageResource>>();

    // Object settings
    int _defaultObjectFrequency = 0;
    ObjectLoadingStrategy _objectLoadingStrategy = ObjectLoadingStrategy::OBJECTLOADINGSTRATEGY_THREADEDINEDITORONLY;
    TypedArray<Ref<ObjectResource>> _objects = TypedArray<Ref<ObjectResource>>();

    // Water settings
    Ref<WaterResource> _waterDefinition = nullptr;

    // Snow settings
    Ref<SnowResource> _snowDefinition = nullptr;

    // Meta info settings
    bool _showMetaInfo = false;
    TypedArray<Ref<MetaInfoLayerResource>> _metaInfoLayers = TypedArray<Ref<MetaInfoLayerResource>>();

    // Zones
    Ref<ZonesResource> _terrainZones = nullptr;

    void loadTerrain();
    void createFoliages();
    void createWater();
    void createSnow();
    void createMetaInfo();
    Ref<Image> getImageFromImageTexture(Ref<ImageTexture> texture);
    void initializeImagesForTerrain(Ref<ZoneResource> zone);

protected:
    static void _bind_methods();

public:
    TerraBrush();
    ~TerraBrush();

    void _ready() override;
    PackedStringArray _get_configuration_warnings() const override;

    String get_version() const;

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

    Terrain *get_terrain();
    Node3D *get_objectsContainer();
    Ref<Texture2D> get_defaultNoise();

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
    Ref<TerrainPositionInformation> getPositionInformation(float x, float y);
    void onLockTerrain();
    void onUnlockTerrain();
    Ref<ZoneResource> addNewZone(Vector2i zonePosition);
};
#endif