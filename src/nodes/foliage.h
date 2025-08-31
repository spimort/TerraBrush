#ifndef FOLIAGE_H
#define FOLIAGE_H

#include "../editor_resources/zones_resource.h"
#include "../editor_resources/texture_sets_resource.h"
#include "../editor_resources/foliage_definition_resource.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class Foliage : public Node3D {
    GDCLASS(Foliage, Node3D);

private:
    Ref<ShaderMaterial> _foliageShader = nullptr;
    Vector3 _lastUpdatedPosition = Vector3();
    Ref<Texture2D> _defaultNoise = nullptr;

    MultiMeshInstance3D *_multiMeshInstance3D = nullptr;
    GPUParticles3D *_particles = nullptr;

    int _foliageIndex = 0;
    int _zonesSize = 0;
    int _resolution = 0;
    Ref<ZonesResource> _terrainZones = nullptr;
    Ref<TextureSetsResource> _textureSets = nullptr;
	int _textureDetail = 0;
    float _waterFactor = 0;
    Ref<FoliageDefinitionResource> _definition = nullptr;

    void updateFoliage();
    void updateFoliagePosition(Vector3 position);
    void updateShaderOffsetPosition();

protected:
    static void _bind_methods();
    void _notification(const int what);

public:
    Foliage();
    ~Foliage();

    void _ready() override;
    void _process(double delta) override;

    void set_foliageIndex(const int value);
    void set_zonesSize(const int value);
    void set_resolution(const int value);
    void set_terrainZones(const Ref<ZonesResource> &value);
    void set_textureSets(const Ref<TextureSetsResource> &value);
	void set_textureDetail(const int value);
    void set_waterFactor(const float value);
    void set_definition(const Ref<FoliageDefinitionResource> &value);

    void updateAABB();
    void updateEditorCameraPosition(Camera3D *viewportCamera);
};
#endif