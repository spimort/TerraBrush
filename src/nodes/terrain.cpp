#include "terrain.h"
#include "clipmap.h"
#include "../editor_resources/meta_info_layer_resource.h"
#include "../misc/utils.h"
#include "../misc/string_names.h"
#include "../misc/zone_utils.h"
#include "../misc/enums.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/height_map_shape3d.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

void Terrain::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onUpdateTerrainCollision", "shapes"), &Terrain::onUpdateTerrainCollision);
    ClassDB::bind_method(D_METHOD("assignCollisionData", "shape", "data"), &Terrain::assignCollisionData);
}

void Terrain::_notification(int what) {
    switch (what) {
        case NOTIFICATION_EXIT_TREE: {
            if (_collisionThread.is_valid()) {
                _collisionCancellationSource.cancel();
                _collisionThread->wait_to_finish();
            }

            _collisionThread.unref();
        } break;
    }
}

Terrain::Terrain() {
    _textureDetail = 1;
    _heightBlendFactor = 10;
    _albedoAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    _normalAlphaChannelUsage = AlphaChannelUsage::ALPHACHANNELUSAGE_NONE;
    _useSharpTransitions = false;
    _visualInstanceLayers = 1;
    _collisionLayers = 1;
    _lodLevels = 8;
    _lodRowsPerLevel = 21;
    _lodInitialCellWidth = 1;
    _collisionOnly = false;
    _createCollisionInThread = true;
    _showMetaInfo = false;
}

Terrain::~Terrain() {}

void Terrain::_ready() {
    _defaultTexture = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Assets/placeholder_texture.png");

    _clipmap = memnew(Clipmap);
    add_child(_clipmap);

    _terrainCollider = memnew(StaticBody3D);
    add_child(_terrainCollider);

    buildTerrain();
}

void Terrain::set_zonesSize(const int value) {
    _zonesSize = value;
}

void Terrain::set_resolution(const int value) {
    _resolution = value;
}

void Terrain::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void Terrain::set_heightMapFactor(const float value) {
    _heightMapFactor = value;
}

void Terrain::set_customShader(const Ref<ShaderMaterial> &value) {
    _customShader = value;
}

void Terrain::set_textureSets(const Ref<TextureSetsResource> &value) {
    _textureSets = value;
}

void Terrain::set_textureDetail(const int value) {
    _textureDetail = value;
}

void Terrain::set_useAntiTile(const bool value) {
    _useAntiTile = value;
}

void Terrain::set_nearestTextureFilter(const bool value) {
    _nearestTextureFilter = value;
}

void Terrain::set_heightBlendFactor(const float value) {
    _heightBlendFactor = value;
}

void Terrain::set_albedoAlphaChannelUsage(const AlphaChannelUsage value) {
    _albedoAlphaChannelUsage = value;
}

void Terrain::set_normalAlphaChannelUsage(const AlphaChannelUsage value) {
    _normalAlphaChannelUsage = value;
}

void Terrain::set_useSharpTransitions(const bool value) {
    _useSharpTransitions = value;
}

void Terrain::set_waterFactor(const float value) {
    _waterFactor = value;
}

void Terrain::set_defaultTexture(const Ref<Texture2D> &value) {
    _defaultTexture = value;
}

void Terrain::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

void Terrain::set_collisionLayers(const int value) {
    _collisionLayers = value;
}

void Terrain::set_collisionMask(const int value) {
    _collisionMask = value;
}

void Terrain::set_lodLevels(const int value) {
    _lodLevels = value;
}

void Terrain::set_lodRowsPerLevel(const int value) {
    _lodRowsPerLevel = value;
}

void Terrain::set_lodInitialCellWidth(const float value) {
    _lodInitialCellWidth = value;
}

void Terrain::set_collisionOnly(const bool value) {
    _collisionOnly = value;
}

void Terrain::set_createCollisionInThread(const bool value) {
    _createCollisionInThread = value;
}

void Terrain::set_showMetaInfo(const bool value) {
    _showMetaInfo = value;
}

void Terrain::set_metaInfoLayers(const TypedArray<Ref<MetaInfoLayerResource>> value) {
    _metaInfoLayers = value;
}

Clipmap *Terrain::get_clipmap() const {
    return _clipmap;
}

void Terrain::terrainTextureUpdated() {
    updateTextures();
    terrainSplatmapsUpdated();
}

void Terrain::terrainSplatmapsUpdated() {
    if (_terrainZones->get_splatmapsTextures()->get_layers() == 0) {
        _clipmap->get_shader()->set_shader_parameter(StringNames::Splatmaps(), nullptr);
    } else {
        _clipmap->get_shader()->set_shader_parameter(StringNames::Splatmaps(), _terrainZones->get_splatmapsTextures());
    }
}

void Terrain::updateCollisionShape() {
    if (_createCollisionInThread) {
        if (_collisionThread.is_valid()) {
            _collisionCancellationSource.cancel();
            _collisionThread->wait_to_finish();
        }
        _collisionCancellationSource = CancellationSource();
    }

    for (int i = 0; i < _terrainCollider->get_child_count(); i++) {
        Node *collisionShape = _terrainCollider->get_child(i);
        collisionShape->queue_free();
    }

    TypedArray<Ref<HeightMapShape3D>> shapes = TypedArray<Ref<HeightMapShape3D>>();
    for (Ref<ZoneResource> zone : _terrainZones->get_zones()) {
        Ref<HeightMapShape3D> heightMapShape3D = addZoneCollision(zone);

        shapes.append(heightMapShape3D);
    }

    if (_createCollisionInThread) {
        _collisionThread.instantiate();
        _collisionThread->start(Callable(this, "onUpdateTerrainCollision").bind(shapes));
    } else {
        onUpdateTerrainCollision(shapes);
    }
}

void Terrain::onUpdateTerrainCollision(const TypedArray<Ref<HeightMapShape3D>> shapes) {
    CancellationToken token = _collisionCancellationSource.token;

    TypedDictionary<Ref<ZoneResource>, Dictionary> imagesCache = TypedDictionary<Ref<ZoneResource>, Dictionary>();

    for (int i = 0; i < _terrainZones->get_zones().size(); i++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[i];
        Ref<ZoneResource> leftNeighbourZone = getZoneForPosition(zone->get_zonePosition().x - 1, zone->get_zonePosition().y);
        Ref<ZoneResource> topNeighbourZone = getZoneForPosition(zone->get_zonePosition().x, zone->get_zonePosition().y - 1);
        Ref<ZoneResource> rightNeighbourZone = getZoneForPosition(zone->get_zonePosition().x + 1, zone->get_zonePosition().y);
        Ref<ZoneResource> bottomNeighbourZone = getZoneForPosition(zone->get_zonePosition().x, zone->get_zonePosition().y + 1);
        Ref<ZoneResource> bottomRightNeighbourZone = getZoneForPosition(zone->get_zonePosition().x + 1, zone->get_zonePosition().y + 1);

        Ref<Image> heightMapImage = zone->get_heightMapTexture()->get_image();
        Ref<Image> waterImage;

        if (!zone->get_waterTexture().is_null()) {
            waterImage = zone->get_waterTexture()->get_image();
        }

        if (token.isCancellationRequested) {
            return;
        }

        TypedArray<float> terrainData = TypedArray<float>();
        for (int y = 0; y < heightMapImage->get_height(); y++) {
            for (int x = 0; x < heightMapImage->get_width(); x++) {
                if (token.isCancellationRequested) {
                    return;
                }

                Ref<ZoneResource> currentZone = zone;
                int lookupX = x;
                int lookupY = y;
                // TODO : This does not always work but it does most of the time.
                // We should ensure of the direction of the pixel directly in the shader, so it works all the time.
                if (_zonesSize % 2 == 0) {
                    if (x == 0 && !leftNeighbourZone.is_null()) {
                        currentZone = leftNeighbourZone;
                        lookupX = heightMapImage->get_width() - 1;
                    } else if (y == 0 && !topNeighbourZone.is_null()) {
                        currentZone = topNeighbourZone;
                        lookupY = heightMapImage->get_height() - 1;
                    }
                } else {
                    if (x == heightMapImage->get_width() - 1 && y == heightMapImage->get_height() - 1 && !bottomRightNeighbourZone.is_null()) {
                        currentZone = bottomRightNeighbourZone;
                        lookupX = 0;
                        lookupY = 0;
                    } else if (x == heightMapImage->get_width() - 1 && !rightNeighbourZone.is_null()) {
                        currentZone = rightNeighbourZone;
                        lookupX = 0;
                    } else if (y == heightMapImage->get_height() - 1 && !bottomNeighbourZone.is_null()) {
                        currentZone = bottomNeighbourZone;
                        lookupY = 0;
                    }
                }

                float pixelHeight = getHeightForZone(currentZone, lookupX, lookupY, imagesCache);
                terrainData.append(pixelHeight);
            }
        }

        if (token.isCancellationRequested) {
            return;
        }

        call_deferred("assignCollisionData", shapes[i], PackedFloat32Array(terrainData));
    }
}

Ref<ZoneResource> Terrain::getZoneForPosition(int x, int y) {
    for (Ref<ZoneResource> zone : _terrainZones->get_zones()) {
        Vector2i zonePosition = zone->get_zonePosition();
        if (zonePosition.x == x && zonePosition.y == y) {
            return zone;
        }
    }

    return nullptr;
}

void Terrain::assignCollisionData(const Ref<HeightMapShape3D> &shape, const PackedFloat32Array data) {
    shape->set_map_data(data);
}

void Terrain::updateTextures() {
    _clipmap->get_shader()->set_shader_parameter(StringNames::NearestFilter(), _nearestTextureFilter);

    String filterParamName = "";
    if (_nearestTextureFilter) {
        filterParamName = "Nearest";
    }

    if (!_textureSets.is_null() && _textureSets->get_textureSets().size() > 0) {
        TypedArray<Ref<Texture2D>> albedoTextures = TypedArray<Ref<Texture2D>>();
        TypedArray<int> textureDetails = TypedArray<int>();
        TypedArray<int> texturesTriplanar = TypedArray<int>();
        bool triplanar = false;
        TypedArray<Ref<Texture2D>> normalTextures = TypedArray<Ref<Texture2D>>();
        TypedArray<Ref<Texture2D>> roughnessTextures = TypedArray<Ref<Texture2D>>();
        TypedArray<Ref<Texture2D>> heightTextures = TypedArray<Ref<Texture2D>>();
        for (Ref<TextureSetResource> textureSet : _textureSets->get_textureSets()) {
            albedoTextures.append(textureSet->get_albedoTexture());
            textureDetails.append(textureSet->get_textureDetail()  <= 0 ? _textureDetail : textureSet->get_textureDetail());
            if (textureSet->get_triplanar()) {
                triplanar = true;
                texturesTriplanar.append(1);
            } else {
                texturesTriplanar.append(0);
            }

            if (!textureSet->get_normalTexture().is_null()) {
                normalTextures.append(textureSet->get_normalTexture());
            }

            if (!textureSet->get_roughnessTexture().is_null()) {
                roughnessTextures.append(textureSet->get_roughnessTexture());
            }

            if (!textureSet->get_heightTexture().is_null()) {
                heightTextures.append(textureSet->get_heightTexture());
            }
        }

        Ref<Texture2DArray> textureArray = Utils::texturesToTextureArray(albedoTextures);

        _clipmap->get_shader()->set_shader_parameter(StringNames::TexturesDetail(), textureDetails);
        _clipmap->get_shader()->set_shader_parameter(StringNames::Triplanar(), triplanar);
        _clipmap->get_shader()->set_shader_parameter(StringNames::TexturesTriplanar(), texturesTriplanar);
        _clipmap->get_shader()->set_shader_parameter("Textures" + filterParamName, textureArray);
        _clipmap->get_shader()->set_shader_parameter(StringNames::NumberOfTextures(), textureArray->get_layers());
        _clipmap->get_shader()->set_shader_parameter(StringNames::UseSharpTransitions(), _useSharpTransitions);

        if (normalTextures.size() > 0) {
            Ref<Texture2DArray> normalArray = Utils::texturesToTextureArray(normalTextures);
            _clipmap->get_shader()->set_shader_parameter("Normals" + filterParamName, normalArray);
            _clipmap->get_shader()->set_shader_parameter(StringNames::HasNormalTextures(), true);
        }

        if (roughnessTextures.size() > 0) {
            Ref<Texture2DArray> roughnessArray = Utils::texturesToTextureArray(roughnessTextures);
            _clipmap->get_shader()->set_shader_parameter("RoughnessTextures" + filterParamName, roughnessArray);
            _clipmap->get_shader()->set_shader_parameter(StringNames::HasRoughnessTextures(), true);
        }

        if (heightTextures.size() > 0) {
            Ref<Texture2DArray> heightArray = Utils::texturesToTextureArray(heightTextures);
            _clipmap->get_shader()->set_shader_parameter("HeightTextures" + filterParamName, heightArray);
            _clipmap->get_shader()->set_shader_parameter(StringNames::HasHeightTextures(), true);
        }

        _clipmap->get_shader()->set_shader_parameter(StringNames::UseAntitile(), _useAntiTile);
        _clipmap->get_shader()->set_shader_parameter(StringNames::BlendFactor(), _heightBlendFactor);
        _clipmap->get_shader()->set_shader_parameter(StringNames::AlbedoAlphaChannelUsage(), (int) _albedoAlphaChannelUsage);
        _clipmap->get_shader()->set_shader_parameter(StringNames::NormalAlphaChannelUsage(), (int) _normalAlphaChannelUsage);
    } else if (!_defaultTexture.is_null()) {
        Ref<Texture2DArray> textureArray = Utils::texturesToTextureArray(TypedArray<Ref<Texture2D>>({_defaultTexture}));
        _clipmap->get_shader()->set_shader_parameter(StringNames::TexturesDetail(), TypedArray<int>({_textureDetail}));
        _clipmap->get_shader()->set_shader_parameter("Textures" + filterParamName, textureArray);
        _clipmap->get_shader()->set_shader_parameter(StringNames::NumberOfTextures(), textureArray->get_layers());
        _clipmap->get_shader()->set_shader_parameter(StringNames::UseAntitile(), false);
    }
}

float Terrain::getHeightForZone(Ref<ZoneResource> zone, int x, int y, TypedDictionary<Ref<ZoneResource>, Dictionary> imagesCache) {
    Dictionary zoneImages;
    if (imagesCache.has(zone)) {
        zoneImages = imagesCache[zone];
    } else {
        zoneImages = Dictionary();
        zoneImages[HeightMapTextureKey] = zone->get_heightMapTexture()->get_image();
        if (!zone->get_waterTexture().is_null()) {
            zoneImages[WaterTextureKey] = zone->get_waterTexture()->get_image();
        }

        imagesCache[zone] = zoneImages;
    }

    Color pixel = Ref<Image>(zoneImages[HeightMapTextureKey])->get_pixel(x, y);
    if (pixel.g > 0.0f) {
        return HoleValue;
    }

    float pixelHeight = pixel.r * _heightMapFactor;
    float waterHeight = 0;
    if (zoneImages.has(WaterTextureKey)) {
        waterHeight = Ref<Image>(zoneImages[WaterTextureKey])->get_pixel(x, y).r;
    }
    pixelHeight -= waterHeight * _waterFactor;

    return pixelHeight;
}

void Terrain::buildTerrain() {
    if (_clipmap == nullptr) {
        return;
    }

    if (_customShader.is_null()) {
        Ref<ShaderMaterial> shaderMaterial = memnew(ShaderMaterial);
        shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader.gdshader"));

        _clipmap->set_shader(shaderMaterial);
    } else {
        _clipmap->set_shader(Utils::createCustomShaderCopy(_customShader));
    }

    _clipmap->get_clipmapMesh()->set_layer_mask(_visualInstanceLayers);

    _terrainCollider->set_collision_layer(_collisionLayers);
    _terrainCollider->set_collision_mask(_collisionMask);

    // TODO : GDExtension (_defaultSettings.CollisionOnly)
    if (!Engine::get_singleton()->is_editor_hint() && (_collisionOnly)) { // || _defaultSettings.CollisionOnly)) {
        updateCollisionShape();
        _clipmap->get_clipmapMesh()->set_visible(false);
    } else {
        _clipmap->set_zonesSize(_zonesSize);
        _clipmap->set_resolution(_resolution);
        _clipmap->set_terrainZones(_terrainZones);
        _clipmap->set_levels(_lodLevels);
        _clipmap->set_rowsPerLevel(_lodRowsPerLevel);
        _clipmap->set_initialCellWidth(_lodInitialCellWidth);

        _clipmap->createMesh();

        if (Engine::get_singleton()->is_editor_hint()) {
            _clipmap->get_shader()->set_shader_parameter(StringNames::ApplyLockTextures(), true);
            _clipmap->get_shader()->set_shader_parameter(StringNames::LockTextures(), _terrainZones->get_lockTextures());
            if (_metaInfoLayers.size() > 0) {
                _clipmap->get_shader()->set_shader_parameter(StringNames::ApplyMetaInfoTextures(), _showMetaInfo);
                _clipmap->get_shader()->set_shader_parameter(StringNames::MetaInfoTextures(), _terrainZones->get_metaInfoTextures());

                TypedArray<Color> metaInfoColors = TypedArray<Color>();
                for (Ref<MetaInfoLayerResource> metaInfo : _metaInfoLayers) {
                    metaInfoColors.append(metaInfo->get_color());
                }
                _clipmap->get_shader()->set_shader_parameter(StringNames::MetaInfoColors(), metaInfoColors);
            }
        }

        terrainUpdated();
        terrainTextureUpdated();
        terrainWaterUpdated();
    }
}

void Terrain::terrainUpdated() {
    updateCollisionShape();
}

void Terrain::terrainWaterUpdated() {
    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterTextures(), _terrainZones->get_waterTextures());
    _clipmap->get_shader()->set_shader_parameter(StringNames::WaterFactor(), _waterFactor);
}

Ref<HeightMapShape3D> Terrain::addZoneCollision(Ref<ZoneResource> zone) {
    int resolutionZoneSize = ZoneUtils::getImageSizeForResolution(_zonesSize, _resolution);

    CollisionShape3D* collisionShape = memnew(CollisionShape3D);
    _terrainCollider->add_child(collisionShape);

    collisionShape->set_position(Vector3((_zonesSize - 1) * zone->get_zonePosition().x, 0, (_zonesSize - 1) * zone->get_zonePosition().y));
    collisionShape->set_scale(Vector3(_resolution, 1, _resolution));

    Ref<HeightMapShape3D> heightMapShape3D = memnew(HeightMapShape3D);
    collisionShape->set_shape(heightMapShape3D);

    heightMapShape3D->set_map_width(resolutionZoneSize);
    heightMapShape3D->set_map_depth(resolutionZoneSize);

    return heightMapShape3D;
}