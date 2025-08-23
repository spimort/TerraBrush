#include "foliage.h"
#include "../misc/utils.h"
#include "../misc/string_names.h"
#include "../editor_resources/zone_resource.h"
#include "../editor_resources/zones_resource.h"
#include "../misc/enums.h"

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

void Foliage::_bind_methods() {

}

void Foliage::_notification(const int what) {
    if (what == NOTIFICATION_TRANSFORM_CHANGED) {
        updateShaderOffsetPosition();
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
        _multiMeshInstance3D = memnew(MultiMeshInstance3D);
        add_child(_multiMeshInstance3D);

        Ref<ShaderMaterial> shaderMaterial;
        if (_definition->get_customShader().is_null()) {
            shaderMaterial = Ref<ShaderMaterial>(memnew(ShaderMaterial));
            shaderMaterial->set_shader(ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader.gdshader"));
        } else {
            shaderMaterial = Utils::createCustomShaderCopy(_definition->get_customShader());
        }

        _multiMeshInstance3D->set_material_override(shaderMaterial);
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

void Foliage::_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        Vector3 position = Vector3(0, 0, 0);
        if (get_viewport() != nullptr && get_viewport()->get_camera_3d() != nullptr) {
            position = get_viewport()->get_camera_3d()->get_global_position();
        }

        updateFoliagePosition(position);
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
    if ((_particles == nullptr && _multiMeshInstance3D == nullptr) || _terrainZones.is_null()) {
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
        _multiMeshInstance3D->set_layer_mask(_definition->get_visualInstanceLayers());

        Ref<godot::MultiMesh> multiMesh = memnew(godot::MultiMesh);
        _multiMeshInstance3D->set_multimesh(multiMesh);

        multiMesh->set_transform_format(godot::MultiMesh::TransformFormat::TRANSFORM_3D);
        multiMesh->set_mesh(_definition->get_mesh());
        multiMesh->set_instance_count(numberOfPoints);

        PackedFloat32Array buffer = PackedFloat32Array();
        for (int i = 0; i < numberOfPoints; i++) {
            buffer.append_array({
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0
            });
        }

        multiMesh->set_buffer(buffer);

        if (_definition->get_castShadow()) {
            _multiMeshInstance3D->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_ON);
        } else {
            _multiMeshInstance3D->set_cast_shadows_setting(GeometryInstance3D::ShadowCastingSetting::SHADOW_CASTING_SETTING_OFF);
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

    if (!_textureSets.is_null() && _textureSets->get_textureSets().size() > 0) {
        _foliageShader->set_shader_parameter(StringNames::Splatmaps(), _terrainZones->get_splatmapsTextures());

        TypedArray<Ref<Texture2D>> albedoTextures = TypedArray<Ref<Texture2D>>();
        for (Ref<Texture2D> albedoTexture : _textureSets->get_textureSets()) {
            albedoTextures.append(albedoTexture);
        }
        _foliageShader->set_shader_parameter(StringNames::Textures(), Utils::texturesToTextureArray(albedoTextures));
        _foliageShader->set_shader_parameter(StringNames::NumberOfTextures(), _textureSets->get_textureSets().size());
        _foliageShader->set_shader_parameter(StringNames::TextureDetail(), _textureDetail);
    }

    _foliageShader->set_shader_parameter(StringNames::FoliageTextures(), _terrainZones->get_foliagesTextures()[_foliageIndex]);
    _foliageShader->set_shader_parameter(StringNames::MeshScale(), _definition->get_meshScale());
    _foliageShader->set_shader_parameter(StringNames::WindStrength(), _definition->get_windStrength());

    _foliageShader->set_shader_parameter(StringNames::WaterTextures(), _terrainZones->get_waterTextures());
    _foliageShader->set_shader_parameter(StringNames::WaterFactor(), _waterFactor);

    Ref<Texture2D> noiseTexture = _definition->get_noiseTexture().is_null() ? _defaultNoise : _definition->get_noiseTexture();
    if (!noiseTexture.is_null()) {

        Ref<Image> noiseImage = memnew(Image);
        noiseImage->copy_from(noiseTexture->get_image());
        noiseImage->resize(_zonesSize, _zonesSize);

        _foliageShader->set_shader_parameter(StringNames::NoiseTexture(), ImageTexture::create_from_image(noiseImage));
    }
}

void Foliage::updateEditorCameraPosition(Camera3D *viewportCamera) {
    updateFoliagePosition(viewportCamera->get_global_position());
}

void Foliage::updateFoliagePosition(Vector3 position) {
    float offset = 0.0f;
    bool isEven = _zonesSize % 2 == 0;
    if (isEven) {
        offset = _definition->get_lodInitialCellWidth() / 2.0f;
    }

    int xPosition = ((int) Math::floor(position.x)) + offset;
    int zPosition = ((int) Math::floor(position.z)) + offset;

    float maxCellWidth = _definition->get_lodInitialCellWidth() * Math::pow(2.0, _definition->get_lodLevels() - 1);

    xPosition -= Math::fmod(xPosition, maxCellWidth);
    zPosition -= Math::fmod(zPosition, maxCellWidth);

    if (isEven) {
        xPosition -= _definition->get_lodInitialCellWidth() / 2.0f;
        zPosition -= _definition->get_lodInitialCellWidth() / 2.0f;
    }

    Vector3 newPosition = Vector3(xPosition, 0, zPosition);
    if (newPosition.distance_to(_lastUpdatedPosition) > maxCellWidth) {
        _foliageShader->set_shader_parameter(StringNames::GlobalPosition(), newPosition);
        _lastUpdatedPosition = newPosition;
    }
}

void Foliage::updateAABB() {
    if ((_particles == nullptr && _multiMeshInstance3D == nullptr) || _terrainZones.is_null()) {
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
    if (_definition->get_strategy() == FoliageStrategy::FOLIAGESTRATEGY_MULTIMESH) {
        _multiMeshInstance3D->set_custom_aabb(aabb);
    } else {
        _particles->set_custom_aabb(aabb);
    }
}

void Foliage::updateShaderOffsetPosition() {
    _foliageShader->set_shader_parameter(StringNames::OffsetPosition(), get_global_position());
}

