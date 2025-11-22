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

class Objects : public ObjectsBase {
    GDCLASS(Objects, ObjectsBase);

private:
    Ref<Image> _noiseImageCache = nullptr;
    CancellationSource _objectsCreationCancellationTokenSource = CancellationSource();
    Ref<Thread> _objectsThread = nullptr;

    void updateObjects();
    void updateObjectsAsync();
    void addObjectNode(const Node3D *parentNode, const String nodeName, const Vector3 nodePosition, const Vector3 nodeRotation, const float nodeSizeFactor, const int packedSceneIndex);
    void calculateObjectPresenceForPixel(Node3D *parentNode, Ref<Image> heightmapImage, Ref<Image> waterImage, int x, int y, Color pixelValue);
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

    void updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) override;
    void updateMeshesFromTool() override;
    void addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage) override;
};
#endif