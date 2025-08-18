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
    GDCLASS(Foliage, Node3D)

private:
    Ref<ShaderMaterial> _foliageShader;
    Vector3 _lastUpdatedPosition;
    Ref<Texture2D> _defaultNoise;

    MultiMeshInstance3D *_multiMeshInstance3D;
    GPUParticles3D *_particles;

    int _foliageIndex;
    int _zonesSize;
    int _resolution;
    Ref<ZonesResource> _terrainZones;
    Ref<TextureSetsResource> _textureSets;
	int _textureDetail;
    float _waterFactor;
    Ref<FoliageDefinitionResource> _definition;

    void updateFoliage();
    void updateEditorCameraPosition(Camera3D *viewportCamera);
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
};
#endif