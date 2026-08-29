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
    _meshesContainer = memnew(Node3D);
    add_child(_meshesContainer);

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

bool Clipmap::get_chunkMesh() const {
    return _chunkMesh;
}
void Clipmap::set_chunkMesh(const bool value) {
    _chunkMesh = value;
}

int Clipmap::get_chunkAABBHeight() const {
    return _chunkAABBHeight;
}
void Clipmap::set_chunkAABBHeight(const int value) {
    _chunkAABBHeight = value;
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

int Clipmap::get_visualInstanceLayers() const {
    return _visualInstanceLayers;
}
void Clipmap::set_visualInstanceLayers(const int value) {
    _visualInstanceLayers = value;
}

Ref<ShaderMaterial> Clipmap::get_shader() const {
    return _shader;
}
void Clipmap::set_shader(const Ref<ShaderMaterial> &value) {
    _shader = value;
}

Vector3 Clipmap::getCameraPosition() {
    Viewport *viewport = get_viewport();
    if (viewport == nullptr) return Vector3(0.0, 0.0, 0.0);

    Camera3D *camera = viewport->get_camera_3d();
    if (camera == nullptr) return Vector3(0.0, 0.0, 0.0);

    return camera->get_global_position();
}

void Clipmap::updateEditorCameraPosition(Camera3D *viewportCamera) {
    updateClipmapMeshPosition(viewportCamera->get_global_position());
}

void Clipmap::updateClipmapMeshPosition(Vector3 position) {
    float offset = 0.0f;
    bool isEven = _zonesSize % 2 == 0;
    if (isEven) {
        offset = _initialCellWidth / 2.0f;
    }

    float xPosition = ((int) Math::floor(position.x)) + offset;
    float zPosition = ((int) Math::floor(position.z)) + offset;

    Vector3 newPosition = Vector3(xPosition, get_global_position().y, zPosition);
    _meshesContainer->set_global_position(newPosition);


    // float maxCellWidth = _initialCellWidth * Math::pow(2.0, _levels - 1);

    // xPosition -= Math::fmod(xPosition, maxCellWidth);
    // zPosition -= Math::fmod(zPosition, maxCellWidth);

    // if (isEven) {
    //     xPosition -= _initialCellWidth / 2.0f;
    //     zPosition -= _initialCellWidth / 2.0f;
    // }

    // Vector3 newPosition = Vector3(xPosition, get_global_position().y, zPosition);
    // if (newPosition.distance_to(_meshesContainer->get_global_position()) > maxCellWidth) {
    //     _meshesContainer->set_global_position(newPosition);
    // }
}

void Clipmap::clearMesh() {
    for (int i = 0; i < _meshesContainer->get_child_count(); i++) {
        _meshesContainer->get_child(i)->queue_free();
    }
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

    _clipmapShader->set_shader_parameter(StringNames::HeightmapTextures(), _terrainZones->get_heightmapTextures());
    _clipmapShader->set_shader_parameter(StringNames::InitialCellWidth(), _initialCellWidth);
    _clipmapShader->set_shader_parameter(StringNames::ZonesSize(), (float) _zonesSize);
    _clipmapShader->set_shader_parameter(StringNames::Resolution(), (float) _resolution);
    _clipmapShader->set_shader_parameter(StringNames::NumberOfZones(), (float) _terrainZones->get_zones().size());
    _clipmapShader->set_shader_parameter(StringNames::ZonesMap(), _terrainZones->get_zonesMap());
    updateShaderOffsetPosition();

    if (_chunkMesh) {
        createMeshChunks();
    } else {
        generateFullMesh();
    }
}

void Clipmap::createMeshChunks() {
    for (int i = 0; i < _levels; i++) {
        for (int x = MinChunkPosition; x <= MaxChunkPosition; x++) {
            for (int z = MinChunkPosition; z <= MaxChunkPosition; z++) {
                // For the first level, we create the center of the mesh as well, while being chunked
                if (
                    i == 0 || (
                        x == MinChunkPosition ||
                        z == MinChunkPosition ||
                        x == MaxChunkPosition ||
                        z == MaxChunkPosition
                    )
                ) {
                    createMeshChunk(i + 1, Vector2(x, z));
                }
            }
        }
    }
}

void Clipmap::createMeshChunk(int level, Vector2 position) {
    TypedArray<Vector3> vertices = TypedArray<Vector3>();
    TypedArray<Vector2> uvs = TypedArray<Vector2>();
    TypedArray<Color> colors = TypedArray<Color>(); // To store information about the zones
    TypedArray<float> custom0 = TypedArray<float>();

    auto rowsPerLevel = _rowsPerLevel;
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    Vector2 numberOfCellsAndWidth = generateChunkedLevel(vertices, uvs, colors, custom0, level, rowsPerLevel, _initialCellWidth, position);
    Vector2 resultPosition = position * Vector2(numberOfCellsAndWidth.x * numberOfCellsAndWidth.y, numberOfCellsAndWidth.x * numberOfCellsAndWidth.y);

    Ref<ArrayMesh> arrayMesh = generateArrayMesh(vertices, uvs, colors, custom0);

    MeshInstance3D *chunkMesh = memnew(MeshInstance3D);
    chunkMesh->set_mesh(arrayMesh);
    chunkMesh->set_layer_mask(_visualInstanceLayers);
    chunkMesh->set_material_override(_clipmapShader);
    chunkMesh->set_position(Vector3(resultPosition.x, 0, resultPosition.y));
    _meshesContainer->add_child(chunkMesh);

    AABB customAABB = chunkMesh->get_aabb();
    customAABB.set_size(Vector3(customAABB.get_size().x, _chunkAABBHeight == -1 ? _zonesSize : _chunkAABBHeight, customAABB.get_size().z));
    chunkMesh->set_custom_aabb(customAABB);
}

Vector2 Clipmap::generateChunkedLevel(TypedArray<Vector3> &vertices, TypedArray<Vector2> &uvs, TypedArray<Color> &colors, TypedArray<float> &custom0, int level, int rowsPerLevel, float initialCellWidth, Vector2 chunkPosition) {
    auto width = initialCellWidth * ((float) Math::pow(2.0, level - 1));

    int numberOfCells = Math::floor(rowsPerLevel / 2.0) + 1;
    int minCellValue = MinChunkPosition * numberOfCells;
    int maxCellValue = ((MaxChunkPosition + 1) * numberOfCells) - 1;

    for (int x = 0; x < numberOfCells; x++) {
        for (int z = 0; z < numberOfCells; z++) {
            addSquareVertices(vertices, uvs, x * width, z * width, width);

            int globalXCellPosition = (chunkPosition.x * numberOfCells) + x;
            int globalZCellPosition = (chunkPosition.y * numberOfCells) + z;

            if (globalXCellPosition > minCellValue && globalZCellPosition > minCellValue) {
                generateLevelEdges(colors, level, minCellValue + 1, maxCellValue, globalXCellPosition, globalZCellPosition);
            } else {
                generateLevelEdges(colors, level, minCellValue, maxCellValue, globalXCellPosition, globalZCellPosition);
            }

            bool isEdge = false;
            if (chunkPosition.x == MinChunkPosition && x == 0 || chunkPosition.y == MinChunkPosition && z == 0) {
                isEdge = true;
            }

            // Add the extra row/column info (these are not actual extra row/column but they are in the edges so they will be hidden in some situations), the first cell is considered both horizontal and vertical
            addCustom0CellData(custom0, isEdge, chunkPosition.y == MinChunkPosition && z == 0, chunkPosition.x == MinChunkPosition && x == 0, Vector2i(-1, -1));
        }
    }

    // Add an extra row/column to smooth the terrain transition
    if (chunkPosition.x == MaxChunkPosition) {
        for (int z = 0; z < numberOfCells; z++) {
            int globalZCellPosition = (chunkPosition.y * numberOfCells) + z;

            addSquareVertices(vertices, uvs, numberOfCells * width, z * width, width);
            // The -10000 here is only so we font find a corresponding edge here with the minimum value (we are already on the maximum row/column, so no need to compute the minimumValue)
            generateLevelEdges(colors, level, -10000, maxCellValue, maxCellValue, globalZCellPosition);

            // Add the extra column (vertical), and the first is also considered as horizontal
            addCustom0CellData(custom0, true, chunkPosition.y == MinChunkPosition && z == 0, true, Vector2i(1, -1));
        }
    }

    if (chunkPosition.y == MaxChunkPosition) {
        // Add one more to fill the join between the X and Z axis
        for (int x = 0; x < numberOfCells + 1; x++) {
            int globalXCellPosition = (chunkPosition.x * numberOfCells) + x;

            addSquareVertices(vertices, uvs, x * width, numberOfCells * width, width);
            // The -10000 here is only so we font find a corresponding edge here with the minimum value (we are already on the maximum row/column, so no need to compute the minimumValue)
            generateLevelEdges(colors, level, -10000, maxCellValue + 1, globalXCellPosition, maxCellValue + 1);

            // Last cell should be aligned with bottom right corner
            Vector2i edgeDirection = Vector2i(-1, 1);
            if (chunkPosition.x == MaxChunkPosition && x == numberOfCells) {
                edgeDirection.x = 1;
            }

            // Add the extra row (horizontal), and the first and the last cell are also considered as vertical
            addCustom0CellData(custom0, true, true, (chunkPosition.x == MinChunkPosition && x == 0) || (chunkPosition.x == MaxChunkPosition && x == numberOfCells), edgeDirection);
        }
    }

    return Vector2(numberOfCells, width);
}

void Clipmap::generateFullMesh() {
    MeshInstance3D *clipmapMesh = memnew(MeshInstance3D);
    _meshesContainer->add_child(clipmapMesh);
    clipmapMesh->set_layer_mask(_visualInstanceLayers);
    clipmapMesh->set_material_override(_clipmapShader);

    TypedArray<Vector3> vertices = TypedArray<Vector3>();
    TypedArray<Vector2> uvs = TypedArray<Vector2>();
    TypedArray<Color> colors = TypedArray<Color>(); // To store information about the zones
    TypedArray<float> custom0 = TypedArray<float>();

    auto rowsPerLevel = _rowsPerLevel;
    if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
        rowsPerLevel += 1;
    }

    for (auto i = 0; i < _levels; i++) {
        generateLevel(vertices, uvs, colors, i + 1, rowsPerLevel, _initialCellWidth);
    }

    Ref<ArrayMesh> arrayMesh = generateArrayMesh(vertices, uvs, colors, custom0);

    clipmapMesh->set_mesh(arrayMesh);
    updateAABB();
}

void Clipmap::generateLevel(TypedArray<Vector3> &vertices, TypedArray<Vector2> &uvs, TypedArray<Color> &colors, int level, int rowsPerLevel, float initialCellWidth) {
    auto width = initialCellWidth * ((float) Math::pow(2.0, level - 1));

    auto startIndex = -1 - rowsPerLevel;
    auto toIndex = 0 + rowsPerLevel;

    auto lowerOffsetIndex = 1;
    auto upperOffsetIndex = 1;

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
                generateLevelEdges(colors, level, startIndex, toIndex, x, z);
            }
        }
    }
}

Ref<ArrayMesh> Clipmap::generateArrayMesh(TypedArray<Vector3> &vertices, TypedArray<Vector2> &uvs, TypedArray<Color> &colors, TypedArray<float> &custom0) {
    Array arrays = Array();
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = PackedVector3Array(vertices);
    arrays[Mesh::ARRAY_TEX_UV] = PackedVector2Array(uvs);
    arrays[Mesh::ARRAY_COLOR] = PackedColorArray(colors);
    arrays[Mesh::ARRAY_CUSTOM0] = PackedFloat32Array(custom0);

    TypedArray<Vector3> normals = TypedArray<Vector3>();
    normals.resize(vertices.size());
    normals.fill(Vector3(0, 1, 0));
    arrays[Mesh::ARRAY_NORMAL] = PackedVector3Array(normals);

    arrays[Mesh::ARRAY_TANGENT] = PackedFloat32Array(calculateTangents(vertices, uvs));

    Ref<ArrayMesh> arrayMesh = memnew(ArrayMesh);
    arrayMesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays, Array(), Dictionary(),  Mesh::ARRAY_FORMAT_CUSTOM0 | (Mesh::ARRAY_CUSTOM_RGBA_FLOAT << Mesh::ARRAY_FORMAT_CUSTOM0_SHIFT));

    return arrayMesh;
}

void Clipmap::generateLevelEdges(TypedArray<Color> &colors, int level, int startIndex, int toIndex, int x, int z) {
    float colorLevel = level / 100.0f;

    auto vertex0MidZone = (x == startIndex) || (z == startIndex);
    auto vertex1MidZone = (x == toIndex) || (z == startIndex);
    auto vertex2MidZone = (x == startIndex) || (z == toIndex);

    auto vertex3MidZone = (x == toIndex) || (z == startIndex);
    auto vertex4MidZone = (x == toIndex) || (z == toIndex);
    auto vertex5MidZone = (x == startIndex) || (z == toIndex);

    colors.append(Color(0, vertex0MidZone && z == startIndex ? 1 : 0, vertex0MidZone && x == startIndex ? 1 : 0, colorLevel));
    colors.append(Color(0, vertex1MidZone && z == startIndex ? 1 : 0, vertex1MidZone && x == toIndex ? 1 : 0, colorLevel));
    colors.append(Color(0, vertex2MidZone && z == toIndex ? 1 : 0, vertex2MidZone && x == startIndex ? 1 : 0, colorLevel));

    colors.append(Color(0, vertex3MidZone && z == startIndex ? 1 : 0, vertex3MidZone && x == toIndex ? 1 : 0, colorLevel));
    colors.append(Color(0, vertex4MidZone && z == toIndex ? 1 : 0, vertex4MidZone && x == toIndex ? 1 : 0, colorLevel));
    colors.append(Color(0, vertex5MidZone && z == toIndex ? 1 : 0, vertex5MidZone && x == startIndex ? 1 : 0, colorLevel));
}

void Clipmap::addSquareVertices(TypedArray<Vector3> &vertices, TypedArray<Vector2> &uvs, float xPosition, float zPosition, float width) {
    /* Square made of 2 triangles
        0  -  -  1
        |     /  |
        |  /     |
        2  -  -  -

        -  -  -  3
        |     /  |
        |  /     |
        5  -  -  4
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

TypedArray<float> Clipmap::calculateTangents(TypedArray<Vector3> &vertices, TypedArray<Vector2> &uvs) {
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
    if (_chunkMesh) {
        return;
    }

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
    ((Ref<ArrayMesh>) ((MeshInstance3D*)_meshesContainer->get_child(0))->get_mesh())->set_custom_aabb(AABB(Vector3(aabbXPoint, Math::max(aabbXPoint, aabbYPoint), aabbYPoint), Vector3(aabbXSize, Math::max(aabbXSize, aabbYSize), aabbYSize)));
}

void Clipmap::updateShaderOffsetPosition() {
    if (!_clipmapShader.is_null()) {
        _clipmapShader->set_shader_parameter(StringNames::OffsetPosition(), get_global_position());
    }
}

// This function is to "anchor" vertices at corners so its easier to find if a vertice belongs to a cell
// This is then used in the shader to check if we are in an extra row/column AND if we need to hide it or not, according to the cell corner position
// We use the 4 Channels available in the Custom0 this way :
// - R : Are we in a horizontal extra row/column situation, if so, 1
// - G : Are we in a vertical extra row/column situation, if so, 1
// - B : The direction we need to move the vertice in the X axis to reach the target corner (in the shader, this is multiplied by the cell size)
// - A : The direction we need to move the vertice in the Z axis to reach the target corner (in the shader, this is multiplied by the cell size)
// G and R can be both 1 at the some time if we are in a situation where we are in a corner cell and the cell if both at the same time
void Clipmap::addCustom0CellData(TypedArray<float> &custom0, bool isEdge, bool isHorizontal, bool isVertical, Vector2i edgeCellDirection) {
    if (isEdge) {
        // Edge is based on the 2 triangles that we make in the addSquareVertices function :
        // Just a reminder here to make it easier to investigate
        /* Square made of 2 triangles
            0  -  -  1
            |     /  |
            |  /     |
            2  -  -  -

            -  -  -  3
            |     /  |
            |  /     |
            5  -  -  4
        */
        // The number correspond to the line index

        // Align to top left
        if (edgeCellDirection.x == -1 && edgeCellDirection.y == -1) {
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, -1));

            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, -1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, -1));
        }

        // Align to top right
        if (edgeCellDirection.x == 1 && edgeCellDirection.y == -1) {
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, -1));

            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, -1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, -1));
        }

        // Align to bottom left
        if (edgeCellDirection.x == -1 && edgeCellDirection.y == 1) {
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));

            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, -1, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));
        }

        // Align to bottom right
        if (edgeCellDirection.x == 1 && edgeCellDirection.y == 1) {
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, 0));

            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 1));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 0, 0));
            custom0.append_array(Array::make(isHorizontal ? 1 : 0, isVertical ? 1 : 0, 1, 0));
        }
    } else {
        custom0.append_array(Array::make(0, 0, 0, 0));
        custom0.append_array(Array::make(0, 0, 0, 0));
        custom0.append_array(Array::make(0, 0, 0, 0));

        custom0.append_array(Array::make(0, 0, 0, 0));
        custom0.append_array(Array::make(0, 0, 0, 0));
        custom0.append_array(Array::make(0, 0, 0, 0));
    }
}
