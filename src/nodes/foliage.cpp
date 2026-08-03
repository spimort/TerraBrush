#include "foliage.h"
#include "../misc/utils.h"
#include "../misc/string_names.h"
#include "../editor_resources/zone_resource.h"
#include "../editor_resources/zones_resource.h"
#include "../misc/enums.h"
#include "../nodes/clipmap.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>

void Foliage::_bind_methods() {

}

void Foliage::_notification(const int what) {
    if (what == NOTIFICATION_TRANSFORM_CHANGED) {
        updateShaderOffsetPosition();
        updateEditorCameraPosition(nullptr, true);
    }
}

Foliage::Foliage() {
    _foliageIndex = 0;
    _textureDetail = 1;
}

Foliage::~Foliage() {

}

void Foliage::_ready() {
    if (_definition.is_null()) {
        return;
    }

    if (_definition->get_strategy() == FoliageStrategy::FOLIAGESTRATEGY_MULTIMESH)  {
        _multiMeshInstancesContainer = memnew(Node3D);
        add_child(_multiMeshInstancesContainer);

        Ref<ShaderMaterial> shaderMaterial;
        if (_definition->get_customShader().is_null()) {
            shaderMaterial = Ref<ShaderMaterial>(memnew(ShaderMaterial));
            shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader"));
        } else {
            shaderMaterial = Utils::createCustomShaderCopy(_definition->get_customShader(), TypedArray<StringName>::make(StringNames::ColorTextures(), StringNames::WaterTextures()));
        }

        _foliageShader = shaderMaterial;
    } else {
        _particles = memnew(GPUParticles3D);
        _particles->set_amount(1);
        _particles->set_lifetime(0.01);
        _particles->set_explosiveness_ratio(1.0);
        _particles->set_fixed_fps(1000);
        _particles->set_interpolate(false);

        Ref<ShaderMaterial> shaderMaterial = Ref<ShaderMaterial>(memnew(ShaderMaterial));
        shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/foliage_shader.gdshader"));

        _particles->set_process_material(shaderMaterial);

        add_child(_particles);

        _foliageShader = shaderMaterial;
    }

    _defaultNoise = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/DefaultNoise.tres");

    updateFoliage();
    updateAABB();
    updateShaderOffsetPosition();

    set_notify_transform(true);
}

void Foliage::_physics_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        updateEditorCameraPosition();
    }
}

void Foliage::set_foliageIndex(const int value) {
    _foliageIndex = value;
}

void Foliage::set_zonesSize(const int value) {
    _zonesSize = value;
}

void Foliage::set_resolution(const int value) {
    _resolution = value;
}

void Foliage::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void Foliage::set_textureSets(const Ref<TextureSetsResource> &value) {
    _textureSets = value;
}

void Foliage::set_textureDetail(const int value) {
    _textureDetail = value;
}

void Foliage::set_waterFactor(const float value) {
    _waterFactor = value;
}

void Foliage::set_definition(const Ref<FoliageDefinitionResource> &value) {
    _definition = value;
}

void Foliage::updateFoliage() {
    if ((_particles == nullptr && _multiMeshInstancesContainer == nullptr) || _terrainZones.is_null()) {
        return;
    }

    int numberOfLevels = _definition->get_lodLevels();
    int rowsPerLevel = _definition->get_lodRowsPerLevel();
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    int numberOfPointsFirstLevel = (rowsPerLevel - (-1 - rowsPerLevel)) + 2;
    int center = numberOfPointsFirstLevel * numberOfPointsFirstLevel;

    int topBottomLines = (rowsPerLevel + 1) / 2;
    int numberOfPointsOtherLevel = (rowsPerLevel - (-1 - rowsPerLevel)) + 2;
    int topBottom = (numberOfPointsOtherLevel * topBottomLines * 2) * numberOfLevels;

    int numberOfSides = numberOfPointsFirstLevel - (topBottomLines * 2);
    int numberOfSidesPoints = (rowsPerLevel + 1) / 2;
    int sides = numberOfSides * numberOfSidesPoints * 2 * numberOfLevels;

    int numberOfPoints = center + topBottom + sides;

    if (_definition->get_strategy() == FoliageStrategy::FOLIAGESTRATEGY_MULTIMESH) {
        if (_definition->get_chunkFoliage()) {
            createMultiMeshChunks();
        } else {
            generateFullMultiMeshes();
        }

        _foliageShader->set_shader_parameter(StringNames::InitialCellWidth(), _definition->get_lodInitialCellWidth());
        _foliageShader->set_shader_parameter(StringNames::LODRowsPerLevel(), _definition->get_lodRowsPerLevel());
        _foliageShader->set_shader_parameter(StringNames::LODLevels(), _definition->get_lodLevels());
        _foliageShader->set_shader_parameter(StringNames::FoliageAlbedo(), _definition->get_albedo());

        if (_definition->get_albedoTextures().size() > 0) {
            Ref<Texture2DArray> albedoTextures = memnew(Texture2DArray);
            TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
            for (Ref<Texture2D> albedoTexture : _definition->get_albedoTextures()) {
                images.append(albedoTexture->get_image());
            }
            albedoTextures->create_from_images(images);

            _foliageShader->set_shader_parameter(_definition->get_nearestTextureFilter() ? StringNames::FoliageAlbedoTexturesNearest() : StringNames::FoliageAlbedoTextures(), albedoTextures);
            _foliageShader->set_shader_parameter(StringNames::FoliageNumberOfTexture(), _definition->get_albedoTextures().size());
            _foliageShader->set_shader_parameter(StringNames::NearestFilter(), _definition->get_nearestTextureFilter());
        }

        _foliageShader->set_shader_parameter(StringNames::UseGroundColor(), _definition->get_useGroundColor());
        _foliageShader->set_shader_parameter(StringNames::UseBrushScale(), _definition->get_useBrushScale());
        _foliageShader->set_shader_parameter(StringNames::ScaleNoiseTexture(), _definition->get_scaleNoiseTexture());
        _foliageShader->set_shader_parameter(StringNames::RandomPlacementRange(), _definition->get_randomPlacementRange());
        _foliageShader->set_shader_parameter(StringNames::ApplyOnTextureIndexes(),_definition->get_applyOnTextureIndexes());
        _foliageShader->set_shader_parameter(StringNames::NumberOfTexturesToApplyOn(), _definition->get_applyOnTextureIndexes().size());
    } else {
        _particles->set_layer_mask(_definition->get_visualInstanceLayers());
        _particles->set_draw_pass_mesh(0, _definition->get_mesh());
        _particles->set_material_override(_definition->get_meshMaterial());
        _particles->set_amount(numberOfPoints);

        if (Engine::get_singleton()->is_editor_hint()) {
            _particles->set_amount(_definition->get_editorMaximumRenderDistance() * _definition->get_editorMaximumRenderDistance());

            _foliageShader->set_shader_parameter(StringNames::MaximumDistance(), _definition->get_editorMaximumRenderDistance());
        } else {
            _particles->set_amount(_definition->get_maximumRenderDistance() * _definition->get_maximumRenderDistance());

            _foliageShader->set_shader_parameter(StringNames::MaximumDistance(), _definition->get_maximumRenderDistance());
        }
    }

    _foliageShader->set_shader_parameter(StringNames::HeightmapTextures(), _terrainZones->get_heightmapTextures());
    _foliageShader->set_shader_parameter(StringNames::ZonesSize(), (float) _zonesSize);
    _foliageShader->set_shader_parameter(StringNames::Resolution(), (float) _resolution);
    _foliageShader->set_shader_parameter(StringNames::NumberOfZones(), (float) _terrainZones->get_zones().size());
    _foliageShader->set_shader_parameter(StringNames::ZonesMap(), _terrainZones->get_zonesMap());

    if ((_definition->get_useGroundColor() || _definition->get_applyOnTextureIndexes().size() > 0) && !_textureSets.is_null() && _textureSets->get_textureSets().size() > 0) {
        _foliageShader->set_shader_parameter(StringNames::Splatmaps(), _terrainZones->get_splatmapsTextures());

        TypedArray<Ref<Texture2D>> groundTextures = TypedArray<Ref<Texture2D>>();
        for (Ref<TextureSetResource> groundTextureSet : _textureSets->get_textureSets()) {
            if (!groundTextureSet->get_albedoTexture().is_null()) {
                groundTextures.append(groundTextureSet->get_albedoTexture());
            }
        }
        _foliageShader->set_shader_parameter(StringNames::Textures(), Utils::texturesToTextureArray(groundTextures));
        _foliageShader->set_shader_parameter(StringNames::NumberOfTextures(), _textureSets->get_textureSets().size());
        _foliageShader->set_shader_parameter(StringNames::TextureDetail(), _textureDetail);
    }

    _foliageShader->set_shader_parameter(StringNames::ColorTextures(), _terrainZones->get_colorTextures());

    _foliageShader->set_shader_parameter(StringNames::FoliageTextures(), _terrainZones->get_foliagesTextures()[_foliageIndex]);
    _foliageShader->set_shader_parameter(StringNames::MeshScale(), _definition->get_meshScale());
    _foliageShader->set_shader_parameter(StringNames::WindStrength(), _definition->get_windStrength());

    _foliageShader->set_shader_parameter(StringNames::WaterTextures(), _terrainZones->get_waterTextures());
    _foliageShader->set_shader_parameter(StringNames::WaterFactor(), _waterFactor);

    Ref<Texture2D> noiseTexture = _definition->get_noiseTexture().is_null() ? _defaultNoise : _definition->get_noiseTexture();
    if (!noiseTexture.is_null()) {
        _foliageShader->set_shader_parameter(StringNames::NoiseTexture(), noiseTexture);
    }
}

void Foliage::updateEditorCameraPosition(Camera3D *viewportCamera, bool forceUpdate) {
    Camera3D *camera = nullptr;
    if (viewportCamera == nullptr) {
        if (Engine::get_singleton()->is_editor_hint()) {
            camera = EditorInterface::get_singleton()->get_editor_viewport_3d()->get_camera_3d();
        }

        if (camera == nullptr && get_viewport() != nullptr) {
            camera = get_viewport()->get_camera_3d();
        }
    } else {
        camera = viewportCamera;
    }

    if (camera == nullptr) {
        return;
    }

    updateFoliagePosition(camera->get_global_position(), forceUpdate);
}

void Foliage::updateFoliagePosition(Vector3 position, bool forceUpdate) {
    float offset = 0.0f;
    bool isEven = _zonesSize % 2 == 0;
    if (isEven) {
        offset = _definition->get_lodInitialCellWidth() / 2.0f;
    }

    float xPosition = ((int) Math::floor(position.x)) + offset;
    float zPosition = ((int) Math::floor(position.z)) + offset;

    float maxCellWidth = _definition->get_lodInitialCellWidth() * Math::pow(2.0, _definition->get_lodLevels() - 1);

    xPosition -= Math::fmod(xPosition, maxCellWidth);
    zPosition -= Math::fmod(zPosition, maxCellWidth);

    if (isEven) {
        xPosition -= _definition->get_lodInitialCellWidth() / 2.0f;
        zPosition -= _definition->get_lodInitialCellWidth() / 2.0f;
    }

    Vector3 newPosition = Vector3(xPosition, 0, zPosition);
    if (forceUpdate || newPosition.distance_to(_lastUpdatedPosition) > maxCellWidth) {
        _foliageShader->set_shader_parameter(StringNames::GlobalPosition(), newPosition);
        _multiMeshInstancesContainer->set_global_position(newPosition);
        _lastUpdatedPosition = newPosition;
    }
}

void Foliage::updateAABB() {
    if (_definition->get_strategy() == FoliageStrategy::FOLIAGESTRATEGY_MULTIMESH ||  _particles == nullptr || _terrainZones.is_null()) {
        return;
    }

    TypedArray<Vector2> zonePositions = TypedArray<Vector2>();
    int maxX = 0;
    int maxY = 0;
    for (Ref<ZoneResource> zone : _terrainZones->get_zones()) {
        Vector2 zonePosition = zone->get_zonePosition();
        zonePositions.append(zonePosition);

        maxX = Math::max(maxX, (int) Math::abs(zonePosition.x));
        maxY = Math::max(maxY, (int) Math::abs(zonePosition.y));
    }
    maxX += 1;
    maxY += 1;

    int aabbXSize = Math::max(maxX * _zonesSize * 2, _zonesSize * 2);
    int aabbYSize = Math::max(maxY * _zonesSize * 2, _zonesSize * 2);
    int aabbXPoint = -(aabbXSize / 2);
    int aabbYPoint = -(aabbYSize / 2);

    AABB aabb = AABB(Vector3(aabbXPoint, Math::max(aabbXPoint, aabbYPoint), aabbYPoint), Vector3(aabbXSize, Math::max(aabbXSize, aabbYSize), aabbYSize));
    _particles->set_custom_aabb(aabb);
}

void Foliage::updateShaderOffsetPosition() {
    if (!_foliageShader.is_null()) {
        _foliageShader->set_shader_parameter(StringNames::OffsetPosition(), get_global_position());
    }
}

void Foliage::createMultiMeshChunks() {
    for (int i = 0; i < _definition->get_lodLevels(); i++) {
        for (int x = Clipmap::MinChunkPosition; x <= Clipmap::MaxChunkPosition; x++) {
            for (int z = Clipmap::MinChunkPosition; z <= Clipmap::MaxChunkPosition; z++) {
                // For the first level, we create the center of the mesh as well, while being chunked
                if (
                    i == 0 || (
                        x == Clipmap::MinChunkPosition ||
                        z == Clipmap::MinChunkPosition ||
                        x == Clipmap::MaxChunkPosition ||
                        z == Clipmap::MaxChunkPosition
                    )
                ) {
                    createMultiMeshChunk(i + 1, Vector2(x, z));
                }
            }
        }
    }
}

void Foliage::createMultiMeshChunk(int level, Vector2 position) {
    PackedFloat32Array buffer = PackedFloat32Array();

    auto rowsPerLevel = _definition->get_lodRowsPerLevel();
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    Vector2 numberOfCellsAndWidth = generateChunkedLevel(buffer, level, rowsPerLevel, _definition->get_lodInitialCellWidth(), position);
    Vector2 resultPosition = position * Vector2(numberOfCellsAndWidth.x * numberOfCellsAndWidth.y, numberOfCellsAndWidth.x * numberOfCellsAndWidth.y);

    MultiMeshInstance3D *chunkMultiMeshInstance = memnew(MultiMeshInstance3D);
    chunkMultiMeshInstance->set_material_override(_foliageShader);
    chunkMultiMeshInstance->set_position(Vector3(resultPosition.x, 0, resultPosition.y));
    chunkMultiMeshInstance->set_layer_mask(_definition->get_visualInstanceLayers());

    Ref<godot::MultiMesh> multiMesh = memnew(godot::MultiMesh);
    chunkMultiMeshInstance->set_multimesh(multiMesh);

    multiMesh->set_transform_format(godot::MultiMesh::TransformFormat::TRANSFORM_3D);
    multiMesh->set_mesh(_definition->get_mesh());
    multiMesh->set_use_custom_data(true);
    multiMesh->set_instance_count(numberOfCellsAndWidth.x * numberOfCellsAndWidth.x);
    multiMesh->set_buffer(buffer);

    if (_definition->get_castShadow()) {
        chunkMultiMeshInstance->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_ON);
    } else {
        chunkMultiMeshInstance->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
    }

    _multiMeshInstancesContainer->add_child(chunkMultiMeshInstance);

    AABB customAABB = chunkMultiMeshInstance->get_aabb();
    customAABB.set_size(Vector3(customAABB.get_size().x, _definition->get_chunkAABBHeight() == -1 ? _zonesSize : _definition->get_chunkAABBHeight(), customAABB.get_size().z));
    chunkMultiMeshInstance->set_custom_aabb(customAABB);
}

void Foliage::generateFullMultiMeshes() {
    MultiMeshInstance3D *multiMeshInstance = memnew(MultiMeshInstance3D);
    multiMeshInstance->set_material_override(_foliageShader);
    multiMeshInstance->set_layer_mask(_definition->get_visualInstanceLayers());

    Ref<godot::MultiMesh> multiMesh = memnew(godot::MultiMesh);
    multiMeshInstance->set_multimesh(multiMesh);

    multiMesh->set_transform_format(godot::MultiMesh::TransformFormat::TRANSFORM_3D);
    multiMesh->set_mesh(_definition->get_mesh());
    multiMesh->set_use_custom_data(true);

    auto rowsPerLevel = _definition->get_lodRowsPerLevel();
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    PackedFloat32Array buffer = PackedFloat32Array();
    int totalNumberOfInstance = 0;
    for (int i = 0; i < _definition->get_lodLevels(); i++) {
        int level = i + 1;

        for (int x = Clipmap::MinChunkPosition; x <= Clipmap::MaxChunkPosition; x++) {
            for (int z = Clipmap::MinChunkPosition; z <= Clipmap::MaxChunkPosition; z++) {
                // For the first level, we create the center of the mesh as well, while being chunked
                if (
                    i == 0 || (
                        x == Clipmap::MinChunkPosition ||
                        z == Clipmap::MinChunkPosition ||
                        x == Clipmap::MaxChunkPosition ||
                        z == Clipmap::MaxChunkPosition
                    )
                ) {
                    Vector2 numberOfCellsAndWidth = generateChunkedLevel(buffer, level, rowsPerLevel, _definition->get_lodInitialCellWidth(), Vector2(x, z), true);
                    totalNumberOfInstance += numberOfCellsAndWidth.x * numberOfCellsAndWidth.x;
                }
            }
        }
    }

    multiMesh->set_instance_count(totalNumberOfInstance);
    multiMesh->set_buffer(buffer);

    if (_definition->get_castShadow()) {
        multiMeshInstance->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_ON);
    } else {
        multiMeshInstance->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
    }

    _multiMeshInstancesContainer->add_child(multiMeshInstance);

    AABB customAABB = multiMeshInstance->get_aabb();
    customAABB.set_size(Vector3(customAABB.get_size().x, _definition->get_chunkAABBHeight() == -1 ? _zonesSize : _definition->get_chunkAABBHeight(), customAABB.get_size().z));
    multiMeshInstance->set_custom_aabb(customAABB);
}

Vector2 Foliage::generateChunkedLevel(PackedFloat32Array &buffer, int level, int rowsPerLevel, float initialCellWidth, Vector2 chunkPosition, bool useGlobalPosition) {
    auto width = initialCellWidth * ((float) Math::pow(2.0, level - 1));

    int numberOfCells = Math::floor(rowsPerLevel / 2.0) + 1;
    int minCellValue = Clipmap::MinChunkPosition * numberOfCells;
    int maxCellValue = ((Clipmap::MaxChunkPosition + 1) * numberOfCells) - 1;

    for (int x = 0; x < numberOfCells; x++) {
        for (int z = 0; z < numberOfCells; z++) {
            float globalXCellPosition = (chunkPosition.x * numberOfCells + x) * width;
            float globalZCellPosition = (chunkPosition.y * numberOfCells + z) * width;

            float xPosition = useGlobalPosition ? globalXCellPosition : x * width;
            float zPosition = useGlobalPosition ? globalZCellPosition : z * width;

            buffer.append_array({
                1.0, 0.0, 0.0, xPosition,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, zPosition,
                // 0.0, 0.0, 0.0, 0.0, Color values, we don't use it right now, we use custom data values instead
               globalXCellPosition, 0.0, globalZCellPosition, 0.0 // Custom data values
            });
        }
    }

    return Vector2(numberOfCells, width);
}
