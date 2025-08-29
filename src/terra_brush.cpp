#include "terra_brush.h"
#include "misc/utils.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void TerraBrush::_bind_methods() {
    ADD_GROUP("TerrainSettings", "");

    ClassDB::bind_method(D_METHOD("get_dataPath"), &TerraBrush::get_dataPath);
    ClassDB::bind_method(D_METHOD("set_dataPath", "value"), &TerraBrush::set_dataPath);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "dataPath"), "set_dataPath", "get_dataPath");

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

    _imageTexturesCache = TypedDictionary<Ref<ImageTexture>, Ref<Image>>();

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