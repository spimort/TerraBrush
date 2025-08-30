#ifndef OBJECTS_OCTREE_MULTI_MESH_H
#define OBJECTS_OCTREE_MULTI_MESH_H

#include "../editor_resources/object_octree_lod_definition_resource.h"
#include "../editor_resources/zones_resource.h"
#include "../editor_resources/zone_resource.h"
#include "../editor_resources/object_definition_resource.h"
#include "../misc/thread_utils.h"
#include "octree/point_octree.h"
#include "objects_base.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class ObjectsOctreeNodeInfo : public RefCounted {
    GDCLASS(ObjectsOctreeNodeInfo, RefCounted);

private:
    Vector2i _imagePosition;
    Vector3 _position;
    int _meshIndex;
    Vector3 _meshRotation;
    float _meshSizeFactor;
    CollisionShape3D* _collisionShape;
    int _previousLodIndex;

protected:
    static void _bind_methods();

public:
    ObjectsOctreeNodeInfo();
    ~ObjectsOctreeNodeInfo();

    Vector2i get_imagePosition() const;
    void set_imagePosition(const Vector2i value);

    Vector3 get_position() const;
    void set_position(const Vector3 value);

    int get_meshIndex() const;
    void set_meshIndex(const int value);

    Vector3 get_meshRotation() const;
    void set_meshRotation(const Vector3 value);

    float get_meshSizeFactor() const;
    void set_meshSizeFactor(const float value);

    CollisionShape3D* get_collisionShape() const;
    void set_collisionShape(const CollisionShape3D *value);

    int get_previousLodIndex() const;
    void set_previousLodIndex(const int value);
};

class ObjectsOctreeMultiMesh : public ObjectsBase {
    GDCLASS(ObjectsOctreeMultiMesh, ObjectsBase);

private:
    const int DecimateFactor = 5;

    // Sadly, we gotta use dictionary key here instead of complexe objects (that makes it really more simple instead of creating a complexe type to hold two properties internally)
    // The original Definition of this object was :
    // internal class MultiMeshInstanceInfo {
    //     public ObjectOctreeLODMeshDefinitionResource LODMeshDefinition { get;set; }
    //     public MultiMeshInstance3D MultiMeshInstance { get;set; }
    // }
    static constexpr const char* MultiMeshInstanceInfo_LODMeshDefinitionKey = "LODMeshDefinition";
    static constexpr const char* MultiMeshInstanceInfo_MultiMeshInstanceKey = "MultiMeshInstance";

    // Sadly, we gotta use dictionary key here instead of complexe objects (that makes it really more simple instead of creating a complexe type to hold two properties internally)
    // The original Definition of this object was :
    // internal class CollisionShapeInfoInfo {
    //     public Vector3 Offset { get;set; } = Vector3.Zero;
    //     public Shape3D Shape { get;set; }
    // }
    static constexpr const char* CollisionShapeInfoInfo_OffsetKey = "Offset";
    static constexpr const char* CollisionShapeInfoInfo_ShapeKey = "Shape";

    Camera3D *_camera;
    StaticBody3D *_staticBodyContainer;
    Ref<Texture2D> _defaultNoise;
    Vector3 _lastUpdatedPosition;
    float _updateTime;
    Ref<PointOctree> _octree;
    float _maxDistance;
    TypedArray<Ref<ObjectOctreeLODDefinitionResource>> _sortedLODDefinitions;
    TypedDictionary<int, Dictionary> _collisionShapes; // Sadly, we gotta use generic Dictonary here beacuse the way Variant/Godot works. The origianl definition was : private Dictionary<int, CollisionShapeInfoInfo> _collisionShapes;
    TypedDictionary<int, Array> _multiMeshIntances; // Sadly, we gotta use generic Array/Dictionary here beacuse the way Variant/Godot works. The origianl definition was : private Dictionary<int, MultiMeshInstanceInfo[]> _multiMeshIntances;
    HashSet<Ref<ObjectsOctreeNodeInfo>> _actualNodesWithCollision;
    CancellationSource _cancellationTokenSource;
    Ref<Thread> _objectsThread;
    bool _initialized;

    void initialize();
    void initializeSortedLODs();
    void initializeMeshesAndCollision();
    void initializeOctree();
    bool sortLODs(const Ref<ObjectOctreeLODDefinitionResource> &lodA, const Ref<ObjectOctreeLODDefinitionResource> &lodB);
    Ref<ObjectOctreeLODDefinitionResource> getLODDefinitionForDistance(float distance);
    void updateMeshes();
    void updateMeshesAsync();
    void assignMultiMesheInstances(const Ref<MultiMesh> &multiMesh, const PackedFloat32Array instances);
    MeshInstance3D* getMeshForSceneNode(Node *node);
    CollisionShape3D* getCollisionForSceneNode(Node *node);
    void calculateObjectPresenceForPixel(Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage, int x, int y, Color pixelValue, bool add, bool checkExistingNode);
    Vector3 getPositionWithNoise(Ref<Image> noiseImage, int x, int y);
    bool isImagePositionInRange(float x, float y);
    float getObjectHeight(Ref<Image> heightmapImage, Ref<Image> waterImage, float imageX, float imageY);

protected:
    static void _bind_methods();
    void _notification(int what);

public:
    ObjectsOctreeMultiMesh();
    ~ObjectsOctreeMultiMesh();

    void _ready() override;
    void _physics_process(double delta) override;

    void updateMeshesFromTool();
    void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones);
    void addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage);
    Vector2 getHeightPositionForResolution(Vector2 position, int resolutionZoneSize);
};
#endif