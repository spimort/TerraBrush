#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "../editor_resources/zones_resource.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>

using namespace godot;

class Clipmap : public Node3D {
    GDCLASS(Clipmap, Node3D);

private:
    Ref<ShaderMaterial> _clipmapShader = nullptr;
    MeshInstance3D *_clipmapMesh = nullptr;
    int _zonesSize = 0;
    int _resolution = 0;
    Ref<ZonesResource> _terrainZones = nullptr;
    int _levels = 0;
    int _rowsPerLevel = 0;
    float _initialCellWidth = 0;
    Ref<ShaderMaterial> _shader = nullptr;

    Vector3 getCameraPosition();
    void updateClipmapMeshPosition(Vector3 position);
    void generateLevel(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs, TypedArray<Color> colors, int level, int rowsPerLevel, float initialCellWidth);
    void addSquareVertices(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs, float xPosition, float zPosition, float width);
    TypedArray<float> calculateTangents(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs);
    TypedArray<Vector2i> getZonePositions();
    void updateShaderOffsetPosition();

protected:
    static void _bind_methods();
    void _notification(const int what);

public:
    Clipmap();
    ~Clipmap();

    void _ready() override;
    void _process(double delta) override;

    int get_zonesSize() const;
    void set_zonesSize(const int value);

    int get_resolution() const;
    void set_resolution(const int value);

    Ref<ZonesResource> get_terrainZones() const;
    void set_terrainZones(const Ref<ZonesResource> &value);

    int get_levels() const;
    void set_levels(const int value);

    int get_rowsPerLevel() const;
    void set_rowsPerLevel(const int value);

    float get_initialCellWidth() const;
    void set_initialCellWidth(const float value);

    Ref<ShaderMaterial> get_shader() const;
    void set_shader(const Ref<ShaderMaterial> &value);

    MeshInstance3D* get_clipmapMesh() const;

    void clearMesh();
    void createMesh();
    void updateAABB();
    void updateEditorCameraPosition(Camera3D *viewportCamera);
};
#endif