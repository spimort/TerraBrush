using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Objects : Node3D, IObjectsNode {
    private Texture2D _defaultNoise;
    private CancellationTokenSource _objectsCreationCancellationTokenSource;

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

        UpdateObjects();
    }

    public override void _ExitTree() {
        base._ExitTree();

        _objectsCreationCancellationTokenSource?.Cancel();
    }

    private async void UpdateObjects() {
        if (LoadInThread) {
            _objectsCreationCancellationTokenSource?.Cancel();
            _objectsCreationCancellationTokenSource = new CancellationTokenSource();

            await Task.Factory.StartNew(async () => {
                await UpdateObjectsAsync(_objectsCreationCancellationTokenSource.Token);
            }, _objectsCreationCancellationTokenSource.Token);
        } else {
            await UpdateObjectsAsync(CancellationToken.None);
        }
    }

    private async Task UpdateObjectsAsync(CancellationToken cancellationToken) {
        if (cancellationToken.IsCancellationRequested) {
            return;
        }

        for (var zoneIndex = 0; zoneIndex < TerrainZones.Zones?.Length; zoneIndex++) {
            if (cancellationToken.IsCancellationRequested) {
                return;
            }

            var zone = TerrainZones.Zones[zoneIndex];

            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            if (cancellationToken.IsCancellationRequested) {
                return;
            }

            var objectsContainerNode = new Node3D();
            objectsContainerNode.Name = $"{zoneIndex}";
            objectsContainerNode.Position = new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize);

            CallDeferred("add_child", objectsContainerNode);

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

                            CallDeferred(
                                nameof(AddObjectNode),
                                objectsContainerNode,
                                $"{x}_{y}",
                                result.ResultPosition,
                                result.ResultRotation,
                                result.ResultPackedSceneIndex
                            );
                        }
                    );
                }
            }
        }
    }

    private void AddObjectNode(Node3D parentNode, string nodeName, Vector3 nodePosition, Vector3 nodeRotation, int packedSceneIndex) {
        var newNode =  Definition.ObjectScenes[packedSceneIndex].Instantiate<Node3D>();
        newNode.Name = nodeName;
        newNode.Position = nodePosition;
        newNode.RotationDegrees = nodeRotation;

        parentNode.AddChild(newNode);
    }

    public void UpdateObjectsHeight(List<ZoneResource> zones) {
        foreach (var zone in zones) {
            var zoneIndex = Array.IndexOf(TerrainZones.Zones, zone);
            var heightmapImage = zone.HeightMapTexture.GetImage();
            var waterImage = zone.WaterTexture?.GetImage();

            var nodeName = $"{zoneIndex}";
            var objectsNode = GetNodeOrNull(nodeName);

            if (objectsNode != null) {
                var noiseTexture = Definition.NoiseTexture ?? _defaultNoise;
                Image noiseImage = null;
                if (noiseTexture != null) {
                    noiseImage = noiseTexture.GetImage();
                }

                foreach (Node3D objectNode in objectsNode.GetChildren()) {
                    var objectNodeName = objectNode.Name;
                    var positions = objectNodeName.ToString().Split("_");
                    var xPosition = int.Parse(positions[0]);
                    var yPosition = int.Parse(positions[1]);

                    var resultPosition = GetPositionWithNoise(noiseImage, xPosition, yPosition);
                    var resultImagePosition = GetImagePosition(resultPosition.X, resultPosition.Z);
                    if (IsImagePositionInRange(resultImagePosition.X, resultImagePosition.Y)) {
                        objectNode.Position = new Vector3(objectNode.Position.X, GetObjectHeight(heightmapImage, waterImage, resultImagePosition.X, resultImagePosition.Y), objectNode.Position.Z);
                    }
                }
            }
        }
    }

    public void UpdateMeshesFromTool() {
        // Nothing to do here, stuff has already been applied
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
                    var randomItemIndex = Utils.GetNextIntWithSeed((x * 1000) + y, 0, Definition.ObjectScenes.Count() - 1);
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
        var zoneIndex = Array.IndexOf(TerrainZones.Zones, zone);
        var containerNode = GetNodeOrNull($"{zoneIndex}");
        if (containerNode == null) {
            containerNode = new Node3D() {
                Name = $"{zoneIndex}",
                Position = new Vector3(zone.ZonePosition.X * ZonesSize, 0, zone.ZonePosition.Y * ZonesSize)
            };
            AddChild(containerNode);
        }

        var nodeName = $"{x}_{y}";
        var existingNode = containerNode.GetNodeOrNull(nodeName);
        if (add && existingNode == null) {
            CalculateObjectPresenceForPixel(
                heightmapImage,
                waterImage,
                noiseImage,
                x,
                y,
                Colors.White,
                result => {
                    CallDeferred(
                        nameof(AddObjectNode),
                        containerNode,
                        nodeName,
                        result.ResultPosition,
                        result.ResultRotation,
                        result.ResultPackedSceneIndex
                    );
                }
            );
        } else if (!add && existingNode != null) {
            existingNode.QueueFree();
        }
    }
}
