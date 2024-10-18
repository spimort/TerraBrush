using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

[Tool]
public partial class MultiMeshObjects : Node3D {
    private Texture2D _defaultNoise;
    private Dictionary<int, MeshInfo> _meshesCache = new();
    private CancellationTokenSource _objectsCreationCancellationTokenSource;

    [Export] public int ObjectsIndex { get;set; }
    [Export] public ObjectDefinitionResource Definition { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public int ZonesSize { get;set; }
    [Export] public float WaterFactor { get;set; }
    [Export] public int DefaultObjectFrequency { get;set;}
    [Export] public bool LoadInThread { get;set; }
    [Export] public Mesh TestMesh { get;set; }

    public override void _Ready() {
        base._Ready();

        if (Definition == null) {
            return;
        }

        _defaultNoise = ResourceLoader.Load<Texture2D>("res://addons/terrabrush/Resources/DefaultNoise.tres");

        for (var i = 0; i < Definition.ObjectScenes?.Length; i++) {
            var objectScene = Definition.ObjectScenes[i];
            var meshInfo = GetMeshInfoForScene(objectScene);

            if (meshInfo != null) {
                _meshesCache.Add(i, meshInfo);
            }
        }

        PopulateMultiMeshes();
    }

    private async void PopulateMultiMeshes() {
        if (LoadInThread) {
            _objectsCreationCancellationTokenSource?.Cancel();
            _objectsCreationCancellationTokenSource = new CancellationTokenSource();

            await Task.Factory.StartNew(async () => {
                await PopulateMultiMeshesAsync(_objectsCreationCancellationTokenSource.Token);
            });
        } else {
            await PopulateMultiMeshesAsync(CancellationToken.None);
        }
    }

    public async void PopulateMultiMeshesZone(ZoneResource zone) {
        if (LoadInThread) {
            _objectsCreationCancellationTokenSource?.Cancel();
            _objectsCreationCancellationTokenSource = new CancellationTokenSource();

            await Task.Factory.StartNew(async () => {
                await PopulateMultiMeshesZoneAsync(zone, _objectsCreationCancellationTokenSource.Token);
            });
        } else {
            await PopulateMultiMeshesZoneAsync(zone, CancellationToken.None);
        }
    }

    private async Task PopulateMultiMeshesAsync(CancellationToken cancellationToken) {
        if (cancellationToken.IsCancellationRequested) {
            return;
        }


        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Length; zoneIndex++) {
            if (cancellationToken.IsCancellationRequested) {
                return;
            }

            await PopulateMultiMeshesZoneAsync(TerrainZones.Zones[zoneIndex], cancellationToken);
        }
    }

    private async Task PopulateMultiMeshesZoneAsync(ZoneResource zone, CancellationToken cancellationToken) {
        if (cancellationToken.IsCancellationRequested) {
            return;
        }

        // _collisionShapesContainer?.QueueFree();
        // _collisionShapesContainer = new Node3D();
        // CallDeferred("add_child", _collisionShapesContainer);

        var resultMultiMeshNodes = _meshesCache.Keys
            .Select(key => new { Key = key, Value = new List<float>()})
            .ToDictionary(item => item.Key, item => item.Value);

        var heightmapImage = zone.HeightMapTexture.GetImage();
        var waterImage = zone.WaterTexture?.GetImage();

        if (cancellationToken.IsCancellationRequested) {
            return;
        }

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
            if (cancellationToken.IsCancellationRequested) {
                return;
            }

            for (var y = 0; y < objectsImage.GetHeight(); y++) {
                if (cancellationToken.IsCancellationRequested) {
                    return;
                }

                var objectPixel = objectsImage.GetPixel(x, y);
                CalculateObjectPresenceForPixel(
                    heightmapImage,
                    waterImage,
                    noiseImage,
                    x,
                    y,
                    objectPixel,
                    result => {
                        if (cancellationToken.IsCancellationRequested) {
                            return;
                        }

                        if (resultMultiMeshNodes.TryGetValue(result.ResultPackedSceneIndex, out var instanceInfos)) {
                            var meshInfo = _meshesCache[result.ResultPackedSceneIndex];
                            var basis = new Basis(Quaternion.FromEuler(result.ResultRotation));
                            basis = basis.Scaled(meshInfo.RootScale);
                            instanceInfos.AddRange(new float[] {
                                basis.X.X, basis.X.Y, basis.X.Z, result.ResultPosition.X,
                                basis.Y.X, basis.Y.Y, basis.Y.Z, result.ResultPosition.Y,
                                basis.Z.X, basis.Z.Y, basis.Z.Z, result.ResultPosition.Z
                            });

                            if (meshInfo.CollisionShape != null) {
                                // var collisionShape = new CollisionShape3D {
                                //     Shape = meshInfo.CollisionShape,
                                //     Position = result.ResultPosition,
                                //     RotationDegrees = result.ResultRotation
                                // };

                                // _collisionShapesContainer.CallDeferred("add_child", collisionShape);
                            }
                        }
                    }
                );
            }
        }

        var resultGodotDictionary = new Godot.Collections.Dictionary<int, float[]>();
        foreach (var keyValue in resultMultiMeshNodes) {
            resultGodotDictionary.Add(keyValue.Key, keyValue.Value.ToArray());
        }

        CallDeferred(nameof(ApplyMeshes), zone, resultGodotDictionary);
    }

    private void ApplyMeshes(ZoneResource zone, Godot.Collections.Dictionary<int, float[]> resultMeshes) {
        var zoneIndex = Array.IndexOf(TerrainZones.Zones, zone);
        var zoneNode = GetNodeOrNull<Node3D>($"{zoneIndex}");
        if (zoneNode == null) {
            zoneNode = new Node3D() {
                Name = $"{zoneIndex}",
                Position = new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize)
            };
            AddChild(zoneNode);
        }

        foreach (var child in zoneNode.GetChildren()) {
            child.QueueFree();
        }

        foreach (var keyValue in resultMeshes) {
            var multiMeshNode = new MultiMeshInstance3D {
                Name = $"{keyValue.Key}",
                Multimesh = new MultiMesh {
                    TransformFormat = MultiMesh.TransformFormatEnum.Transform3D,
                    Mesh = _meshesCache[keyValue.Key].Mesh,
                    InstanceCount = keyValue.Value.Length / 12,
                    Buffer = keyValue.Value
                }
            };
            zoneNode.AddChild(multiMeshNode);
        }
    }

    private void CalculateObjectPresenceForPixel(Image heightmapImage, Image waterImage, Image noiseImage, int x, int y, Color pixelValue, Action<(Vector3 ResultPosition, Vector3 ResultRotation, int ResultPackedSceneIndex)> objectPresentCallback, Action objectNotPresentCallback = null) {
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
                    var randomItemIndex = Utils.GetNextIntWithSeed((x * 1000) + y, 0, Definition.ObjectScenes.Count() - 1);
                    resultPosition -= new Vector3(ZonesSize / 2, -GetObjectHeight(heightmapImage, waterImage, resultImagePosition.X, resultImagePosition.Y), ZonesSize / 2);

                    objectPresentCallback(
                        (
                            resultPosition,
                            Definition.RandomYRotation ? new Vector3(0, Mathf.DegToRad(Utils.GetNextFloatWithSeed((x * 1000) + y, 0f, 360f)), 0) : Vector3.Zero,
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

    private MeshInfo GetMeshInfoForScene(PackedScene packedScene) {
        var scene = packedScene.Instantiate<Node3D>();
        var mesh = GetMeshForSceneNode(scene);
        if (mesh == null) {
            return null;
        }

        var collisionShape = GetCollisionForSceneNode(scene);

        return new MeshInfo {
            RootScale = scene.Scale,
            Mesh = mesh,
            CollisionShape = collisionShape
        };
    }

    private Mesh GetMeshForSceneNode(Node node) {
        if (node is MeshInstance3D meshInstance) {
            return meshInstance.Mesh;
        }

        foreach (var childNode in node.GetChildren()) {
            var childMesh = GetMeshForSceneNode(childNode);

            if (childMesh != null) {
                return childMesh;
            }
        }

        return null;
    }

    private Shape3D GetCollisionForSceneNode(Node node) {
        if (node is CollisionShape3D collisionShape) {
            return collisionShape.Shape;
        }

        foreach (var childNode in node.GetChildren()) {
            var childCollision = GetCollisionForSceneNode(childNode);

            if (childCollision != null) {
                return childCollision;
            }
        }

        return null;
    }

    private class MeshInfo {
        public Vector3 RootScale { get;set; }
        public Mesh Mesh { get;set; }
        public Shape3D CollisionShape { get;set;}
    }

    private class InstanceInfo {
        public Vector3 Position { get;set; }
        public Vector3 Rotation { get;set; }
    }
}

