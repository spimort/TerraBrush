#include "clipmap.h"
#include "../misc/string_names.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void Clipmap::_bind_methods() {

}

void Clipmap::_notification(const int what) {
    if (what == NOTIFICATION_TRANSFORM_CHANGED) {
        updateShaderOffsetPosition();
    }
}

Clipmap::Clipmap() {
    _levels = 8;
    _rowsPerLevel = 21;
    _initialCellWidth = 1;
}

Clipmap::~Clipmap() {}

void Clipmap::_ready() {
    _clipmapMesh = memnew(MeshInstance3D);
    add_child(_clipmapMesh);

    set_notify_transform(true);
}

void Clipmap::_process(double delta) {
    if (!Engine::get_singleton()->is_editor_hint()) {
        Vector3 cameraPosition = getCameraPosition();
        updateClipmapMeshPosition(cameraPosition);
    }
}

int Clipmap::get_zonesSize() const {
    return _zonesSize;
}
void Clipmap::set_zonesSize(const int value) {
    _zonesSize = value;
}

int Clipmap::get_resolution() const {
    return _resolution;
}
void Clipmap::set_resolution(const int value) {
    _resolution = value;
}

Ref<ZonesResource> Clipmap::get_terrainZones() const {
    return _terrainZones;
}
void Clipmap::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

int Clipmap::get_levels() const {
    return _levels;
}
void Clipmap::set_levels(const int value) {
    _levels = value;
}

int Clipmap::get_rowsPerLevel() const {
    return _rowsPerLevel;
}
void Clipmap::set_rowsPerLevel(const int value) {
    _rowsPerLevel = value;
}

float Clipmap::get_initialCellWidth() const {
    return _initialCellWidth;
}
void Clipmap::set_initialCellWidth(const float value) {
    _initialCellWidth = value;
}

Ref<ShaderMaterial> Clipmap::get_shader() const {
    return _shader;
}
void Clipmap::set_shader(const Ref<ShaderMaterial> &value) {
    _shader = value;
}

MeshInstance3D* Clipmap::get_clipmapMesh() const {
    return _clipmapMesh;
}

Vector3 Clipmap::getCameraPosition() {
    Viewport *viewport = get_viewport();
    if (viewport == nullptr) return Vector3(0.0, 0.0, 0.0);

    Camera3D *camera = viewport->get_camera_3d();
    if (camera == nullptr) return Vector3(0.0, 0.0, 0.0);

    return camera->get_global_position();
}

void Clipmap::updateClipmapMeshPosition(Vector3 position) {
    auto offset = 0.0f;
    auto isEven = _zonesSize % 2 == 0;
    if (isEven) {
        offset = _initialCellWidth / 2.0f;
    }

    auto xPosition = ((int) Math::floor(position.x)) + offset;
    auto zPosition = ((int) Math::floor(position.z)) + offset;

    float maxCellWidth = _initialCellWidth * Math::pow(2.0, _levels - 1);

    xPosition -= Math::fmod(xPosition, maxCellWidth);
    zPosition -= Math::fmod(zPosition, maxCellWidth);

    if (isEven) {
        xPosition -= _initialCellWidth / 2.0f;
        zPosition -= _initialCellWidth / 2.0f;
    }

    auto newPosition = Vector3(xPosition, get_global_position().y, zPosition);
    if (newPosition.distance_to(_clipmapMesh->get_global_position()) > maxCellWidth) {
        _clipmapMesh->set_global_position(newPosition);
    }
}

void Clipmap::clearMesh() {
    _clipmapMesh->set_mesh(nullptr);
}

void Clipmap::createMesh() {
    Ref<ShaderMaterial> clipmapShader = _shader;
    if (clipmapShader == nullptr) {
        Ref<Shader> defaultShader = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/Shaders/clipmap_shader.gdshader");
        Ref<ShaderMaterial> defaultShaderMaterial = memnew(ShaderMaterial);
        defaultShaderMaterial->set_shader(defaultShader);
        clipmapShader = defaultShaderMaterial;
    }
    _clipmapShader = clipmapShader;
    _clipmapMesh->set_material_override(clipmapShader);

    TypedArray<Vector3> vertices = TypedArray<Vector3>();
    TypedArray<Vector2> uvs = TypedArray<Vector2>();
    TypedArray<Color> colors = TypedArray<Color>(); // To store information about the zones

    auto rowsPerLevel = _rowsPerLevel;
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    for (auto i = 0; i < _levels; i++) {
        generateLevel(vertices, uvs, colors, i + 1, rowsPerLevel, _initialCellWidth);
    }

    Array arrays = Array();
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = PackedVector3Array(vertices);
    arrays[Mesh::ARRAY_TEX_UV] = PackedVector2Array(uvs);
    arrays[Mesh::ARRAY_COLOR] = PackedColorArray(colors);

    TypedArray<Vector3> normals = TypedArray<Vector3>();
    normals.resize(vertices.size());
    normals.fill(Vector3(0, 1, 0));
    arrays[Mesh::ARRAY_NORMAL] = PackedVector3Array(normals);

    arrays[Mesh::ARRAY_TANGENT] = PackedFloat32Array(calculateTangents(vertices, uvs));

    Ref<ArrayMesh> arrayMesh = memnew(ArrayMesh);
    arrayMesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays, Array());

    _clipmapMesh->set_mesh(arrayMesh);
    updateAABB();

    clipmapShader->set_shader_parameter(StringNames::HeightmapTextures(), _terrainZones->get_heightmapTextures());
    clipmapShader->set_shader_parameter(StringNames::InitialCellWidth(), _initialCellWidth);
    clipmapShader->set_shader_parameter(StringNames::ZonesSize(), (float) _zonesSize);
    clipmapShader->set_shader_parameter(StringNames::Resolution(), (float) _resolution);
    clipmapShader->set_shader_parameter(StringNames::NumberOfZones(), (float) _terrainZones->get_zones().size());
    clipmapShader->set_shader_parameter(StringNames::ZonesMap(), _terrainZones->get_zonesMap());
    updateShaderOffsetPosition();
}

void Clipmap::generateLevel(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs, TypedArray<Color> colors, int level, int rowsPerLevel, float initialCellWidth) {
    auto width = initialCellWidth * ((float) Math::pow(2.0, level - 1));

    auto startIndex = -1 - rowsPerLevel;
    auto toIndex = 0 + rowsPerLevel;

    // The first level is different from the others because it has multiple group of 4 cells
    if (level == 1) {
        startIndex -= 2;
        toIndex += 2;
    }

    // Because the second level is next to the first one which is a strange one, we need to adjust the offset
    auto lowerOffsetIndex = 1;
    auto upperOffsetIndex = 1;
    if (level == 2) {
        lowerOffsetIndex = 3;
        upperOffsetIndex = -1;
    }

    for (auto x = startIndex; x <= toIndex; x++) {
        for (auto z = startIndex; z <= toIndex; z++) {
            if (
                level == 1 ||
                (x >= startIndex && x < -((rowsPerLevel + lowerOffsetIndex) / 2)) ||
                (x > ((rowsPerLevel - upperOffsetIndex) / 2) && x <= toIndex) ||
                (z >= startIndex && z < -((rowsPerLevel + lowerOffsetIndex) / 2)) ||
                (z > ((rowsPerLevel - upperOffsetIndex) / 2) && z <= toIndex)
            ) {
                addSquareVertices(vertices, uvs, x * width, z * width, width);

                auto vertex0MidZone = (x == startIndex && z % 2 != 0) || (z == startIndex && x % 2 != 0);
                auto vertex1MidZone = (x == toIndex && z % 2 != 0) || (z == startIndex && x % 2 == 0);
                auto vertex2MidZone = (x == startIndex && z % 2 == 0) || (z == toIndex && x % 2 != 0);

                auto vertex3MidZone = (x == toIndex && z % 2 != 0) || (z == startIndex && x % 2 == 0);
                auto vertex4MidZone = (x == toIndex && z % 2 == 0) || (z == toIndex && x % 2 == 0);
                auto vertex5MidZone = (x == startIndex && z % 2 == 0) || (z == toIndex && x % 2 != 0);

                colors.append(Color(vertex0MidZone ? 1 : 0, vertex0MidZone && z == startIndex ? 1 : 0, vertex0MidZone && x == startIndex ? 1 : 0, level / 100.0f));
                colors.append(Color(vertex1MidZone ? 1 : 0, vertex1MidZone && z == startIndex ? 1 : 0, vertex1MidZone && x == toIndex ? 1 : 0, level / 100.0f));
                colors.append(Color(vertex2MidZone ? 1 : 0, vertex2MidZone && z == toIndex ? 1 : 0, vertex2MidZone && x == startIndex ? 1 : 0, level / 100.0f));

                colors.append(Color(vertex3MidZone ? 1 : 0, vertex3MidZone && z == startIndex ? 1 : 0, vertex3MidZone && x == toIndex ? 1 : 0, level / 100.0f));
                colors.append(Color(vertex4MidZone ? 1 : 0, vertex4MidZone && z == toIndex ? 1 : 0, vertex4MidZone && x == toIndex ? 1 : 0, level / 100.0f));
                colors.append(Color(vertex5MidZone ? 1 : 0, vertex5MidZone && z == toIndex ? 1 : 0, vertex5MidZone && x == startIndex ? 1 : 0, level / 100.0f));
            }
        }
    }
}

void Clipmap::addSquareVertices(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs, float xPosition, float zPosition, float width) {
    /* Square made of 2 triangles
        2  #  #  #
        |  \  #  #
        |  #  \  #
        0  -  -  1

        5  -  -  4
        #  \  #  |
        #  #  \  |
        #  #  #  3
    */

    // Vertices
    vertices.append(Vector3(xPosition, 0, zPosition));
    vertices.append(Vector3(xPosition + width, 0, zPosition));
    vertices.append(Vector3(xPosition, 0, zPosition + width));

    vertices.append(Vector3(xPosition + width, 0, zPosition));
    vertices.append(Vector3(xPosition + width, 0, zPosition + width));
    vertices.append(Vector3(xPosition, 0, zPosition + width));

    // UVS
    uvs.append(Vector2(0, 1));
    uvs.append(Vector2(1, 1));
    uvs.append(Vector2(0, 0));

    uvs.append(Vector2(1, 1));
    uvs.append(Vector2(1, 0));
    uvs.append(Vector2(0, 0));
}

TypedArray<float> Clipmap::calculateTangents(TypedArray<Vector3> vertices, TypedArray<Vector2> uvs) {
    TypedArray<float> tangents = TypedArray<float>();
    int triangleCount = vertices.size() / 3;

    for (int i = 0; i < triangleCount; i++) {
        Vector3 v0 = vertices[i * 3];
        Vector3 v1 = vertices[i * 3 + 1];
        Vector3 v2 = vertices[i * 3 + 2];

        Vector2 uv0 = uvs[i * 3];
        Vector2 uv1 = uvs[i * 3 + 1];
        Vector2 uv2 = uvs[i * 3 + 2];

        // Calculate the edges
        auto edge1 = v1 - v0;
        auto edge2 = v2 - v0;

        // Calculate the UV space edges
        auto deltaUV1 = uv1 - uv0;
        auto deltaUV2 = uv2 - uv0;

        // Calculate tangent
        auto inversedDeterminant = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        auto tangent = ((edge1 * deltaUV2.y) - (edge2 * deltaUV1.y)) * inversedDeterminant;
        tangent = tangent.normalized();

        // Create tangent for each vertex of the triangle
        tangents.append(tangent.x);
        tangents.append(tangent.y);
        tangents.append(tangent.z);
        tangents.append(1.0f); // W is for a positive sign (1.0)

        tangents.append(tangent.x);
        tangents.append(tangent.y);
        tangents.append(tangent.z);
        tangents.append(1.0f);

        tangents.append(tangent.x);
        tangents.append(tangent.y);
        tangents.append(tangent.z);
        tangents.append(1.0f);
    }

    return tangents;
}

TypedArray<Vector2i> Clipmap::getZonePositions() {
    TypedArray<Vector2i> result = TypedArray<Vector2i>();

    TypedArray<Ref<ZoneResource>> zones = _terrainZones->get_zones();
    for (auto i = 0; i < zones.size(); i++) {
        Ref<ZoneResource> zone = zones[i];
        result.append(zone->get_zonePosition());
    }

    return result;
}

void Clipmap::updateAABB() {
    auto maxX = 0;
    auto maxY = 0;

    auto zonePositions = getZonePositions();
    for (int i = 0; i < zonePositions.size(); i++) {
        Vector2i zonePosition = zonePositions[i];
        maxX = Math::max(maxX, zonePosition.x);
        maxY = Math::max(maxY, zonePosition.y);
    }

    auto aabbXSize = Math::max(maxX * _zonesSize * 2, _zonesSize * 2);
    auto aabbYSize = Math::max(maxY * _zonesSize * 2, _zonesSize * 2);
    auto aabbXPoint = -(aabbXSize / 2);
    auto aabbYPoint = -(aabbYSize / 2);
    ((Ref<ArrayMesh>) _clipmapMesh->get_mesh())->set_custom_aabb(AABB(Vector3(aabbXPoint, Math::max(aabbXPoint, aabbYPoint), aabbYPoint), Vector3(aabbXSize, Math::max(aabbXSize, aabbYSize), aabbYSize)));
}

void Clipmap::updateShaderOffsetPosition() {
    _clipmapShader->set_shader_parameter(StringNames::OffsetPosition(), get_global_position());
}