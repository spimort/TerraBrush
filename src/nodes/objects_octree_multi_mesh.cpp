#include "objects_octree_multi_mesh.h"
#include "../misc/zone_utils.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/mesh.hpp>

using namespace godot;

void ObjectsOctreeMultiMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("sortLODs", "lodA", "loadB"), &ObjectsOctreeMultiMesh::sortLODs);
    ClassDB::bind_method(D_METHOD("updateMeshesAsync"), &ObjectsOctreeMultiMesh::updateMeshesAsync);
    ClassDB::bind_method(D_METHOD("assignMultiMesheInstances", "multiMesh", "instances"), &ObjectsOctreeMultiMesh::assignMultiMesheInstances);
}

void ObjectsOctreeMultiMesh::_notification(int what) {
    switch (what) {
        case NOTIFICATION_EXIT_TREE: {
            if (_objectsThread.is_valid()) {
                _cancellationTokenSource.cancel();
                _objectsThread->wait_to_finish();
            }

            _objectsThread.unref();
        } break;
    }
}

ObjectsOctreeMultiMesh::ObjectsOctreeMultiMesh() {
    _lastUpdatedPosition = Vector3(0, 0, 0);
    _updateTime = 0;
    _actualNodesWithCollision = HashSet<Ref<ObjectsOctreeNodeInfo>>();
    _initialized = false;
}

ObjectsOctreeMultiMesh::~ObjectsOctreeMultiMesh() {}

void ObjectsOctreeMultiMesh::_ready() {
    if (_definition.is_null()) {
        return;
    }

    _defaultNoise = ResourceLoader::get_singleton()->load("res://addons/terrabrush/Resources/DefaultNoise.tres");

    if (Engine::get_singleton()->is_editor_hint()) {
        _camera = EditorInterface::get_singleton()->get_editor_viewport_3d()->get_camera_3d();
    }

    if (_camera == nullptr && get_viewport() != nullptr) {
        _camera = get_viewport()->get_camera_3d();
    }

    initialize();
}

void ObjectsOctreeMultiMesh::_physics_process(double delta) {
    if (!_initialized) {
        return;
    }

    _updateTime += (float) delta;
    if (_updateTime >= _definition->get_updateTimeFrequency()) {
        Vector3 currentPosition = to_local(_camera->get_global_position());
        if (currentPosition.distance_to(_lastUpdatedPosition) > _definition->get_updateDistanceThreshold()) {
            updateMeshes();

            _lastUpdatedPosition = currentPosition;
        }

        _updateTime = 0;
    }
}

void ObjectsOctreeMultiMesh::set_objectsIndex(const int value) {
    _objectsIndex = value;
}

void ObjectsOctreeMultiMesh::set_definition(const Ref<ObjectDefinitionResource> &value) {
    _definition = value;
}

void ObjectsOctreeMultiMesh::set_terrainZones(const Ref<ZonesResource> &value) {
    _terrainZones = value;
}

void ObjectsOctreeMultiMesh::set_zonesSize(const int value) {
    _zonesSize = value;
}

void ObjectsOctreeMultiMesh::set_resolution(const int value) {
    _resolution = value;
}

void ObjectsOctreeMultiMesh::set_waterFactor(const float value) {
    _waterFactor = value;
}

void ObjectsOctreeMultiMesh::set_loadInThread(const bool value) {
    _loadInThread = value;
}

void ObjectsOctreeMultiMesh::set_defaultObjectFrequency(const int value) {
    _defaultObjectFrequency = value;
}

void ObjectsOctreeMultiMesh::initialize() {
    initializeSortedLODs();
    initializeMeshesAndCollision();
    initializeOctree();
    _maxDistance = Ref<ObjectOctreeLODDefinitionResource>(_sortedLODDefinitions[_sortedLODDefinitions.size() - 1])->get_maxDistance();

    _staticBodyContainer = Object::cast_to<StaticBody3D>(get_node_or_null("StaticBody"));
    if (_staticBodyContainer != nullptr) {
        _staticBodyContainer->queue_free();
    }

    _staticBodyContainer = memnew(StaticBody3D);
    _staticBodyContainer->set_name("StaticBody");
    add_child(_staticBodyContainer);

    updateMeshes();

    _initialized = true;
}

void ObjectsOctreeMultiMesh::initializeSortedLODs() {
    if (_definition->get_lodList().size() > 0) {
        TypedArray<Ref<ObjectOctreeLODDefinitionResource>> lodList = _definition->get_lodList().duplicate();
        lodList.sort_custom(Callable(this, "sortLODs"));

        _sortedLODDefinitions = lodList;
    } else {
        _sortedLODDefinitions = TypedArray<Ref<ObjectOctreeLODDefinitionResource>>();
        Ref<ObjectOctreeLODDefinitionResource> lodDefinition = memnew(ObjectOctreeLODDefinitionResource);
        lodDefinition->set_maxDistance(_zonesSize);
        lodDefinition->set_addCollision(true);
        _sortedLODDefinitions.append(lodDefinition);
    }
}

void ObjectsOctreeMultiMesh::initializeMeshesAndCollision() {
    if (_definition->get_lodMeshes().size() > 0) {
        _multiMeshIntances = TypedDictionary<int, TypedArray<Dictionary>>();

        for (int i = 0; i < _definition->get_lodMeshes().size(); i++) {
            Ref<ObjectOctreeLODMeshesDefinitionResource> lodMeshDefinition = _definition->get_lodMeshes()[i];
            TypedArray<Dictionary> multiMeshInstances = TypedArray<Dictionary>();
            for (Ref<ObjectOctreeLODMeshDefinitionResource> lodMesh : lodMeshDefinition->get_meshes()) {
                Ref<MultiMesh> multiMesh = memnew(MultiMesh);
                multiMesh->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
                multiMesh->set_mesh(lodMesh->get_mesh());

                MultiMeshInstance3D *multiMeshInstance = memnew(MultiMeshInstance3D);
                multiMeshInstance->set_multimesh(multiMesh);
                multiMeshInstance->set_material_override(lodMesh->get_materialOverride());
                multiMeshInstance->set_layer_mask(_definition->get_visualInstanceLayers());
                multiMeshInstance->set_cast_shadows_setting(lodMesh->get_castShadow());

                add_child(multiMeshInstance);

                Dictionary multiMeshInstanceInfo = Dictionary();
                multiMeshInstanceInfo[MultiMeshInstanceInfo_LODMeshDefinitionKey] = lodMesh;
                multiMeshInstanceInfo[MultiMeshInstanceInfo_MultiMeshInstanceKey] = multiMeshInstance;
                multiMeshInstances.append(multiMeshInstanceInfo);
            }

            _multiMeshIntances[i] = multiMeshInstances;

            if (!lodMeshDefinition->get_collisionShape().is_null()) {
                if (_collisionShapes.size() == 0) {
                    _collisionShapes = TypedDictionary<int, Dictionary>();
                }

                Dictionary collisionShapeInfoInfo = Dictionary();
                collisionShapeInfoInfo[CollisionShapeInfoInfo_ShapeKey] = lodMeshDefinition->get_collisionShape();

                _collisionShapes[i] = collisionShapeInfoInfo;
            }
        }
    } else if (_definition->get_objectScenes().size() > 0) {
        for (int i = 0; i < _definition->get_objectScenes().size(); i++) {
            Ref<PackedScene> packedScene = _definition->get_objectScenes()[i];
            Node3D *scene = Object::cast_to<Node3D>(packedScene->instantiate());

            CollisionShape3D *collisionShape = getCollisionForSceneNode(scene);
            if (collisionShape != nullptr) {
                if (_collisionShapes.size() == 0) {
                    _collisionShapes = TypedDictionary<int, Dictionary>();
                }

                Dictionary collisionShapeInfoInfo = Dictionary();
                collisionShapeInfoInfo[CollisionShapeInfoInfo_ShapeKey] = collisionShape->get_shape();
                collisionShapeInfoInfo[CollisionShapeInfoInfo_OffsetKey] = collisionShape->get_position();

                _collisionShapes[i] = collisionShapeInfoInfo;
            }

            MeshInstance3D *meshInstance = getMeshForSceneNode(scene);
            if (meshInstance != nullptr) {
                if (_definition->get_lodList().size() == 0) {
                    Ref<MultiMesh> multiMesh = memnew(MultiMesh);
                    multiMesh->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
                    multiMesh->set_mesh(meshInstance->get_mesh());

                    MultiMeshInstance3D *multiMeshInstance = memnew(MultiMeshInstance3D);
                    multiMeshInstance->set_multimesh(multiMesh);
                    multiMeshInstance->set_material_override(meshInstance->get_material_override());
                    multiMeshInstance->set_layer_mask(_definition->get_visualInstanceLayers());

                    add_child(multiMeshInstance);

                    if (_multiMeshIntances.size() == 0) {
                        _multiMeshIntances = TypedDictionary<int, TypedArray<Dictionary>>();
                    }

                    // Create a "fake" ObjectOctreeLODMeshDefinitionResource with the mesh values
                    Ref<ObjectOctreeLODMeshDefinitionResource> objectOctreeLodMeshDefinition = memnew(ObjectOctreeLODMeshDefinitionResource);
                    objectOctreeLodMeshDefinition->set_mesh(meshInstance->get_mesh());
                    objectOctreeLodMeshDefinition->set_materialOverride(meshInstance->get_material_override());
                    objectOctreeLodMeshDefinition->set_scale(meshInstance->get_scale());

                    Dictionary multiMeshInstanceInfo = Dictionary();
                    multiMeshInstanceInfo[MultiMeshInstanceInfo_LODMeshDefinitionKey] = objectOctreeLodMeshDefinition;
                    multiMeshInstanceInfo[MultiMeshInstanceInfo_MultiMeshInstanceKey] = multiMeshInstance;

                    TypedArray<Dictionary> multiMeshInstances = TypedArray<Dictionary>();
                    multiMeshInstances.append(multiMeshInstanceInfo);

                    _multiMeshIntances[i] = multiMeshInstances;
                } else {
                    if (_multiMeshIntances.size() == 0) {
                        _multiMeshIntances = TypedDictionary<int, TypedArray<Dictionary>>();
                    }

                    TypedArray<Dictionary> lodMultiMeshInstances = TypedArray<Dictionary>();
                    int minimumSurfaceVerticesCount = 0;
                    for (int surfaceIndex = 0; surfaceIndex < meshInstance->get_mesh()->get_surface_count(); surfaceIndex++) {
                        Array surfaceArrays = meshInstance->get_mesh()->surface_get_arrays(surfaceIndex);
                        int surfaceVerticesCount = Array(surfaceArrays[Mesh::ArrayType::ARRAY_VERTEX]).size();

                        if (minimumSurfaceVerticesCount == 0 || surfaceVerticesCount < minimumSurfaceVerticesCount) {
                            minimumSurfaceVerticesCount = surfaceVerticesCount;
                        }
                    }

                    for (int lodIndex = 0; lodIndex < _definition->get_lodList().size(); lodIndex++) {
                        Ref<ObjectOctreeLODDefinitionResource> lodDefinition = _definition->get_lodList()[lodIndex];

                        Ref<Mesh> resultLodMesh = meshInstance->get_mesh();
                        if (lodIndex != 0 && lodDefinition->get_automaticLOD()) {
                            int targetVertices = lodDefinition->get_automaticLODTargetVertices();
                            if (targetVertices <= 0) {
                                targetVertices =  Math::max(3, minimumSurfaceVerticesCount / (lodIndex * DecimateFactor));
                            }
                            // TODO : GDExtension
                            // resultLodMesh = qem.QEMAlgorithm.GetGodotLODMesh(resultLodMesh, targetVertices, meshInstance);
                        }

                        Ref<MultiMesh> multiMesh = memnew(MultiMesh);
                        multiMesh->set_transform_format(MultiMesh::TransformFormat::TRANSFORM_3D);
                        multiMesh->set_mesh(resultLodMesh);

                        MultiMeshInstance3D *lodMultiMeshInstance = memnew(MultiMeshInstance3D);
                        lodMultiMeshInstance->set_multimesh(multiMesh);
                        lodMultiMeshInstance->set_material_override(meshInstance->get_material_override());
                        lodMultiMeshInstance->set_layer_mask(_definition->get_visualInstanceLayers());

                        add_child(lodMultiMeshInstance);

                        // Create a "fake" ObjectOctreeLODMeshDefinitionResource with the mesh values
                        Ref<ObjectOctreeLODMeshDefinitionResource> objectOctreeLodMeshDefinition = memnew(ObjectOctreeLODMeshDefinitionResource);
                        objectOctreeLodMeshDefinition->set_mesh(resultLodMesh);
                        objectOctreeLodMeshDefinition->set_materialOverride(meshInstance->get_material_override());
                        objectOctreeLodMeshDefinition->set_scale(meshInstance->get_scale());

                        Dictionary lodMultiMeshInfo = Dictionary();
                        lodMultiMeshInfo[MultiMeshInstanceInfo_LODMeshDefinitionKey] = objectOctreeLodMeshDefinition;
                        lodMultiMeshInfo[MultiMeshInstanceInfo_MultiMeshInstanceKey] = lodMultiMeshInstance;
                        lodMultiMeshInstances.append(lodMultiMeshInfo);
                    }

                    _multiMeshIntances[i] = lodMultiMeshInstances;
                }
            }
        }
    } else {
        ERR_FAIL_MSG("You must define either the ObjectScenes or the LODMeshes");
    }
}

void ObjectsOctreeMultiMesh::initializeOctree() {
    _octree = Ref<PointOctree>(memnew(PointOctree));
    _octree->init(15, Vector3(0, 0, 0), 1);

    for (int zoneIndex = 0; zoneIndex < _terrainZones->get_zones().size(); zoneIndex++) {
        Ref<ZoneResource> zone = _terrainZones->get_zones()[zoneIndex];

        Ref<Image> heightmapImage = zone->get_heightMapTexture()->get_image();
        Ref<Image> waterImage;

        if (!zone->get_waterTexture().is_null()) {
            waterImage = zone->get_waterTexture()->get_image();
        }

        Ref<ImageTexture> imageTexture = zone->get_objectsTexture()[_objectsIndex];

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
            for (int y = 0; y < objectsImage->get_height(); y++) {
                Color objectPixel = objectsImage->get_pixel(x, y);
                calculateObjectPresenceForPixel(
                    zone,
                    heightmapImage,
                    waterImage,
                    noiseImage,
                    x,
                    y,
                    objectPixel,
                    true,
                    false
                );
            }
        }
    }
}

bool sortLODs(const Ref<ObjectOctreeLODDefinitionResource> lodA, const Ref<ObjectOctreeLODDefinitionResource> lodB) {
    return lodA->get_maxDistance() < lodB->get_maxDistance();
}

void ObjectsOctreeMultiMesh::updateMeshes() {
    if (_loadInThread) {
        if (_objectsThread.is_valid()) {
            _cancellationTokenSource.cancel();
            _objectsThread->wait_to_finish();
        }

        _cancellationTokenSource = CancellationSource();

        _objectsThread.instantiate();
        _objectsThread->start(Callable(this, "updateMeshesAsync"));
    } else {
        updateMeshesAsync();
    }
}

Ref<ObjectOctreeLODDefinitionResource> ObjectsOctreeMultiMesh::getLODDefinitionForDistance(float distance) {
    for (Ref<ObjectOctreeLODDefinitionResource> lodDefinition : _sortedLODDefinitions) {
        if (lodDefinition->get_maxDistance() >= distance) {
            return lodDefinition;
        }
    }

    return nullptr;
}

void ObjectsOctreeMultiMesh::updateMeshesAsync() {
    CancellationToken cancellationToken = _cancellationTokenSource.token;

    if (cancellationToken.isCancellationRequested) return;

    if (_sortedLODDefinitions.size() == 0 || _multiMeshIntances.size() == 0) {
        return;
    }

    TypedDictionary<int, TypedDictionary<int, TypedArray<float>>> multiMeshNodes = TypedDictionary<int, TypedDictionary<int, TypedArray<float>>>();
    for (int i = 0; i < _multiMeshIntances.size(); i++) {
        int multiMeshInstanceKey = _multiMeshIntances.keys()[i];
        TypedArray<Dictionary> multiMeshInstanceInfos = _multiMeshIntances[i];

        TypedDictionary<int, TypedArray<float>> lodMultiMeshNodesBuffer = TypedDictionary<int, TypedArray<float>>();
        for (int y = 0; y < multiMeshInstanceInfos.size(); y++) {
            lodMultiMeshNodesBuffer[y] = TypedArray<float>();
        }
        multiMeshNodes[multiMeshInstanceKey] = lodMultiMeshNodesBuffer;
    }

    TypedArray<Ref<ObjectsOctreeNodeInfo>> nodes = _octree->getNearby(_lastUpdatedPosition, _maxDistance);
    TypedArray<Ref<ObjectsOctreeNodeInfo>> toRemoveNodes = TypedArray<Ref<ObjectsOctreeNodeInfo>>();
    for (Ref<ObjectsOctreeNodeInfo> nodeInfo : nodes) {
        if (cancellationToken.isCancellationRequested) return;

        float nodeDistance = nodeInfo->get_position().distance_to(_lastUpdatedPosition);
        Ref<ObjectOctreeLODDefinitionResource> lodDefinition = getLODDefinitionForDistance(nodeDistance);
        if (!lodDefinition.is_null()) {
            if (cancellationToken.isCancellationRequested) return;
            int lodDefinitionIndex = _sortedLODDefinitions.find(lodDefinition);

            // It means that the mesh for the lod level has not been provided, we skip it
            if (TypedArray<Dictionary>(_multiMeshIntances[nodeInfo->get_meshIndex()]).size() <= lodDefinitionIndex) {
                continue;
            }

            // We can have different frequency per lod level so check if we need to skip a node
            int objectFrequency = lodDefinition->get_objectFrequency() < 1 ? _definition->get_objectFrequency() < 1 ? _defaultObjectFrequency : _definition->get_objectFrequency() : lodDefinition->get_objectFrequency();
            if (nodeInfo->get_imagePosition().x % objectFrequency != 0 ||nodeInfo->get_imagePosition().y % objectFrequency != 0) {
                continue;
            }

            TypedDictionary<int, TypedArray<float>> multiMeshNodesForMeshIndex = multiMeshNodes[nodeInfo->get_meshIndex()];
            if (!multiMeshNodesForMeshIndex.has(lodDefinitionIndex)) {
                multiMeshNodesForMeshIndex[lodDefinitionIndex] = TypedArray<float>();
            }
            TypedArray<float> lodMultiMeshNodesBuffer = multiMeshNodesForMeshIndex[lodDefinitionIndex];

            Dictionary multiMeshInstanceInfo = TypedArray<Dictionary>(_multiMeshIntances[nodeInfo->get_meshIndex()])[lodDefinitionIndex];
            Ref<ObjectOctreeLODMeshDefinitionResource> lodMeshDefinition = multiMeshInstanceInfo[MultiMeshInstanceInfo_LODMeshDefinitionKey];

            Basis basis = Basis(Quaternion::from_euler(nodeInfo->get_meshRotation()));
            basis = basis.scaled(lodMeshDefinition->get_scale() * nodeInfo->get_meshSizeFactor());
            lodMultiMeshNodesBuffer.append_array(Array::make(
                basis.get_column(0).x, basis.get_column(0).y, basis.get_column(0).z, nodeInfo->get_position().x,
                basis.get_column(1).x, basis.get_column(1).y, basis.get_column(1).z, nodeInfo->get_position().y,
                basis.get_column(2).x, basis.get_column(2).y, basis.get_column(2).z, nodeInfo->get_position().z
            ));

            if (lodDefinition->get_addCollision() && (_collisionShapes.has(nodeInfo->get_meshIndex()))) {
                if (cancellationToken.isCancellationRequested) return;

                Dictionary shapeInfo = _collisionShapes[nodeInfo->get_meshIndex()];

                if (nodeInfo->get_collisionShape() == nullptr) {
                    CollisionShape3D *collisionShape = memnew(CollisionShape3D);
                    collisionShape->set_shape(shapeInfo[CollisionShapeInfoInfo_ShapeKey]);
                    collisionShape->set_position(nodeInfo->get_position() + shapeInfo[CollisionShapeInfoInfo_OffsetKey]);
                    collisionShape->set_scale(lodMeshDefinition->get_scale());
                    nodeInfo->set_collisionShape(collisionShape);
                    _staticBodyContainer->call_deferred("add_child", collisionShape);
                }

                _actualNodesWithCollision.insert(nodeInfo);
            }

            if (!lodDefinition->get_addCollision()) {
                toRemoveNodes.append(nodeInfo);
            }
        }
    }

    TypedArray<Ref<ObjectsOctreeNodeInfo>> actualNodesToRemove = TypedArray<Ref<ObjectsOctreeNodeInfo>>();
    for (Ref<ObjectsOctreeNodeInfo> actualNode : _actualNodesWithCollision) {
        if (cancellationToken.isCancellationRequested) return;

        if (!nodes.has(actualNode) || toRemoveNodes.has(actualNode)) {
            if (cancellationToken.isCancellationRequested) return;

            actualNode->get_collisionShape()->call_deferred("queue_free");
            actualNode->set_collisionShape(nullptr);

            actualNodesToRemove.append(actualNode);
        }
    }

    for (Ref<ObjectsOctreeNodeInfo> actualNodeToRemove : actualNodesToRemove) {
        _actualNodesWithCollision.erase(actualNodeToRemove);
    }
    actualNodesToRemove.clear();

    for (int meshIndex = 0; meshIndex < multiMeshNodes.size(); meshIndex++) {
        int multiMeshNodeKey = multiMeshNodes.keys()[meshIndex];
        TypedDictionary<int, TypedArray<float>> multiMeshNode = multiMeshNodes[multiMeshNodeKey];

        for (int lodIndex = 0; lodIndex < multiMeshNode.size(); lodIndex++) {
            int multiMeshNodeBufferKey = multiMeshNode.keys()[lodIndex];
            TypedArray<float> multiMeshNodeBuffer = multiMeshNode[multiMeshNodeBufferKey];

            Dictionary multiMeshInstanceInfo = TypedArray<Dictionary>(_multiMeshIntances[multiMeshNodeKey])[multiMeshNodeBufferKey];
            MultiMeshInstance3D *multiMeshInstance = Object::cast_to<MultiMeshInstance3D>(multiMeshInstanceInfo[MultiMeshInstanceInfo_MultiMeshInstanceKey]);
            if (multiMeshNodeBuffer.size() == 0) {
                multiMeshInstance->get_multimesh()->call_deferred("set_instance_count", 0);
            } else {
                call_deferred("assignMultiMesheInstances", multiMeshInstance->get_multimesh(), PackedFloat32Array(multiMeshNodeBuffer));
            }
        }
    }
}

void ObjectsOctreeMultiMesh::assignMultiMesheInstances(const Ref<MultiMesh> multiMesh, const PackedFloat32Array instances) {
    multiMesh->set_instance_count(instances.size() / 12);
    multiMesh->set_buffer(instances);
}

MeshInstance3D* ObjectsOctreeMultiMesh::getMeshForSceneNode(Node *node) {
    if (Object::cast_to<MeshInstance3D>(node) != nullptr) {
        return Object::cast_to<MeshInstance3D>(node);
    }

    for (int i = 0; node->get_child_count(); i++) {
        Node *childNode = node->get_child(i);
        MeshInstance3D *childMesh = getMeshForSceneNode(childNode);

        if (childMesh != nullptr) {
            return childMesh;
        }
    }

    return nullptr;
}

CollisionShape3D* ObjectsOctreeMultiMesh::getCollisionForSceneNode(Node *node) {
    if (Object::cast_to<CollisionShape3D>(node) != nullptr) {
        return Object::cast_to<CollisionShape3D>(node);
    }

    for (int i = 0; node->get_child_count(); i++) {
        Node *childNode = node->get_child(i);
        CollisionShape3D *childCollision = getCollisionForSceneNode(childNode);

        if (childCollision != nullptr) {
            return childCollision;
        }
    }

    return nullptr;
}

void ObjectsOctreeMultiMesh::calculateObjectPresenceForPixel(Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage, int x, int y, Color pixelValue, bool add, bool checkExistingNode) {
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
                int randomItemIndex = Utils::getNextIntWithSeed((x * 1000) + y, 0, _multiMeshIntances.size() - 1);
                resultPosition -= Vector3(_zonesSize / 2, -getObjectHeight(heightmapImage, waterImage, heightImagePosition.x, heightImagePosition.y), _zonesSize / 2);
                resultPosition += Vector3(zone->get_zonePosition().x * _zonesSize, 0, zone->get_zonePosition().y * _zonesSize);

                Vector3 resultRotation = _definition->get_randomYRotation() ? Vector3(0, Utils::getNextFloatWithSeed((x * 1000) + y, 0, 360), 0) : Vector3(0, 0, 0);
                float resultSizeFactor = _definition->get_randomSize() ? Utils::getNextFloatWithSeed((x * 1000) + y, _definition->get_randomSizeFactorMin(), _definition->get_randomSizeFactorMax()) : 1.0f;

                TypedArray<Ref<ObjectsOctreeNodeInfo>> existingNodes = checkExistingNode ? _octree->getNearby(resultPosition, 0.1f) : TypedArray<Ref<ObjectsOctreeNodeInfo>>();
                if (add && existingNodes.size() == 0) {
                    Ref<ObjectsOctreeNodeInfo> octreeNodeInfo = memnew(ObjectsOctreeNodeInfo);
                    octreeNodeInfo->set_imagePosition(Vector2i(x, y));
                    octreeNodeInfo->set_position(resultPosition);
                    octreeNodeInfo->set_meshIndex(randomItemIndex);
                    octreeNodeInfo->set_meshRotation(resultRotation);
                    octreeNodeInfo->set_meshSizeFactor(resultSizeFactor);

                    _octree->add(octreeNodeInfo, octreeNodeInfo->get_position());
                } else if (!add && existingNodes.size() == 1) {
                    _octree->remove(existingNodes[0]);
                }
            }
        }
    }
}

Vector3 ObjectsOctreeMultiMesh::getPositionWithNoise(Ref<Image> noiseImage, int x, int y) {
    int remapXPixel = (int) Math::round(Math::remap((float) x, 0, _zonesSize - 1, 0, noiseImage->get_width() - 1));
    int remapYPixel = (int) Math::round(Math::remap((float) y, 0, _zonesSize - 1, 0, noiseImage->get_height() - 1));

    Vector3 resultPosition = Vector3(x, 0, y);
    if (!noiseImage.is_null()) {
        float noisePixel = noiseImage->get_pixel(remapXPixel, remapYPixel).r;
        float randomValueX = Utils::getNextFloatWithSeed((int) (noisePixel * 100), -_definition->get_randomRange(), _definition->get_randomRange());
        float randomValueZ = Utils::getNextFloatWithSeed(1 + (int) (noisePixel * 100), -_definition->get_randomRange(), _definition->get_randomRange());
        resultPosition += Vector3(randomValueX, 0, randomValueZ);
    }

    return resultPosition;
}

bool ObjectsOctreeMultiMesh::isImagePositionInRange(float x, float y) {
    return x >= 0.0 && x < _zonesSize && y >= 0.0 && y < _zonesSize;
}

float ObjectsOctreeMultiMesh::getObjectHeight(Ref<Image> heightmapImage, Ref<Image> waterImage, float imageX, float imageY) {
    Color heightmapPixel = Utils::getPixelLinear(heightmapImage, imageX, imageY);
    float waterHeight = 0;
    if (!waterImage.is_null()) {
        waterHeight = Utils::getPixelLinear(waterImage, imageX, imageY).r;
    }

    return heightmapPixel.r - (waterHeight * _waterFactor);
}

void ObjectsOctreeMultiMesh::updateMeshesFromTool() {
    updateMeshesAsync();
}

void ObjectsOctreeMultiMesh::updateObjectsHeight(TypedArray<Ref<ZoneResource>> zones) {
    initializeOctree();
    updateMeshesAsync();
}

void ObjectsOctreeMultiMesh::addRemoveObjectFromTool(bool add, int x, int y, Ref<ZoneResource> zone, Ref<Image> heightmapImage, Ref<Image> waterImage, Ref<Image> noiseImage) {
    calculateObjectPresenceForPixel(
        zone,
        heightmapImage,
        waterImage,
        noiseImage,
        x,
        y,
        Color::named("WHITE"),
        add,
        true
    );
}

Vector2 ObjectsOctreeMultiMesh::getHeightPositionForResolution(Vector2 position, int resolutionZoneSize) {
    if (_resolution != 1) {
        return Vector2(
            Math::remap(position.x, 0, _zonesSize - 1, 0, resolutionZoneSize - 1),
            Math::remap(position.y, 0, _zonesSize - 1, 0, resolutionZoneSize - 1)
        );
    }

    return position;
}

// ObjectsOctreeNodeInfo (Octree class)

void ObjectsOctreeNodeInfo::_bind_methods() {}

ObjectsOctreeNodeInfo::ObjectsOctreeNodeInfo() {}

ObjectsOctreeNodeInfo::~ObjectsOctreeNodeInfo() {}

Vector2i ObjectsOctreeNodeInfo::get_imagePosition() const {
    return _imagePosition;
}
void ObjectsOctreeNodeInfo::set_imagePosition(const Vector2i value) {
    _imagePosition = value;
}

Vector3 ObjectsOctreeNodeInfo::get_position() const {
    return _position;
}
void ObjectsOctreeNodeInfo::set_position(const Vector3 value) {
    _position = value;
}

int ObjectsOctreeNodeInfo::get_meshIndex() const {
    return _meshIndex;
}
void ObjectsOctreeNodeInfo::set_meshIndex(const int value) {
    _meshIndex = value;
}

Vector3 ObjectsOctreeNodeInfo::get_meshRotation() const {
    return _meshRotation;
}
void ObjectsOctreeNodeInfo::set_meshRotation(const Vector3 value) {
    _meshRotation = value;
}

float ObjectsOctreeNodeInfo::get_meshSizeFactor() const {
    return _meshSizeFactor;
}
void ObjectsOctreeNodeInfo::set_meshSizeFactor(const float value) {
    _meshSizeFactor = value;
}

CollisionShape3D* ObjectsOctreeNodeInfo::get_collisionShape() const {
    return _collisionShape;
}
void ObjectsOctreeNodeInfo::set_collisionShape(const CollisionShape3D *value) {
    _collisionShape = const_cast<CollisionShape3D*>(value);
}

int ObjectsOctreeNodeInfo::get_previousLodIndex() const {
    return _previousLodIndex;
}
void ObjectsOctreeNodeInfo::set_previousLodIndex(const int value) {
    _previousLodIndex = value;
}