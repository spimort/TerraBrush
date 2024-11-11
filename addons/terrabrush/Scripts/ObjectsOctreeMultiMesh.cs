using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Threading;
using System.Threading.Tasks;
using Godot;
using TerraBrush.NetOctree;

namespace TerraBrush;

internal class OctreeNodeInfo {
    public Vector3 Position { get;set; }
    public int MeshIndex { get;set; }
    public Vector3 MeshRotation { get;set; }
    public CollisionShape3D CollisionShape { get;set; }
    public int PreviousLodIndex { get;set; }
}

internal class MultiMeshInstanceInfo {
    public ObjectOctreeLODMeshDefinitionResource LODMeshDefinition { get;set; }
    public MultiMeshInstance3D MultiMeshInstance { get;set; }
}

internal class CollisionShapeInfoInfo {
    public Vector3 Offset { get;set; } = Vector3.Zero;
    public Shape3D Shape { get;set; }
}

[Tool]
public partial class ObjectsOctreeMultiMesh : Node3D, IObjectsNode {
    private const int DecimateFactor = 5;

    private Texture2D _defaultNoise;
    private Camera3D _camera;
    private StaticBody3D _staticBodyContainer;
    private Vector3 _lastUpdatedPosition = Vector3.Zero;
    private float _updateTime = 0;
    private PointOctree<OctreeNodeInfo> _octree;
    private float _maxDistance;
    private ObjectOctreeLODDefinitionResource[] _sortedLODDefinitions;
    private Dictionary<int, CollisionShapeInfoInfo> _collisionShapes;
    private Dictionary<int, MultiMeshInstanceInfo[]> _multiMeshIntances;
    private HashSet<OctreeNodeInfo> _actualNodesWithCollision = new();
    private CancellationTokenSource _cancellationTokenSource;
    private bool _initialized = false;

    [Export] public int ObjectsIndex { get;set; }
    [Export] public ObjectDefinitionResource Definition { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public int ZonesSize { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export] public bool LoadInThread { get;set; }
    [Export] public int DefaultObjectFrequency { get;set;}

    public override void _Ready() {
        base._Ready();

        if (Definition == null) {
            return;
        }

        _defaultNoise = ResourceLoader.Load<Texture2D>("res://addons/terrabrush/Resources/DefaultNoise.tres");

#if TOOLS
        if (Engine.IsEditorHint()) {
            _camera = EditorInterface.Singleton.GetEditorViewport3D().GetCamera3D();
        }
#endif

        _camera ??= GetViewport()?.GetCamera3D();

        Initialize();
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        if (!_initialized) {
            return;
        }

        _updateTime += (float) delta;
        if (_updateTime >= Definition.UpdateTimeFrequency) {
            var currentPosition = ToLocal(_camera.GlobalPosition);
            if (currentPosition.DistanceTo(_lastUpdatedPosition) > Definition.UpdateDistanceThreshold) {
                UpdateMeshes();

                _lastUpdatedPosition = currentPosition;
            }

            _updateTime = 0;
        }
    }

    public override void _ExitTree() {
        base._ExitTree();

        _cancellationTokenSource?.Cancel();
    }

    private async void Initialize() {
        InitializeSortedLODs();
        InitializeMeshesAndCollision();
        await InitializeOctree();
        _maxDistance = _sortedLODDefinitions.Last().MaxDistance;

        _staticBodyContainer = GetNodeOrNull<StaticBody3D>("StaticBody");
		_staticBodyContainer?.QueueFree();

        _staticBodyContainer = new StaticBody3D {
            Name = "StaticBody"
        };
        AddChild(_staticBodyContainer);

        UpdateMeshes();

        _initialized = true;
    }

    private void InitializeSortedLODs() {
        if (Definition.LODList?.Length > 0) {
            _sortedLODDefinitions = Definition.LODList.ToList().OrderBy(x => x.MaxDistance).ToArray();
        } else {
            _sortedLODDefinitions = new[] {
                new ObjectOctreeLODDefinitionResource() {
                    MaxDistance = ZonesSize,
                    AddCollision = true
                }
            };
        }
    }

    private void InitializeMeshesAndCollision() {
        if (Definition.LODMeshes?.Length > 0) {
            _multiMeshIntances = new();

            for (var i = 0; i < Definition.LODMeshes.Length; i++) {
                var lodMesh = Definition.LODMeshes[i];
                var multiMeshInstances = lodMesh.Meshes.Select(x => {
                    var multiMeshInstance = new MultiMeshInstance3D() {
                        Multimesh = new MultiMesh() {
                            TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
                            Mesh = x.Mesh
                        },
                        MaterialOverride = x.MaterialOverride,
                        Layers = (uint) Definition.VisualInstanceLayers
                    };

                    AddChild(multiMeshInstance);

                    return new MultiMeshInstanceInfo {
                        LODMeshDefinition = x,
                        MultiMeshInstance = multiMeshInstance
                    };
                });
                _multiMeshIntances.Add(i, multiMeshInstances.ToArray());

                if (lodMesh.CollisionShape != null) {
                    _collisionShapes ??= new();
                    _collisionShapes.Add(i, new CollisionShapeInfoInfo {
                        Shape = lodMesh.CollisionShape
                    });
                }
            }
        } else if (Definition.ObjectScenes?.Length > 0) {
            for (var i = 0; i < Definition.ObjectScenes.Length; i++) {
                var packedScene = Definition.ObjectScenes[i];
                var scene = packedScene.Instantiate<Node3D>();

                var collisionShape = GetCollisionForSceneNode(scene);
                if (collisionShape != null) {
                    _collisionShapes ??= new();
                    _collisionShapes.Add(i, new CollisionShapeInfoInfo {
                        Shape = collisionShape.Shape,
                        Offset = collisionShape.Position
                    });
                }

                var meshInstance = GetMeshForSceneNode(scene);
                if (meshInstance != null) {
                    if ((Definition.LODList?.Length).GetValueOrDefault() == 0) {
                        var multiMeshInstance = new MultiMeshInstance3D() {
                            Multimesh = new MultiMesh() {
                                TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
                                Mesh = meshInstance.Mesh
                            },
                            MaterialOverride = meshInstance.MaterialOverride,
                            Layers = (uint) Definition.VisualInstanceLayers
                        };

                        AddChild(multiMeshInstance);

                        _multiMeshIntances ??= new();
                        _multiMeshIntances.Add(i, new[] {
                            new MultiMeshInstanceInfo {
                                LODMeshDefinition = new ObjectOctreeLODMeshDefinitionResource {
                                    Mesh = meshInstance.Mesh,
                                    MaterialOverride = meshInstance.MaterialOverride,
                                    Scale = meshInstance.Scale
                                },
                                MultiMeshInstance = multiMeshInstance
                            }
                        });
                    } else {
                        _multiMeshIntances ??= new();
                        var lodMultiMeshInstances = new List<MultiMeshInstanceInfo>();
                        var minimumSurfaceVerticesCount = 0;
                        for (int surfaceIndex = 0; surfaceIndex < meshInstance.Mesh.GetSurfaceCount(); surfaceIndex++) {
                            var surfaceArrays = meshInstance.Mesh.SurfaceGetArrays(surfaceIndex);
                            var surfaceVerticesCount = ((Godot.Collections.Array) surfaceArrays[(int) Mesh.ArrayType.Vertex]).Count;

                            if (minimumSurfaceVerticesCount == 0 || surfaceVerticesCount < minimumSurfaceVerticesCount) {
                                minimumSurfaceVerticesCount = surfaceVerticesCount;
                            }
                        }

                        for (var lodIndex = 0; lodIndex < Definition.LODList.Length; lodIndex++) {
                            var lodDefinition = Definition.LODList[lodIndex];

                            var resultLodMesh = meshInstance.Mesh;
                            if (lodIndex != 0) {
                                var targetVertices = lodDefinition.AutomaticLODTargetVertices;
                                if (targetVertices <= 0) {
                                    targetVertices =  Math.Max(3, minimumSurfaceVerticesCount / (lodIndex * DecimateFactor));
                                }
                                resultLodMesh = qem.QEMAlgorithm.GetGodotLODMesh(resultLodMesh, targetVertices, meshInstance);
                            }

                            var lodMultiMeshInstance = new MultiMeshInstance3D() {
                                Multimesh = new MultiMesh() {
                                    TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
                                    Mesh = resultLodMesh
                                },
                                MaterialOverride = meshInstance.MaterialOverride,
                                Layers = (uint) Definition.VisualInstanceLayers
                            };

                            AddChild(lodMultiMeshInstance);

                            var lodMultiMeshInfo = new MultiMeshInstanceInfo {
                                LODMeshDefinition = new ObjectOctreeLODMeshDefinitionResource {
                                    Mesh = resultLodMesh,
                                    MaterialOverride = meshInstance.MaterialOverride,
                                    Scale = meshInstance.Scale
                                },
                                MultiMeshInstance = lodMultiMeshInstance
                            };

                            lodMultiMeshInstances.Add(lodMultiMeshInfo);
                        }

                        _multiMeshIntances.Add(i, lodMultiMeshInstances.ToArray());
                    }
                }
            }
        } else {
            throw new Exception("You must define either the ObjectScenes or the LODMeshes");
        }
    }

    private async Task InitializeOctree() {
        _octree = new(15, Vector3.Zero, 1);

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Length; zoneIndex++) {
            var zone = TerrainZones.Zones[zoneIndex];

            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            var imageTexture = zone.ObjectsTexture[ObjectsIndex];

            var noiseTexture = Definition?.NoiseTexture != null ? await Utils.WaitForTextureReady(Definition.NoiseTexture) : _defaultNoise;
            Image noiseImage = null;
            if (noiseTexture != null) {
                noiseImage = new Image();
                noiseImage.CopyFrom(noiseTexture.GetImage());
            }

            // Load all the objects from the image
            var objectsImage = imageTexture.GetImage();

            for (var x = 0; x < objectsImage.GetWidth(); x++) {
                for (var y = 0; y < objectsImage.GetHeight(); y++) {
                    var objectPixel = objectsImage.GetPixel(x, y);
                    CalculateObjectPresenceForPixel(
                        heightmapImage,
                        waterImage,
                        noiseImage,
                        x,
                        y,
                        objectPixel,
                        result => {
                            var resultPosition = result.ResultPosition + new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize);
                            var octreeNodeInfo = new OctreeNodeInfo() {
                                Position = resultPosition,
                                MeshIndex = result.ResultPackedSceneIndex,
                                MeshRotation = result.ResultRotation
                            };

                            _octree.Add(octreeNodeInfo, octreeNodeInfo.Position);
                        }
                    );
                }
            }
        }
    }

    private async void UpdateMeshes() {
        if (LoadInThread) {
            _cancellationTokenSource?.Cancel();
            _cancellationTokenSource = new CancellationTokenSource();

            await Task.Factory.StartNew(() => {
                UpdateMeshesAsync(_cancellationTokenSource.Token);
            });
        } else {
            UpdateMeshesAsync(CancellationToken.None);
        }
    }

    private void UpdateMeshesAsync(CancellationToken cancellationToken) {
        if (cancellationToken.IsCancellationRequested) return;

        if (_sortedLODDefinitions == null || _multiMeshIntances == null) {
            return;
        }

        var multiMeshNodes = _multiMeshIntances.ToDictionary(
            x => x.Key,
            x => Enumerable.Range(0, _multiMeshIntances[x.Key].Length).ToDictionary(
                y => y,
                _ => new List<float>()
            )
        );

        var nodes = _octree.GetNearby(_lastUpdatedPosition, _maxDistance);
        var toRemoveNodes = new List<OctreeNodeInfo>();
        foreach (var nodeInfo in nodes) {
            if (cancellationToken.IsCancellationRequested) return;

            var nodeDistance = nodeInfo.Position.DistanceTo(_lastUpdatedPosition);
            var lodDefinition = _sortedLODDefinitions.FirstOrDefault(x => x.MaxDistance >= nodeDistance);
            if (lodDefinition != null) {
                var lodDefinitionIndex = Array.IndexOf(_sortedLODDefinitions, lodDefinition);
                if (cancellationToken.IsCancellationRequested) return;

                // It means that the mesh for the lod level has not been provided, we skip it
                if (_multiMeshIntances[nodeInfo.MeshIndex].Length <= lodDefinitionIndex) {
                    continue;
                }

                var multiMeshNodesForMeshIndex = multiMeshNodes[nodeInfo.MeshIndex];
                if (!multiMeshNodesForMeshIndex.ContainsKey(lodDefinitionIndex)) {
                    multiMeshNodesForMeshIndex.Add(lodDefinitionIndex, new List<float>());
                }
                var lodMultiMeshNodesBuffer = multiMeshNodesForMeshIndex[lodDefinitionIndex];

                var lodMeshDefinition = _multiMeshIntances[nodeInfo.MeshIndex][lodDefinitionIndex].LODMeshDefinition;

                var basis = new Basis(Quaternion.FromEuler(nodeInfo.MeshRotation));
                basis = basis.Scaled(lodMeshDefinition.Scale);
                lodMultiMeshNodesBuffer.AddRange(new float[] {
                    basis.X.X, basis.X.Y, basis.X.Z, nodeInfo.Position.X,
                    basis.Y.X, basis.Y.Y, basis.Y.Z, nodeInfo.Position.Y,
                    basis.Z.X, basis.Z.Y, basis.Z.Z, nodeInfo.Position.Z
                });

                if (lodDefinition.AddCollision && (_collisionShapes?.ContainsKey(nodeInfo.MeshIndex)).GetValueOrDefault()) {
                    if (cancellationToken.IsCancellationRequested) return;
                    var shapeInfo = _collisionShapes[nodeInfo.MeshIndex];

                    if (nodeInfo.CollisionShape == null) {
                        nodeInfo.CollisionShape = new CollisionShape3D {
                            Shape = shapeInfo.Shape,
                            Position = nodeInfo.Position + shapeInfo.Offset,
                            Scale = lodMeshDefinition.Scale
                        };
                        _staticBodyContainer.CallDeferred("add_child", nodeInfo.CollisionShape);
                    }

                    _actualNodesWithCollision.Add(nodeInfo);
                }

                if (!lodDefinition.AddCollision) {
                    toRemoveNodes.Add(nodeInfo);
                }
            }
        }

        for (var i = _actualNodesWithCollision.Count - 1; i >= 0; i--) {
            if (cancellationToken.IsCancellationRequested) return;

            var actualNode = _actualNodesWithCollision.ElementAt(i);
            if (!nodes.Contains(actualNode) || toRemoveNodes.Contains(actualNode)) {
                if (cancellationToken.IsCancellationRequested) return;

                actualNode.CollisionShape?.CallDeferred("queue_free");
                actualNode.CollisionShape = null;

                _actualNodesWithCollision.Remove(actualNode);
            }
        }

        for (var meshIndex = 0; meshIndex < multiMeshNodes.Count; meshIndex++) {
            var multiMeshNodeValuePair = multiMeshNodes.ElementAt(meshIndex);
            var multiMeshNode = multiMeshNodeValuePair.Value;

            for (var lodIndex = 0; lodIndex < multiMeshNode.Count; lodIndex++) {
                var multiMeshNodeBufferValuePair = multiMeshNode.ElementAt(lodIndex);
                var multiMeshNodeBuffer = multiMeshNodeBufferValuePair.Value;

                var multiMeshInstance = _multiMeshIntances[multiMeshNodeValuePair.Key][multiMeshNodeBufferValuePair.Key].MultiMeshInstance;
                if (multiMeshNodeBuffer.Count == 0) {
                    multiMeshInstance.Multimesh.CallDeferred("set_instance_count", 0);
                } else {
                    multiMeshInstance.Multimesh.CallDeferred("set_instance_count", multiMeshNodeBuffer.Count / 12);
                    multiMeshInstance.Multimesh.CallDeferred("set_buffer", multiMeshNodeBuffer.ToArray());
                }
            }
        }
    }

    private MeshInstance3D GetMeshForSceneNode(Node node) {
        if (node is MeshInstance3D meshInstance) {
            return meshInstance;
        }

        foreach (var childNode in node.GetChildren()) {
            var childMesh = GetMeshForSceneNode(childNode);

            if (childMesh != null) {
                return childMesh;
            }
        }

        return null;
    }

    private CollisionShape3D GetCollisionForSceneNode(Node node) {
        if (node is CollisionShape3D collisionShape) {
            return collisionShape;
        }

        foreach (var childNode in node.GetChildren()) {
            var childCollision = GetCollisionForSceneNode(childNode);

            if (childCollision != null) {
                return childCollision;
            }
        }

        return null;
    }

    public void UpdateMeshesFromTool() {
        UpdateMeshesAsync(CancellationToken.None);
    }

    public async void UpdateObjectsHeight(List<ZoneResource> zones) {
        await InitializeOctree();
        UpdateMeshesAsync(CancellationToken.None);
    }

    // TODO : Refactor this part so it shares the same code as the other strategy
    public void CalculateObjectPresenceForPixel(Image heightmapImage, Image waterImage, Image noiseImage, int x, int y, Color pixelValue, Action<(Vector3 ResultPosition, Vector3 ResultRotation, int ResultPackedSceneIndex)> objectPresentCallback, Action objectNotPresentCallback = null) {
        if (pixelValue.A > 0.0f) {
            var objectFrequency = Definition.ObjectFrequency < 1 ? DefaultObjectFrequency : Definition.ObjectFrequency;
            if (x % objectFrequency != 0 || y % objectFrequency != 0) {
                return;
            }

            var resultPosition = GetPositionWithNoise(noiseImage, x, y);
            var resultImagePosition = GetImagePosition(resultPosition.X, resultPosition.Z);
            if (IsImagePositionInRange(resultImagePosition.X, resultImagePosition.Y)) {
                var heightmapPixel = heightmapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                // Check for hole
                if (heightmapPixel.G == 0.0) {
                    var randomItemIndex = Utils.GetNextIntWithSeed((x * 1000) + y, 0, _multiMeshIntances.Count() - 1);
                    resultPosition -= new Vector3(ZonesSize / 2, -GetObjectHeight(heightmapImage, waterImage, resultImagePosition.X, resultImagePosition.Y), ZonesSize / 2);

                    objectPresentCallback(
                        (
                            resultPosition,
                            Definition.RandomYRotation ? new Vector3(0, Utils.GetNextFloatWithSeed((x * 1000) + y, 0f, 360f), 0) : Vector3.Zero,
                            randomItemIndex
                        )
                    );
                }
            }
        } else {
            objectNotPresentCallback?.Invoke();
        }
    }

    private Vector3 GetPositionWithNoise(Image noiseImage, int x, int y) {
        var resultPosition = new Vector3(x, 0, y);
        if (noiseImage != null) {
            var noisePixel = noiseImage.GetPixel(x, y).R;
            var randomValueX = Utils.GetNextFloatWithSeed((int) (noisePixel * 100), -Definition.RandomRange, Definition.RandomRange);
            var randomValueZ = Utils.GetNextFloatWithSeed(1 + (int) (noisePixel * 100), -Definition.RandomRange, Definition.RandomRange);
            resultPosition += new Vector3(randomValueX, 0, randomValueZ);
        }

        return resultPosition;
    }

    private Vector2I GetImagePosition(float x, float y) {
        return new Vector2I((int) Math.Round(x), (int) Math.Round(y));
    }

    private bool IsImagePositionInRange(int x, int y) {
        return x >= 0 && x < ZonesSize && y >= 0 && y < ZonesSize;
    }

    private float GetObjectHeight(Image heightmapImage, Image waterImage, int imageX, int imageY) {
        var heightmapPixel = heightmapImage.GetPixel(imageX, imageY);
        var waterHeight = waterImage?.GetPixel(imageX, imageY).R ?? 0;
        return heightmapPixel.R - (waterHeight * WaterFactor);
    }

    public void AddRemoveObjectFromTool(bool add, int x, int y, ZoneResource zone, Image heightmapImage, Image waterImage, Image noiseImage) {
        CalculateObjectPresenceForPixel(
            heightmapImage,
            waterImage,
            noiseImage,
            x,
            y,
            Colors.White,
            result => {
                var resultPosition = result.ResultPosition + new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize);
                var existingNodes = _octree.GetNearby(resultPosition, 0.1f);
                if (add && existingNodes.Length == 0) {
                    var octreeNodeInfo = new OctreeNodeInfo() {
                        Position = resultPosition,
                        MeshIndex = result.ResultPackedSceneIndex,
                        MeshRotation = result.ResultRotation
                    };

                    _octree.Add(octreeNodeInfo, octreeNodeInfo.Position);
                } else if (!add && existingNodes.Length == 1) {
                    _octree.Remove(existingNodes[0]);
                }
            }
        );
    }
}
