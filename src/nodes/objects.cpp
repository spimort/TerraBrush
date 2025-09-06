#include "objects.h"
#include "../misc/zone_utils.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

void Objects::_bind_methods() {
    ClassDB::bind_method(D_METHOD("updateObjectsAsync"), &Objects::updateObjectsAsync);
    ClassDB::bind_method(D_METHOD("addObjectNode", "parentNode", "nodeName", "nodePosition", "nodeRotation", "nodeSizeFactor", "packedSceneIndex"), &Objects::addObjectNode);
}

void Objects::_notification(int what) {
    switch (what) {
        case NOTIFICATION_EXIT_TREE: {
            if (_objectsThread.is_valid()) {
                _objectsCreationCancellationTokenSource.cancel();
                _objectsThread->wait_to_finish();
            }

            _objectsThread.unref();
        } break;
    }
}

Objects::Objects() {
    _defaultObjectFrequency = 10;
}

Objects::~Objects() {}

void Objects::_ready() {
    if (_definition.is_null()) {
        return;
    }

    _defaultNoise = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/DefaultNoise.tres");

    updateObjects();
}

void Objects::updateObjects() {
    if (_loadInThread) {
        if (_objectsThread.is_valid()) {
            _objectsCreationCancellationTokenSource.cancel();
            _objectsThread->wait_to_finish();
        }

        _objectsCreationCancellationTokenSource = CancellationSource();

        _objectsThread.instantiate();
        _objectsThread->start(Callable(this, "updateObjectsAsync"));
    }
    else {
        updateObjectsAsync();
    }
}

void Objects::updateObjectsAsync() {
    CancellationToken cancellationToken = _objectsCreationCancellationTokenSource.token;

    if (cancellationToken.isCancellationRequested) {
        return;
    }

    for (int zoneIndex = 0; zoneIndex < _terrainZones->get_zones().size(); zoneIndex++) {
        if (cancellationToken.isCancellationRequested) {
            return;
        }

        Ref<ZoneResource> zone = _terrainZones->get_zones()[zoneIndex];

        Ref<Image> heightmapImage = zone->get_heightMapTexture()->get_image();
        Ref<Image> waterImage;

        if (!zone->get_waterTexture().is_null()) {
            waterImage = zone->get_waterTexture()->get_image();
        }

        if (cancellationToken.isCancellationRequested) {
            return;
        }

        Node3D *objectsContainerNode = memnew(Node3D);
        objectsContainerNode->set_name(String::num_int64(zoneIndex));
        objectsContainerNode->set_position(Vector3(zone->get_zonePosition().x * _zonesSize, 0, zone->get_zonePosition().y * _zonesSize));

        call_deferred("add_child", objectsContainerNode);

        Ref<Texture2D> imageTexture = zone->get_objectsTexture()[_objectsIndex];

        Ref<Texture2D> noiseTexture;
        if (_definition->get_noiseTexture().is_null()) {
            noiseTexture = _defaultNoise;
        } else {
            noiseTexture = _definition->get_noiseTexture();
        }

        Ref<Image> noiseImage;
        if (!noiseTexture.is_null()) {
            noiseImage = Ref<Image>(memnew(Image));
            noiseImage->copy_from(noiseTexture->get_image());
        }

        // Load all the objects from the image
        Ref<Image> objectsImage = imageTexture->get_image();

        for (int x = 0; x < objectsImage->get_width(); x++) {
            if (cancellationToken.isCancellationRequested) {
                return;
            }

            for (int y = 0; y < objectsImage->get_height(); y++) {
                if (cancellationToken.isCancellationRequested) {
                    return;
                }

                Color objectPixel = objectsImage->get_pixel(x, y);
                calculateObjectPresenceForPixel(
                    objectsContainerNode,
                    heightmapImage,
                    waterImage,
                    noiseImage,
                    x,
                    y,
                    objectPixel
                );
            }
        }
    }
}

void Objects::addObjectNode(const Node3D *parentNode, const String nodeName, const Vector3 nodePosition, const Vector3 nodeRotation, const float nodeSizeFactor, const int packedSceneIndex) {
    Ref<PackedScene> objectPackedScene = _definition->get_objectScenes()[packedSceneIndex];
    Node3D *newNode = (Node3D*)(objectPackedScene->instantiate());

    newNode->set_name(nodeName);
    newNode->set_position(nodePosition);
    newNode->set_rotation_degrees(nodeRotation);
    newNode->set_scale(newNode->get_scale() * nodeSizeFactor);

    const_cast<Node3D*>(parentNode)->add_child(newNode);
}

void Objects::calculateObjectPresenceForPixel(Node3D *parentNode, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage, int x, int y, Color pixelValue) {
    if (pixelValue.a > 0.0f) {
        int objectFrequency = _definition->get_objectFrequency() < 1 ? _defaultObjectFrequency : _definition->get_objectFrequency();
        if (x % objectFrequency != 0 || y % objectFrequency != 0) {
            return;
        }

        Vector3 resultPosition = getPositionWithNoise(noiseImage, x, y);
        if (isImagePositionInRange(resultPosition.x, resultPosition.z)) {
            int resolutionZoneSize = ZoneUtils::getImageSizeForResolution(_zonesSize, _resolution);
            Vector2 heightImagePosition = getHeightPositionForResolution(Vector2(resultPosition.x, resultPosition.z), resolutionZoneSize);
            Color heightmapPixel = Utils::getPixelLinear(heightmapImage, heightImagePosition.x, heightImagePosition.y);
            // Check for hole
            if (heightmapPixel.g == 0.0) {
                int randomItemIndex = Utils::getNextIntWithSeed((x * 1000) + y, 0, _definition->get_objectScenes().size() - 1);
                resultPosition -= Vector3(_zonesSize / 2, -getObjectHeight(heightmapImage, waterImage, heightImagePosition.x, heightImagePosition.y), _zonesSize / 2);

                Vector3 resultRotation = _definition->get_randomYRotation() ? Vector3(0, Utils::getNextFloatWithSeed((x * 1000) + y, 0, 360), 0) : Vector3(0, 0, 0);
                float resultSizeFactor = _definition->get_randomSize() ? Utils::getNextFloatWithSeed((x * 1000) + y, _definition->get_randomSizeFactorMin(), _definition->get_randomSizeFactorMax()) : 1.0f;

                call_deferred("addObjectNode", parentNode, String(String::num_int64(x) + "_" + String::num_int64(y)), resultPosition, resultRotation, resultSizeFactor, randomItemIndex);
            }
        }
    }
}

Vector3 Objects::getPositionWithNoise(Ref<Image> noiseImage, int x, int y) {
    int remapXPixel = (int) Math::round(Math::remap((float) x, 0, _zonesSize - 1, 0, noiseImage->get_width() - 1));
    int remapYPixel = (int) Math::round(Math::remap((float) y, 0, _zonesSize - 1, 0, noiseImage->get_height() - 1));

    Vector3 resultPosition = Vector3(x, 0, y);
    if (!noiseImage.is_null()) {
        float noisePixel = noiseImage->get_pixel(remapXPixel, remapYPixel).r;
        float randomValueX = Utils::getNextFloatWithSeed((int)(noisePixel * 100), -_definition->get_randomRange(), _definition->get_randomRange());
        float randomValueZ = Utils::getNextFloatWithSeed(1 + (int)(noisePixel * 100), -_definition->get_randomRange(), _definition->get_randomRange());
        resultPosition += Vector3(randomValueX, 0, randomValueZ);
    }

    return resultPosition;
}

bool Objects::isImagePositionInRange(float x, float y) {
    return x >= 0.0 && x < _zonesSize && y >= 0.0 && y < _zonesSize;
}

float Objects::getObjectHeight(Ref<Image> heightmapImage, Ref<Image> waterImage, float imageX, float imageY) {
    Color heightmapPixel = Utils::getPixelLinear(heightmapImage, imageX, imageY);
    float waterHeight = waterImage.is_null() ? 0 : Utils::getPixelLinear(waterImage, imageX, imageY).r;
    return heightmapPixel.r - (waterHeight * _waterFactor);
}

Vector2 Objects::getHeightPositionForResolution(Vector2 position, int resolutionZoneSize) {
    if (_resolution != 1) {
        return Vector2(
            Math::remap(position.x, 0, _zonesSize - 1, 0, resolutionZoneSize - 1),
            Math::remap(position.y, 0, _zonesSize - 1, 0, resolutionZoneSize - 1)
        );
    }

    return position;
}

void Objects::updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) {
    int resolutionZoneSize = ZoneUtils::getImageSizeForResolution(_zonesSize, _resolution);

    for (Ref<ZoneResource> zone : zones) {
        int zoneIndex = _terrainZones->get_zones().find(zone);
        Ref<Image> heightmapImage = zone->get_heightMapTexture()->get_image();
        Ref<Image> waterImage;
        if (!zone->get_waterTexture().is_null()) {
            waterImage = zone->get_waterTexture()->get_image();
        }

        String nodeName = String::num_int64(zoneIndex);
        Node *objectsNode = get_node_or_null(nodeName);

        if (objectsNode != nullptr) {
            Ref<Texture2D> noiseTexture = _definition->get_noiseTexture().is_null() ? _defaultNoise : _definition->get_noiseTexture();
            Ref<Image> noiseImage;
            if (!noiseTexture.is_null()) {
                noiseImage = noiseTexture->get_image();
            }

            for (int i = 0; i < objectsNode->get_child_count(); i++) {
                Node3D *objectNode = (Node3D*)(objectsNode->get_child(i));
                String objectNodeName = objectNode->get_name();
                PackedStringArray positions = objectNodeName.split("_");
                int xPosition = positions[0].to_int();
                int yPosition = positions[1].to_int();

                Vector3 resultPosition = getPositionWithNoise(noiseImage, xPosition, yPosition);
                if (isImagePositionInRange(resultPosition.x, resultPosition.z)) {
                    Vector2 heightImagePosition = getHeightPositionForResolution(Vector2(resultPosition.x, resultPosition.z), resolutionZoneSize);
                    objectNode->set_position(Vector3(objectNode->get_position().x, getObjectHeight(heightmapImage, waterImage, heightImagePosition.x, heightImagePosition.y), objectNode->get_position().z));
                }
            }
        }
    }
}

void Objects::updateMeshesFromTool() {
    // Nothing to do here, stuff has already been applied
}

void Objects::addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage) {
    int zoneIndex = _terrainZones->get_zones().find(zone);
    Node3D *containerNode = (Node3D*)(get_node_or_null(String::num_int64(zoneIndex)));
    if (containerNode == nullptr) {
        containerNode = memnew(Node3D);
        containerNode->set_name(String::num_int64(zoneIndex));
        containerNode->set_position(Vector3(zone->get_zonePosition().x * _zonesSize, 0, zone->get_zonePosition().y));
        add_child(containerNode);
    }

    String nodeName = String(String::num_int64(x) + "_" + String::num_int64(y));
    Node3D *existingNode = (Node3D*)(containerNode->get_node_or_null(nodeName));
    if (add && existingNode == nullptr) {
        calculateObjectPresenceForPixel(
            containerNode,
            heightmapImage,
            waterImage,
            noiseImage,
            x,
            y,
            Color::named("WHITE")
        );
    }
    else if (!add && existingNode != nullptr) {
        existingNode->queue_free();
    }
}
