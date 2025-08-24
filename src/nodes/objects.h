#ifndef OBJECTS_H
#define OBJECTS_H

#include "objects_base.h"
#include "../editor_resources/object_definition_resource.h"
#include "../editor_resources/zones_resource.h"
#include "../misc/thread_utils.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/thread.hpp>

using namespace godot;

class Objects : public Node3D, public ObjectsBase {
    GDCLASS(Objects, Node3D)

private:
    Ref<Texture2D> _defaultNoise;
    CancellationSource _objectsCreationCancellationTokenSource;
    Ref<Thread> _objectsThread;

    int _objectsIndex;
    Ref<ObjectDefinitionResource> _definition;
    Ref<ZonesResource> _terrainZones;
    int _zonesSize;
    int _resolution;
    float _waterFactor;
    bool _loadInThread;
    int _defaultObjectFrequency;

    void updateObjects();
    void updateObjectsAsync();
    void addObjectNode(const Node3D *parentNode, const String nodeName, const Vector3 nodePosition, const Vector3 nodeRotation, const float nodeSizeFactor, const int packedSceneIndex);
    void calculateObjectPresenceForPixel(Node3D *parentNode, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage, int x, int y, Color pixelValue);
    Vector3 getPositionWithNoise(Ref<Image> noiseImage, int x, int y);
    bool isImagePositionInRange(float x, float y);
    float getObjectHeight(Ref<Image> heightmapImage, Ref<Image> waterImage, float imageX, float imageY);
    Vector2 getHeightPositionForResolution(Vector2 position, int resolutionZoneSize);

protected:
    static void _bind_methods();
    void _notification(int what);

public:
    Objects();
    ~Objects();

    void _ready() override;

    void set_objectsIndex(const int value);
    void set_definition(const Ref<ObjectDefinitionResource> &value);
    void set_terrainZones(const Ref<ZonesResource> &value);
    void set_zonesSize(const int value);
    void set_resolution(const int value);
    void set_waterFactor(const float value);
    void set_loadInThread(const bool value);
    void set_defaultObjectFrequency(const int value);

    void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) override;
    void updateMeshesFromTool() override;
    void addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage) override;
};
#endif