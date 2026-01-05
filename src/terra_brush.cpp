#include "terra_brush.h"
#include "misc/utils.h"
#include "misc/zone_utils.h"
#include "misc/string_names.h"
#include "nodes/foliage.h"
#include "nodes/objects.h"
#include "nodes/objects_octree_multi_mesh.h"
#include "nodes/objects_base.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/display_server.hpp>

#include <vector>
#include <algorithm>

using namespace godot;

void TerraBrush::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onTerrainCollisionUpdated"), &TerraBrush::onTerrainCollisionUpdated);
    ClassDB::bind_method(D_METHOD("getPositionInformation", "x", "y"), &TerraBrush::getPositionInformation);
    ClassDB::bind_method(D_METHOD("addInteractionPoint", "x", "y"), &TerraBrush::addInteractionPoint);
    ClassDB::bind_method(D_METHOD("onUpdateTerrainSettings"), &TerraBrush::onUpdateTerrainSettings);
    ClassDB::bind_method(D_METHOD("getTerrainCollider"), &TerraBrush::getTerrainCollider);
    ClassDB::bind_method(D_METHOD("getHeightAtPosition", "x", "z", "useGlobalPosition"), &TerraBrush::getHeightAtPosition);
    ClassDB::bind_method(D_METHOD("getHeightForMousePosition", "camera"), &TerraBrush::getHeightForMousePosition);
    ClassDB::bind_method(D_METHOD("getHeightForScreenPosition", "camera", "screenPosition"), &TerraBrush::getHeightForScreenPosition);

    ADD_SIGNAL(MethodInfo(StringNames::TerrainLoaded()));

    ADD_GROUP("TerrainSettings", "");

    ClassDB::bind_method(D_METHOD("get_version"), &TerraBrush::get_version);

    ClassDB::bind_method(D_METHOD("get_dataPath"), &TerraBrush::get_dataPath);
    ClassDB::bind_method(D_METHOD("set_dataPath", "value"), &TerraBrush::set_dataPath);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "dataPath", PROPERTY_HINT_DIR), "set_dataPath", "get_dataPath");

    ClassDB::bind_method(D_METHOD("get_zonesSize"), &TerraBrush::get_zonesSize);
    ClassDB::bind_method(D_METHOD("set_zonesSize", "value"), &TerraBrush::set_zonesSize);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "zonesSize"), "set_zonesSize", "get_zonesSize");

    ClassDB::bind_method(D_METHOD("get_resolution"), &TerraBrush::get_resolution);
    ClassDB::bind_method(D_METHOD("set_resolution", "value"), &TerraBrush::set_resolution);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "resolution"), "set_resolution", "get_resolution");

    ClassDB::bind_method(D_METHOD("get_collisionOnly"), &TerraBrush::get_collisionOnly);
    ClassDB::bind_method(D_METHOD("set_collisionOnly", "value"), &TerraBrush::set_collisionOnly);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collisionOnly"), "set_collisionOnly", "get_collisionOnly");

    ClassDB::bind_method(D_METHOD("get_visualInstanceLayers"), &TerraBrush::get_visualInstanceLayers);
    ClassDB::bind_method(D_METHOD("set_visualInstanceLayers", "value"), &TerraBrush::set_visualInstanceLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visualInstanceLayers", PROPERTY_HINT_LAYERS_3D_RENDER), "set_visualInstanceLayers", "get_visualInstanceLayers");

    ClassDB::bind_method(D_METHOD("get_customShader"), &TerraBrush::get_customShader);
    ClassDB::bind_method(D_METHOD("set_customShader", "value"), &TerraBrush::set_customShader);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "customShader", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial"), "set_customShader", "get_customShader");

    ADD_GROUP("LOD", "");

    ClassDB::bind_method(D_METHOD("get_lodLevels"), &TerraBrush::get_lodLevels);
    ClassDB::bind_method(D_METHOD("set_lodLevels", "value"), &TerraBrush::set_lodLevels);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lodLevels"), "set_lodLevels", "get_lodLevels");

    ClassDB::bind_method(D_METHOD("get_lodRowsPerLevel"), &TerraBrush::get_lodRowsPerLevel);
    ClassDB::bind_method(D_METHOD("set_lodRowsPerLevel", "value"), &TerraBrush::set_lodRowsPerLevel);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lodRowsPerLevel"), "set_lodRowsPerLevel", "get_lodRowsPerLevel");

    ClassDB::bind_method(D_METHOD("get_lodInitialCellWidth"), &TerraBrush::get_lodInitialCellWidth);
    ClassDB::bind_method(D_METHOD("set_lodInitialCellWidth", "value"), &TerraBrush::set_lodInitialCellWidth);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lodInitialCellWidth"), "set_lodInitialCellWidth", "get_lodInitialCellWidth");

    ADD_GROUP("Collisions", "");

    ClassDB::bind_method(D_METHOD("get_createCollisionInThread"), &TerraBrush::get_createCollisionInThread);
    ClassDB::bind_method(D_METHOD("set_createCollisionInThread", "value"), &TerraBrush::set_createCollisionInThread);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "createCollisionInThread"), "set_createCollisionInThread", "get_createCollisionInThread");

    ClassDB::bind_method(D_METHOD("get_collisionLayers"), &TerraBrush::get_collisionLayers);
    ClassDB::bind_method(D_METHOD("set_collisionLayers", "value"), &TerraBrush::set_collisionLayers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collisionLayers", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collisionLayers", "get_collisionLayers");

    ClassDB::bind_method(D_METHOD("get_collisionMask"), &TerraBrush::get_collisionMask);
    ClassDB::bind_method(D_METHOD("set_collisionMask", "value"), &TerraBrush::set_collisionMask);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collisionMask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collisionMask", "get_collisionMask");

    ADD_GROUP("Textures", "");

    ClassDB::bind_method(D_METHOD("get_textureSets"), &TerraBrush::get_textureSets);
    ClassDB::bind_method(D_METHOD("set_textureSets", "value"), &TerraBrush::set_textureSets);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "textureSets", PROPERTY_HINT_RESOURCE_TYPE, "TextureSetsResource"), "set_textureSets", "get_textureSets");

    ClassDB::bind_method(D_METHOD("get_textureDetail"), &TerraBrush::get_textureDetail);
    ClassDB::bind_method(D_METHOD("set_textureDetail", "value"), &TerraBrush::set_textureDetail);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "textureDetail"), "set_textureDetail", "get_textureDetail");

    ClassDB::bind_method(D_METHOD("get_useAntiTile"), &TerraBrush::get_useAntiTile);
    ClassDB::bind_method(D_METHOD("set_useAntiTile", "value"), &TerraBrush::set_useAntiTile);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "useAntiTile"), "set_useAntiTile", "get_useAntiTile");

    ClassDB::bind_method(D_METHOD("get_nearestTextureFilter"), &TerraBrush::get_nearestTextureFilter);
    ClassDB::bind_method(D_METHOD("set_nearestTextureFilter", "value"), &TerraBrush::set_nearestTextureFilter);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "nearestTextureFilter"), "set_nearestTextureFilter", "get_nearestTextureFilter");

    ClassDB::bind_method(D_METHOD("get_heightBlendFactor"), &TerraBrush::get_heightBlendFactor);
    ClassDB::bind_method(D_METHOD("set_heightBlendFactor", "value"), &TerraBrush::set_heightBlendFactor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "heightBlendFactor"), "set_heightBlendFactor", "get_heightBlendFactor");

    ClassDB::bind_method(D_METHOD("get_albedoAlphaChannelUsage"), &TerraBrush::get_albedoAlphaChannelUsage);
    ClassDB::bind_method(D_METHOD("set_albedoAlphaChannelUsage", "value"), &TerraBrush::set_albedoAlphaChannelUsage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "albedoAlphaChannelUsage", PROPERTY_HINT_ENUM, "None:0,Roughness:1,Height:2"), "set_albedoAlphaChannelUsage", "get_albedoAlphaChannelUsage");

    ClassDB::bind_method(D_METHOD("get_normalAlphaChannelUsage"), &TerraBrush::get_normalAlphaChannelUsage);
    ClassDB::bind_method(D_METHOD("set_normalAlphaChannelUsage", "value"), &TerraBrush::set_normalAlphaChannelUsage);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "normalAlphaChannelUsage", PROPERTY_HINT_ENUM, "None:0,Roughness:1,Height:2"), "set_normalAlphaChannelUsage", "get_normalAlphaChannelUsage");

    ClassDB::bind_method(D_METHOD("get_useSharpTransitions"), &TerraBrush::get_useSharpTransitions);
    ClassDB::bind_method(D_METHOD("set_useSharpTransitions", "value"), &TerraBrush::set_useSharpTransitions);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "useSharpTransitions"), "set_useSharpTransitions", "get_useSharpTransitions");

    ADD_GROUP("Foliages", "");

    ClassDB::bind_method(D_METHOD("get_foliages"), &TerraBrush::get_foliages);
    ClassDB::bind_method(D_METHOD("set_foliages", "value"), &TerraBrush::set_foliages);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "foliages", PROPERTY_HINT_TYPE_STRING, "24/17:FoliageResource"), "set_foliages", "get_foliages");

    ADD_GROUP("Objects", "");

    ClassDB::bind_method(D_METHOD("get_defaultObjectFrequency"), &TerraBrush::get_defaultObjectFrequency);
    ClassDB::bind_method(D_METHOD("set_defaultObjectFrequency", "value"), &TerraBrush::set_defaultObjectFrequency);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "defaultObjectFrequency"), "set_defaultObjectFrequency", "get_defaultObjectFrequency");

    ClassDB::bind_method(D_METHOD("get_objectLoadingStrategy"), &TerraBrush::get_objectLoadingStrategy);
    ClassDB::bind_method(D_METHOD("set_objectLoadingStrategy", "value"), &TerraBrush::set_objectLoadingStrategy);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "objectLoadingStrategy", PROPERTY_HINT_ENUM, "ThreadedInEditorOnly:1,Threaded:2,NotThreaded:3"), "set_objectLoadingStrategy", "get_objectLoadingStrategy");

    ClassDB::bind_method(D_METHOD("get_objects"), &TerraBrush::get_objects);
    ClassDB::bind_method(D_METHOD("set_objects", "value"), &TerraBrush::set_objects);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "objects", PROPERTY_HINT_TYPE_STRING, "24/17:ObjectResource"), "set_objects", "get_objects");

    ADD_GROUP("Water", "");

    ClassDB::bind_method(D_METHOD("get_waterDefinition"), &TerraBrush::get_waterDefinition);
    ClassDB::bind_method(D_METHOD("set_waterDefinition", "value"), &TerraBrush::set_waterDefinition);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterDefinition", PROPERTY_HINT_RESOURCE_TYPE, "WaterResource"), "set_waterDefinition", "get_waterDefinition");

    ADD_GROUP("Snow", "");

    ClassDB::bind_method(D_METHOD("get_snowDefinition"), &TerraBrush::get_snowDefinition);
    ClassDB::bind_method(D_METHOD("set_snowDefinition", "value"), &TerraBrush::set_snowDefinition);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowDefinition", PROPERTY_HINT_RESOURCE_TYPE, "SnowResource"), "set_snowDefinition", "get_snowDefinition");

    ADD_GROUP("Meta", "");

    ClassDB::bind_method(D_METHOD("get_showMetaInfo"), &TerraBrush::get_showMetaInfo);
    ClassDB::bind_method(D_METHOD("set_showMetaInfo", "value"), &TerraBrush::set_showMetaInfo);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "showMetaInfo"), "set_showMetaInfo", "get_showMetaInfo");

    ClassDB::bind_method(D_METHOD("get_metaInfoLayers"), &TerraBrush::get_metaInfoLayers);
    ClassDB::bind_method(D_METHOD("set_metaInfoLayers", "value"), &TerraBrush::set_metaInfoLayers);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "metaInfoLayers", PROPERTY_HINT_TYPE_STRING, "24/17:MetaInfoLayerResource"), "set_metaInfoLayers", "get_metaInfoLayers");

    ADD_GROUP("Zones", "");

    ClassDB::bind_method(D_METHOD("get_terrainZones"), &TerraBrush::get_terrainZones);
    ClassDB::bind_method(D_METHOD("set_terrainZones", "value"), &TerraBrush::set_terrainZones);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "terrainZones", PROPERTY_HINT_RESOURCE_TYPE, "ZonesResource"), "set_terrainZones", "get_terrainZones");
}

TerraBrush::TerraBrush() {
    _terrain = nullptr;
    _foliagesNode = nullptr;
    _objectsContainerNode = nullptr;
    _waterNodeContainer = nullptr;
    _waterNode = nullptr;
    _snowNodeContainer = nullptr;
    _snowNode = nullptr;

    _defaultNoise = Ref<Texture2D>(nullptr);

    // General settings
    _dataPath = String("");
    _zonesSize = 256;
    _resolution = 1;
    _collisionOnly = false;
    _visualInstanceLayers = 1;
    _customShader = Ref<ShaderMaterial>(nullptr);

    // LOD Settings
    _lodLevels = 5;
    _lodRowsPerLevel = 50;
    _lodInitialCellWidth = 1;

    // Collision settings
    _createCollisionInThread = true;
    _collisionLayers = 1;
    _collisionMask = 1;

    // Texture settings
    _textureDetail = 10;
    _textureSets = Ref<TextureSetsResource>(nullptr);
    _useAntiTile = true;
    _nearestTextureFilter = false;
    _heightBlendFactor = 10;
    _albedoAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    _normalAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    _useSharpTransitions = false;

    // Foliages settings
    _foliages = TypedArray<Ref<FoliageResource>>();

    // Objcets settings
    _objects = TypedArray<Ref<ObjectResource>>();

    // Water settings
    _waterDefinition = Ref<WaterResource>(nullptr);

    // Snow settings
    _snowDefinition = Ref<SnowResource>(nullptr);

    // Object settings
    _defaultObjectFrequency = 10;
    _objectLoadingStrategy = ObjectLoadingStrategy::OBJECTLOADINGSTRATEGY_THREADEDINEDITORONLY;

    // Meta info settings
    _showMetaInfo = false;

    // Zones
    _terrainZones = Ref<ZonesResource>(nullptr);
}

TerraBrush::~TerraBrush() {}

void TerraBrush::_ready() {
    _defaultNoise = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/DefaultNoise.tres");

    if (_dataPath.is_empty()) {
        String scenePath = get_tree()->get_edited_scene_root()->get_scene_file_path();
        if (!scenePath.is_empty()) {
            _dataPath = scenePath.replace(scenePath.get_file(), get_tree()->get_edited_scene_root()->get_name());
        }
    }

    if (!_terrainZones.is_null()) {
        loadTerrain();
    }
}

PackedStringArray TerraBrush::_get_configuration_warnings() const {
    PackedStringArray warnings = PackedStringArray();

    if (_dataPath.is_empty()) {
        warnings.append("DataPath is required");
    }

    if (_resolution != 1) {
        if (!Utils::isPowerOfTwo(_resolution)) {
            warnings.append("Resolution must be a power of 2");
        }

        if (!Utils::isPowerOfTwo(_zonesSize - 1)) {
            warnings.append("ZonesSize must be a (power of 2) + 1");
        }

        if (_lodInitialCellWidth != _resolution) {
            warnings.append("LODInitialCellWidth should be equals to Resolution for better result");
        }
    }

    return warnings;
}

String TerraBrush::get_version() const {
    return _version;
}

String TerraBrush::get_dataPath() const {
    return _dataPath;
}
void TerraBrush::set_dataPath(const String value) {
    _dataPath = value;

    update_configuration_warnings();
}

int TerraBrush::get_zonesSize() const {
    return _zonesSize;
}
void TerraBrush::set_zonesSize(const int value) {
    if (_terrain == nullptr) {
        if (_resolution != 1 && !Utils::isPowerOfTwo(value - 1)) {
            OS::get_singleton()->alert("When the resolution is not 1, it must be a (power of 2) + 1 (ex. 257).");
            return;
        }

        _zonesSize = value;

        update_configuration_warnings();
    } else if (value != _zonesSize) {
        OS::get_singleton()->alert("The ZonesSize property cannot change once the terrain has been created. Make sure you remove the terrain before changing the ZonesSize.");
    }
}

int TerraBrush::get_resolution() const {
    return _resolution;
}
void TerraBrush::set_resolution(const int value) {
    if (_terrain == nullptr) {
        if (value < 1) {
            OS::get_singleton()->alert("The minimum value for the resolution is 1.");
            return;
        }

        if (value > 1 && !Utils::isPowerOfTwo(value)){
            OS::get_singleton()->alert("When the resolution is not 1, it must be a power of 2.");
            return;
        }

        _resolution = value;

        update_configuration_warnings();
    } else if (value != _resolution) {
        OS::get_singleton()->alert("The Resolution property cannot change once the terrain has been created. Make sure you remove the terrain before changing the Resolution.");
    }
}

bool TerraBrush::get_collisionOnly() const {
    return _collisionOnly;
}
void TerraBrush::set_collisionOnly(const bool value) {
    _collisionOnly = value;
}

int TerraBrush::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void TerraBrush::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

Ref<ShaderMaterial> TerraBrush::get_customShader() const {
    return _customShader;
}
void TerraBrush::set_customShader(const Ref<ShaderMaterial> &value) {
    _customShader = value;

    if (!value.is_null() && value->get_shader().is_null()) {
        Ref<Shader> defaultShader = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader.gdshader");
        auto defaultCode = defaultShader->get_code();

        Ref<Shader> newShader = memnew(Shader);
        newShader->set_code(defaultCode);
        value->set_shader(newShader);
    }
}

int TerraBrush::get_lodLevels() const {
    return _lodLevels;
}
void TerraBrush::set_lodLevels(const int value) {
    _lodLevels = value;
}

int TerraBrush::get_lodRowsPerLevel() const {
    return _lodRowsPerLevel;
}
void TerraBrush::set_lodRowsPerLevel(const int value) {
    _lodRowsPerLevel = value;
}

float TerraBrush::get_lodInitialCellWidth() const {
    return _lodInitialCellWidth;
}
void TerraBrush::set_lodInitialCellWidth(const float value) {
    _lodInitialCellWidth = value;
}

bool TerraBrush::get_createCollisionInThread() const {
    return _createCollisionInThread;
}
void TerraBrush::set_createCollisionInThread(const bool value) {
    _createCollisionInThread = value;
}

int TerraBrush::get_collisionLayers() const {
    return _collisionLayers;
}
void TerraBrush::set_collisionLayers(const int value) {
    _collisionLayers = value;
}

int TerraBrush::get_collisionMask() const {
    return _collisionMask;
}
void TerraBrush::set_collisionMask(const int value) {
    _collisionMask = value;
}

Ref<TextureSetsResource> TerraBrush::get_textureSets() const {
    return _textureSets;
}
void TerraBrush::set_textureSets(const Ref<TextureSetsResource> &value) {
    _textureSets = value;
}

int TerraBrush::get_textureDetail() const {
    return _textureDetail;
}
void TerraBrush::set_textureDetail(const int value) {
    _textureDetail = value;
}

bool TerraBrush::get_useAntiTile() const {
    return _useAntiTile;
}
void TerraBrush::set_useAntiTile(const bool value) {
    _useAntiTile = value;
}

bool TerraBrush::get_nearestTextureFilter() const {
    return _nearestTextureFilter;
}
void TerraBrush::set_nearestTextureFilter(const bool value) {
    _nearestTextureFilter = value;
}

float TerraBrush::get_heightBlendFactor() const {
    return _heightBlendFactor;
}
void TerraBrush::set_heightBlendFactor(const float value) {
    _heightBlendFactor = value;
}

AlphaChannelUsage TerraBrush::get_albedoAlphaChannelUsage() const {
    return _albedoAlphaChannelUsage;
}
void TerraBrush::set_albedoAlphaChannelUsage(const AlphaChannelUsage value) {
    _albedoAlphaChannelUsage = value;
}

AlphaChannelUsage TerraBrush::get_normalAlphaChannelUsage() const {
    return _normalAlphaChannelUsage;
}
void TerraBrush::set_normalAlphaChannelUsage(const AlphaChannelUsage value) {
    _normalAlphaChannelUsage = value;
}

bool TerraBrush::get_useSharpTransitions() const {
    return _useSharpTransitions;
}
void TerraBrush::set_useSharpTransitions(const bool value) {
    _useSharpTransitions = value;
}

TypedArray<Ref<FoliageResource>> TerraBrush::get_foliages() const {
    return _foliages;
}
void TerraBrush::set_foliages(const TypedArray<Ref<FoliageResource>> value) {
    _foliages = value;
}

int TerraBrush::get_defaultObjectFrequency() const {
    return _defaultObjectFrequency;
}
void TerraBrush::set_defaultObjectFrequency(const int value) {
    _defaultObjectFrequency = value;
}

ObjectLoadingStrategy TerraBrush::get_objectLoadingStrategy() const {
    return _objectLoadingStrategy;
}
void TerraBrush::set_objectLoadingStrategy(const ObjectLoadingStrategy value) {
    _objectLoadingStrategy = value;
}

TypedArray<Ref<ObjectResource>> TerraBrush::get_objects() const {
    return _objects;
}
void TerraBrush::set_objects(const TypedArray<Ref<ObjectResource>> value) {
    _objects = value;
}

Ref<WaterResource> TerraBrush::get_waterDefinition() const {
    return _waterDefinition;
}
void TerraBrush::set_waterDefinition(const Ref<WaterResource> &value) {
    _waterDefinition = value;
}

Ref<SnowResource> TerraBrush::get_snowDefinition() const {
    return _snowDefinition;
}
void TerraBrush::set_snowDefinition(const Ref<SnowResource> &value) {
    _snowDefinition = value;
}

bool TerraBrush::get_showMetaInfo() const {
    return _showMetaInfo;
}
void TerraBrush::set_showMetaInfo(const bool value) {
    _showMetaInfo = value;
}

TypedArray<Ref<MetaInfoLayerResource>> TerraBrush::get_metaInfoLayers() const {
    return _metaInfoLayers;
}
void TerraBrush::set_metaInfoLayers(const TypedArray<Ref<MetaInfoLayerResource>> value) {
    _metaInfoLayers = value;
}

Ref<ZonesResource> TerraBrush::get_terrainZones() const {
    return _terrainZones;
}
void TerraBrush::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void TerraBrush::loadTerrain() {
    if (_terrainZones.is_null()) {
        return;
    }

    for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[i];

        if (zone->get_heightMapImage().is_null()) {
            zone->set_heightMapImage(ZoneUtils::createHeightmapImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
        }

        createSplatmaps(zone);
    }
    _terrainZones->updateSplatmapsTextures();

    if (Engine::get_singleton()->is_editor_hint()) {
        _terrainZones->updateLockTexture(_zonesSize);
    }
    _terrainZones->updateZonesMap();
    _terrainZones->updateHeightmaps();
    _terrainZones->updateColorTextures(_zonesSize);

    if (Engine::get_singleton()->is_editor_hint() || (!_collisionOnly && DisplayServer::get_singleton()->window_can_draw())) {
        // Water needs to be created first so we have the reference to the image texture
        createWater();
    }

    _terrain = memnew(Terrain);
    _terrain->set_textureSets(_textureSets);
    _terrain->set_visualInstanceLayers(_visualInstanceLayers);
    _terrain->set_customShader(_customShader);
    _terrain->set_collisionLayers(_collisionLayers);
    _terrain->set_collisionMask(_collisionMask);
    _terrain->set_zonesSize(_zonesSize);
    _terrain->set_resolution(_resolution);
    _terrain->set_terrainZones(_terrainZones);
    _terrain->set_heightMapFactor(HeightMapFactor);
    _terrain->set_textureDetail(_textureDetail);
    _terrain->set_useAntiTile(_useAntiTile);
    _terrain->set_nearestTextureFilter(_nearestTextureFilter);
    _terrain->set_heightBlendFactor(_heightBlendFactor);
    _terrain->set_albedoAlphaChannelUsage(_albedoAlphaChannelUsage);
    _terrain->set_normalAlphaChannelUsage(_normalAlphaChannelUsage);
    _terrain->set_useSharpTransitions(_useSharpTransitions);
    _terrain->set_waterFactor(_waterDefinition.is_null() ? 0 : _waterDefinition->get_waterFactor());
    _terrain->set_lodLevels(_lodLevels);
    _terrain->set_lodRowsPerLevel(_lodRowsPerLevel);
    _terrain->set_lodInitialCellWidth(_lodInitialCellWidth);
    _terrain->set_collisionOnly(_collisionOnly);
    _terrain->set_createCollisionInThread(_createCollisionInThread);
    _terrain->set_showMetaInfo(_showMetaInfo);
    _terrain->set_metaInfoLayers(_metaInfoLayers);

    // This is attached to emit the initialized event
    _terrain->connect(StringNames::TerrainCollisionUpdated(), Callable(this, "onTerrainCollisionUpdated"));

    add_child(_terrain);

    createObjects();

    if (Engine::get_singleton()->is_editor_hint() || (!_collisionOnly && DisplayServer::get_singleton()->window_can_draw())) {
        createFoliages();
        createSnow();
    }

    createMetaInfo();
}

void TerraBrush::createFoliages() {
    if (_foliages.size() == 0) {
        return;
    }

    _foliagesNode = Object::cast_to<Node3D>(get_node_or_null("Foliages"));
    if (_foliagesNode == nullptr) {
        _foliagesNode = memnew(Node3D);
        add_child(_foliagesNode);
    }

    for (int i = 0; i < _foliagesNode->get_child_count(); i++) {
        _foliagesNode->get_child(i)->queue_free();
    }

    for (int zoneIndex = 0; zoneIndex < _terrainZones->get_zones().size(); zoneIndex++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[zoneIndex];
        TypedArray<Ref<Image>> newList = TypedArray<Ref<Image>>();
        for (int foliageIndex = 0; foliageIndex < _foliages.size(); foliageIndex++) {
            if (zone->get_foliagesImage().size() > foliageIndex) {
                newList.append(zone->get_foliagesImage()[foliageIndex]);
            } else {
                newList.append(ZoneUtils::createFoliageImage(_zonesSize, zone->get_zonePosition(), foliageIndex, _dataPath));
            }
        }

        zone->set_foliagesImage(newList);
    }

    _terrainZones->initializeFoliageTextures(_foliages.size());
    _terrainZones->updateFoliagesTextures();

    for (int i = 0; i < _foliages.size(); i++) {
        Ref<FoliageResource> foliage = _foliages[i];
        if (foliage.is_null()) {
            break;
        }

        if (!foliage->get_definition().is_null()) {
            Foliage *newFoliage = memnew(Foliage);

            newFoliage->set_foliageIndex(i);
            newFoliage->set_zonesSize(_zonesSize);
            newFoliage->set_resolution(_resolution);
            newFoliage->set_terrainZones(_terrainZones);
            newFoliage->set_textureSets(_textureSets);
            newFoliage->set_textureDetail(_textureDetail);
            newFoliage->set_waterFactor(_waterDefinition.is_null() ? 0 : _waterDefinition->get_waterFactor());
            newFoliage->set_definition(foliage->get_definition());

            _foliagesNode->add_child(newFoliage);
        }
    }
}

void TerraBrush::createWater() {
    if (_waterDefinition.is_null()) {
        return;
    }

    for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[i];

        if (zone->get_waterImage().is_null()) {
            zone->set_waterImage(ZoneUtils::createWaterImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
        }
    }

    _terrainZones->updateWaterTextures();

    _waterNodeContainer = Object::cast_to<Node3D>(get_node_or_null("Water"));
    if (_waterNodeContainer == nullptr) {
        _waterNodeContainer = memnew(Node3D);
        add_child(_waterNodeContainer);

        _waterNode = memnew(Water);

        _waterNode->set_terrainZones(_terrainZones);
        _waterNode->set_zonesSize(_zonesSize);
        _waterNode->set_resolution(_resolution);
        _waterNode->set_waterFactor(_waterDefinition->get_waterFactor());
        _waterNode->set_waterInnerOffset(_waterDefinition->get_waterInnerOffset());
        _waterNode->set_heightMapFactor(HeightMapFactor);
        _waterNode->set_waterColor(_waterDefinition->get_waterColor());
        _waterNode->set_fresnelColor(_waterDefinition->get_waterFresnelColor());
        _waterNode->set_metallic(_waterDefinition->get_waterMetallic());
        _waterNode->set_roughness(_waterDefinition->get_waterRoughness());
        _waterNode->set_timeScale(_waterDefinition->get_waterTimeScale());
        _waterNode->set_strength(_waterDefinition->get_waterStrength());
        _waterNode->set_noiseScale(_waterDefinition->get_waterNoiseScale());
        _waterNode->set_heightScale(_waterDefinition->get_waterHeightScale());
        _waterNode->set_colorDeep(_waterDefinition->get_waterColorDeep());
        _waterNode->set_colorShallow(_waterDefinition->get_waterColorShallow());
        _waterNode->set_beersLaw(_waterDefinition->get_waterBeersLaw());
        _waterNode->set_depthOffset(_waterDefinition->get_waterDepthOffset());
        _waterNode->set_edgeScale(_waterDefinition->get_waterEdgeScale());
        _waterNode->set_near(_waterDefinition->get_waterNear());
        _waterNode->set_far(_waterDefinition->get_waterFar());
        _waterNode->set_edgeColor(_waterDefinition->get_waterEdgeColor());
        _waterNode->set_visualInstanceLayers(_waterDefinition->get_visualInstanceLayers());
        _waterNode->set_lodLevels(_lodLevels);
        _waterNode->set_lodRowsPerLevel(_lodRowsPerLevel);
        _waterNode->set_lodInitialCellWidth(_lodInitialCellWidth);
        _waterNode->set_customShader(_waterDefinition->get_customShader());

        _waterNode->set_wave(_waterDefinition->get_waterWave());
        _waterNode->set_normalMap(_waterDefinition->get_waterNormalMap());
        _waterNode->set_normalMap2(_waterDefinition->get_waterNormalMap2());

        _waterNodeContainer->add_child(_waterNode);
    }
}

void TerraBrush::createSnow() {
    if (_snowDefinition.is_null()) {
        return;
    }

    for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[i];

        if (zone->get_snowImage().is_null()) {
            zone->set_snowImage(ZoneUtils::createSnowImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
        }
    }

    _snowNodeContainer = Object::cast_to<Node3D>(get_node_or_null("Snow"));
    if (_snowNodeContainer == nullptr) {
        _snowNodeContainer = memnew(Node3D);
        add_child(_snowNodeContainer);
    }

    _terrainZones->updateSnowTextures();

    _snowNode = memnew(Snow);

    _snowNode->set_terrainZones(_terrainZones);
    _snowNode->set_zonesSize(_zonesSize);
    _snowNode->set_resolution(_resolution);
    _snowNode->set_snowDefinition(_snowDefinition);
    _snowNode->set_lodLevels(_lodLevels);
    _snowNode->set_lodRowsPerLevel(_lodRowsPerLevel);
    _snowNode->set_lodInitialCellWidth(_lodInitialCellWidth);

    _snowNodeContainer->add_child(_snowNode);
}

void TerraBrush::createMetaInfo() {
    if (_metaInfoLayers.size() == 0) {
        return;
    }

    for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[i];

        if (zone->get_metaInfoImage().is_null()) {
            zone->set_metaInfoImage(ZoneUtils::createMetaInfoImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
        }
    }

    _terrainZones->updateMetaInfoTextures();
}

Terrain *TerraBrush::get_terrain() {
    return _terrain;
}

Node3D *TerraBrush::get_objectsContainer() {
    return _objectsContainerNode;
}

Ref<Texture2D> TerraBrush::get_defaultNoise() {
    return _defaultNoise;
}

void TerraBrush::onTerrainCollisionUpdated() {
    if (!_initialized) {
        _initialized = true;
        emit_signal(StringNames::TerrainLoaded());
    }
}

void TerraBrush::onCreateTerrain() {
    if (_resolution != 1) {
        if (!Utils::isPowerOfTwo(_resolution)) {
            return;
        }

        if (!Utils::isPowerOfTwo(_zonesSize - 1)) {
            return;
        }
    }

    if (_dataPath.is_empty()) {
        return;
    }

    if (!DirAccess::dir_exists_absolute(_dataPath)) {
        DirAccess::make_dir_absolute(_dataPath);
    }

    onRemoveTerrain();

    _terrainZones = Ref<ZonesResource>(memnew(ZonesResource));

    Ref<ZoneResource> zoneResource = memnew(ZoneResource);
    zoneResource->set_heightMapImage(ZoneUtils::createHeightmapImage(_zonesSize, _resolution, Vector2i(0, 0), _dataPath));
    _terrainZones->set_zones(Array::make(zoneResource));

    loadTerrain();
}

void TerraBrush::onRemoveTerrain() {
    if (_terrain != nullptr) {
        _terrain->queue_free();
        _terrain = nullptr;
    }

    if (_foliagesNode != nullptr) {
        _foliagesNode->queue_free();
        _foliagesNode = nullptr;
    }

    clearObjects();

    if (_waterNodeContainer != nullptr) {
        _waterNodeContainer->queue_free();
        _waterNodeContainer = nullptr;

        _waterNode = nullptr;
    }

    if (_snowNodeContainer != nullptr) {
        _snowNodeContainer->queue_free();
        _snowNodeContainer = nullptr;

        _snowNode = nullptr;
    }

    _terrainZones = Ref<ZonesResource>(nullptr);
}

void TerraBrush::onUpdateTerrainSettings() {
    if (_terrain != nullptr) {
        _terrain->queue_free();
        _terrain = nullptr;
    }

    if (_foliagesNode != nullptr) {
        _foliagesNode->queue_free();
        _foliagesNode = nullptr;
    }

    clearObjects();

    if (_waterNodeContainer != nullptr) {
        _waterNodeContainer->queue_free();
        _waterNodeContainer = nullptr;

        _waterNode = nullptr;
    }

    if (_snowNodeContainer != nullptr) {
        _snowNodeContainer->queue_free();
        _snowNodeContainer = nullptr;

        _snowNode = nullptr;
    }

    loadTerrain();
}

void TerraBrush::clearObjects() {
    if (_objectsContainerNode != nullptr) {
        _objectsContainerNode->queue_free();
        _objectsContainerNode = nullptr;
    }
}

void TerraBrush::createSplatmaps(Ref<ZoneResource> zone) {
    int numberOfSplatmaps = (int) Math::ceil(_textureSets.is_null() ? 0 : _textureSets->get_textureSets().size() / 4.0f);

    if (zone->get_splatmapsImage().size() == 0 || zone->get_splatmapsImage().size() < numberOfSplatmaps) {
        TypedArray<Image> newList = TypedArray<Image>();
        newList.append_array(zone->get_splatmapsImage());

        for (int i = zone->get_splatmapsImage().size(); i < numberOfSplatmaps; i++) {
            newList.append(ZoneUtils::createSplatmapImage(_zonesSize, zone->get_zonePosition(), i, _dataPath));
        }

        zone->set_splatmapsImage(newList);
    }
}

void TerraBrush::createObjects() {
    if (_objects.size() == 0) {
        return;
    }

    _objectsContainerNode = Object::cast_to<Node3D>(get_node_or_null("Objects"));
    if (_objectsContainerNode == nullptr) {
        _objectsContainerNode = memnew(Node3D);
        add_child(_objectsContainerNode);
    }

    for (int zoneIndex = 0; zoneIndex < _terrainZones->get_zones().size(); zoneIndex++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[zoneIndex];
        TypedArray<Image> newList = TypedArray<Image>();
        for (int objectIndex = 0; objectIndex < _objects.size(); objectIndex++) {
            if (zone->get_objectsImage().size() > objectIndex) {
                newList.append(zone->get_objectsImage()[objectIndex]);
            } else {
                newList.append(ZoneUtils::createObjectImage(_zonesSize, zone->get_zonePosition(), objectIndex, _dataPath));
            }
        }

        zone->set_objectsImage(newList);
    }

    bool loadInThread = _objectLoadingStrategy == ObjectLoadingStrategy::OBJECTLOADINGSTRATEGY_THREADED || (_objectLoadingStrategy == ObjectLoadingStrategy::OBJECTLOADINGSTRATEGY_THREADEDINEDITORONLY && Engine::get_singleton()->is_editor_hint());
    for (int objectIndex = 0; objectIndex < _objects.size(); objectIndex++) {
        Ref<ObjectResource> objectItem = _objects[objectIndex];
        if (objectItem.is_null() || objectItem->get_definition().is_null()) {
            break;
        }

        if (objectItem->get_hide()) {
            continue;
        }

        ObjectsBase *objectNode;
        if (objectItem->get_definition()->get_strategy() == ObjectStrategy::OBJECTSTRATEGY_PACKEDSCENES) {
            objectNode = memnew(Objects);
        } else if (objectItem->get_definition()->get_strategy() == ObjectStrategy::OBJECTSTRATEGY_OCTREEMULTIMESHES) {
            objectNode = memnew(ObjectsOctreeMultiMesh);
        }

        objectNode->set_name(String::num_int64(objectIndex));

        objectNode->set_objectsIndex(objectIndex);
        objectNode->set_definition(objectItem->get_definition());
        objectNode->set_terrainZones(_terrainZones);
        objectNode->set_zonesSize(_zonesSize);
        objectNode->set_resolution(_resolution);
        objectNode->set_waterFactor(_waterDefinition.is_null() ? 0 : _waterDefinition->get_waterFactor());
        objectNode->set_loadInThread(loadInThread);
        objectNode->set_defaultObjectFrequency(_defaultObjectFrequency);

        _objectsContainerNode->add_child(objectNode);
    }

    _terrainZones->updateObjectsTextures();
}

void TerraBrush::updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) {
    for (int i = 0; i < _objects.size(); i++) {
        Ref<ObjectResource> objectItem = _objects[i];
        if (!objectItem->get_hide()) {
            ObjectsBase *objectsNode = Object::cast_to<ObjectsBase>(_objectsContainerNode->get_node_or_null(String::num_int64(i)));
            objectsNode->updateObjectsHeight(zones);
        }
    }
}

void TerraBrush::updateCameraPosition(Camera3D *viewportCamera) {
    if (_terrain != nullptr) {
        _terrain->get_clipmap()->updateEditorCameraPosition(viewportCamera);
    }
    if (_waterNode != nullptr) {
        _waterNode->get_clipmap()->updateEditorCameraPosition(viewportCamera);
    }
    if (_snowNode != nullptr) {
        _snowNode->get_clipmap()->updateEditorCameraPosition(viewportCamera);
    }

    if (_foliagesNode != nullptr) {
        for (int i = 0; i < _foliagesNode->get_child_count(); i++) {
            Foliage *foliageNode = Object::cast_to<Foliage>(_foliagesNode->get_child(i));
            foliageNode->updateEditorCameraPosition(viewportCamera);
        }
    }
}

void TerraBrush::saveResources() {
    if (!_dataPath.is_empty() && !_terrainZones.is_null()) {
        _terrainZones->saveResources();
    }
}

void TerraBrush::addInteractionPoint(float x, float y) {
    x += _zonesSize / 2;
    y += _zonesSize / 2;

    if (_zonesSize % 2 == 0) {
        x -= _lodInitialCellWidth / 2.0f;
        y -= _lodInitialCellWidth / 2.0f;
    }

    if (_snowNode != nullptr) {
        _snowNode->addCompressedSnow(x, y);
    }
    if (_waterNode != nullptr) {
        _waterNode->addRippleEffect(x, y);
    }
}

Ref<TerrainPositionInformation> TerraBrush::getPositionInformation(float x, float y) {
    x += _zonesSize / 2;
    y += _zonesSize / 2;

    if (_zonesSize % 2 == 0) {
        x -= _lodInitialCellWidth / 2.0f;
        y -= _lodInitialCellWidth / 2.0f;
    }

    ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(x, y, _zonesSize, _resolution);
    Ref<ZoneResource> zone = _terrainZones->getZoneForZoneInfo(zoneInfo);

    if (!zone.is_null()) {
        float waterFactor = 0;
        float snowFactor = 0;
        int metaInfoIndex = -1;
        String metaInfoName = "";

        if (!zone->get_waterImage().is_null()) {
            waterFactor = zone->get_waterImage()->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y).r;
        }

        if (!zone->get_snowImage().is_null()) {
            snowFactor = zone->get_snowImage()->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y).r;
        }

        if (_metaInfoLayers.size() > 0 && !zone->get_metaInfoImage().is_null()) {
            Color metaInfoColor = zone->get_metaInfoImage()->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
            int metaInfoColorIndex = (int) metaInfoColor.r;

            if (metaInfoColorIndex >= 0 && _metaInfoLayers.size() - 1 >= metaInfoColorIndex) {
                metaInfoIndex = metaInfoColorIndex;
                metaInfoName = Ref<MetaInfoLayerResource>(_metaInfoLayers[metaInfoColorIndex])->get_name();
            }
        }

        TypedArray<Ref<TerrainPositionTextureInformation>> textures = TypedArray<Ref<TerrainPositionTextureInformation>>();
        if (zone->get_splatmapsImage().size() > 0) {
            std::vector<Ref<TerrainPositionTextureInformation>> tempTextures = std::vector<Ref<TerrainPositionTextureInformation>>();
            for (int i = 0; i < _textureSets->get_textureSets().size(); i++) {
                Ref<TextureSetResource> textureSet = _textureSets->get_textureSets()[i];

                int splatmapIndex = (int) Math::floor(i / 4.0);
                Ref<Image> splatmapImage = zone->get_splatmapsImage()[splatmapIndex];
                Color pixel = splatmapImage->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
                int colorIndex = i % 4;

                Ref<TerrainPositionTextureInformation> textureInfo = memnew(TerrainPositionTextureInformation);
                textureInfo->set_index(i);
                textureInfo->set_name(textureSet->get_name());
                textureInfo->set_factor(pixel[colorIndex]);

                tempTextures.push_back(textureInfo);
            }

            std::sort(tempTextures.begin(), tempTextures.end(),
                ([](const Ref<TerrainPositionTextureInformation> &a, const Ref<TerrainPositionTextureInformation> &b) {
                    return a->get_factor() > b->get_factor();
                })
            );

            for (Ref<TerrainPositionTextureInformation> textureInfo : tempTextures) {
                textures.append(textureInfo);
            }
        }

        Ref<TerrainPositionInformation> info = memnew(TerrainPositionInformation);
        info->set_textures(textures);
        info->set_waterFactor(waterFactor);
        info->set_waterDeepness(waterFactor * (_waterDefinition.is_null() ? 0 : _waterDefinition->get_waterFactor()));
        info->set_snowFactor(snowFactor);
        info->set_snowHeight(snowFactor * (_snowDefinition.is_null() ? 0 : _snowDefinition->get_snowFactor()));
        info->set_metaInfoIndex(metaInfoIndex);
        info->set_metaInfoName(metaInfoName);

        return info;
    }

    return nullptr;
}

void TerraBrush::onLockTerrain() {
    if (_terrainZones->get_zones().size() > 0) {
        for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
            Ref<ZoneResource> zone = _terrainZones->get_zones()[i];
            zone->set_lockTexture(ZoneUtils::createLockImage(_zonesSize, zone->get_zonePosition(), true));
        }

        _terrainZones->updateLockTexture(_zonesSize);
    }
}

void TerraBrush::onUnlockTerrain() {
    if (_terrainZones->get_zones().size() > 0) {
        for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
            Ref<ZoneResource> zone = _terrainZones->get_zones()[i];
            zone->set_lockTexture(nullptr);
        }

        _terrainZones->updateLockTexture(_zonesSize);
    }
}

Ref<ZoneResource> TerraBrush::addNewZone(Vector2i zonePosition) {
    Ref<ZoneResource> zone = memnew(ZoneResource);
    zone->set_zonePosition(zonePosition);

    initializeImagesForTerrain(zone);

    TypedArray<Ref<ZoneResource>> newList = TypedArray<Ref<ZoneResource>>();
    newList.append_array(_terrainZones->get_zones());
    newList.append(zone);
    _terrainZones->set_zones(newList);

    _terrainZones->updateImageImages(_zonesSize);

    if (_terrain != nullptr) {
        _terrain->get_clipmap()->updateAABB();
    }
    if (_waterNode != nullptr) {
        _waterNode->get_clipmap()->updateAABB();
    }
    if (_snowNode != nullptr) {
        _snowNode->get_clipmap()->updateAABB();
    }

    return zone;
}

void TerraBrush::initializeImagesForTerrain(Ref<ZoneResource> zone) {
    zone->set_heightMapImage(ZoneUtils::createHeightmapImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));

    if (!_textureSets.is_null() && _textureSets->get_textureSets().size() > 0) {
        int numberOfSplatmaps = (int) Math::ceil(_textureSets->get_textureSets().size() / 4.0f);
        TypedArray<Ref<Image>> splatmaps = TypedArray<Ref<Image>>();
        for (int i = 0; i < numberOfSplatmaps; i++) {
            splatmaps.append(ZoneUtils::createSplatmapImage(_zonesSize, zone->get_zonePosition(), i, _dataPath));
        }
        zone->set_splatmapsImage(splatmaps);
    }

    if (_foliages.size() > 0) {
        TypedArray<Ref<Image>> foliagesImage = TypedArray<Ref<Image>>();
        for (int i = 0; i < _foliages.size(); i++) {
            foliagesImage.append(ZoneUtils::createFoliageImage(_zonesSize, zone->get_zonePosition(), i, _dataPath));
        }
        zone->set_foliagesImage(foliagesImage);
    }

    if (_objects.size() > 0) {
        TypedArray<Ref<Image>> objectsImage = TypedArray<Ref<Image>>();
        for (int i = 0; i < _objects.size(); i++) {
            objectsImage.append(ZoneUtils::createObjectImage(_zonesSize, zone->get_zonePosition(), i, _dataPath));
        }
        zone->set_objectsImage(objectsImage);
    }

    if (!_waterDefinition.is_null()) {
        zone->set_waterImage(ZoneUtils::createWaterImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
    }

    if (!_snowDefinition.is_null()) {
        zone->set_snowImage(ZoneUtils::createSnowImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
    }

    if (_metaInfoLayers.size() > 0) {
        zone->set_metaInfoImage(ZoneUtils::createMetaInfoImage(_zonesSize, _resolution, zone->get_zonePosition(), _dataPath));
    }
}

StaticBody3D *TerraBrush::getTerrainCollider() const {
    return _terrain->get_terrainCollider();
}

float TerraBrush::getHeightAtPosition(float x, float z, bool useGlobalPosition) const {
    int zoneSize = get_zonesSize();
    int resolution = get_resolution();

    Vector3 globalPosition = Vector3(0, 0, 0);
    if (useGlobalPosition) {
        globalPosition = get_global_position();
        x -= globalPosition.x;
        z -= globalPosition.z;
    }

    ZoneInfo zoneInfo = ZoneUtils::getPixelToZoneInfo(x + (zoneSize / 2), z + (zoneSize / 2), zoneSize, resolution);
    Ref<ZoneResource> zone;
    if (!get_terrainZones().is_null()) {
        zone = get_terrainZones()->getZoneForZoneInfo(zoneInfo);
    }

    if (!zone.is_null() && !zone->get_heightMapImage().is_null()) {
        Ref<Image> heightMapImage = zone->get_heightMapImage();
        float zoneHeight = heightMapImage->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y).r;
        return zoneHeight + globalPosition.y;
    }

    return Utils::InfinityValue;
}

Vector3 TerraBrush::getHeightForMousePosition(Camera3D *camera) const {
    Vector2 screenPosition = camera->get_viewport()->get_mouse_position();
    return getHeightForScreenPosition(camera, screenPosition);
}

Vector3 TerraBrush::getHeightForScreenPosition(Camera3D *camera, Vector2 screenPosition) const {
    Vector3 from = camera->project_ray_origin(screenPosition);
    Vector3 direction = camera->project_ray_normal(screenPosition);

    for (int i = 0; i < 20000; i++) {
        Vector3 position = from + (direction * i * 0.1f) - get_global_position();

        float zoneHeight = getHeightAtPosition(position.x, position.z, false);
        if (zoneHeight != Utils::InfinityValue && zoneHeight >= position.y) {
            return Vector3(position.x, zoneHeight, position.z) + get_global_position();
        }
    }

    return Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
}