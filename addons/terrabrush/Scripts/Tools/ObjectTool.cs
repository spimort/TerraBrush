#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class ObjectTool : ToolBase {
    private Dictionary<string, Node3D> _objectsNodeCache = null;
    private Dictionary<ZoneResource, Image> _heightmapImagesCache = null;
    private Dictionary<ZoneResource, Image> _waterImagesCache = null;

    public ObjectTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint() {
        base.BeginPaint();

        _objectsNodeCache = new Dictionary<string, Node3D>();
        _heightmapImagesCache = new Dictionary<ZoneResource, Image>();
        _waterImagesCache = new Dictionary<ZoneResource, Image>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _objectsNodeCache = null;
        _heightmapImagesCache = null;
        _waterImagesCache = null;
    }

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.ObjectsTexture[_terraBrush.ObjectIndex.Value];
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.ObjectIndex == null) {
            return;
        }

        var currentObject = _terraBrush.Objects[_terraBrush.ObjectIndex.Value];
        if (currentObject?.Definition?.ObjectScenes == null) {
            return;
        }

        var noiseTexture = currentObject.Definition?.NoiseTexture ?? _terraBrush.DefaultNoise;
        Image noiseImage = null;
        if (noiseTexture != null) {
            noiseImage = noiseTexture.GetImage();
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var zoneIndex = Array.IndexOf(_terraBrush.TerrainZones.Zones, imageZoneInfo.Zone);
            var objectsNodeName = $"{zoneIndex}_{_terraBrush.ObjectIndex.Value}";

            _objectsNodeCache.TryGetValue(objectsNodeName, out Node3D currentObjectsNode);
            if (currentObjectsNode == null) {
                currentObjectsNode = _terraBrush.ObjectsContainerNode.GetNode<Node3D>(objectsNodeName);
                if (currentObjectsNode == null) {
                    currentObjectsNode = new Node3D();
                    currentObjectsNode.Name = objectsNodeName;
                    currentObjectsNode.Visible = !currentObject.Hide;
                    currentObjectsNode.Position = new Vector3(imageZoneInfo.Zone.ZonePosition.X * _terraBrush.ZonesSize, 0, imageZoneInfo.Zone.ZonePosition.Y * _terraBrush.ZonesSize);
                    _terraBrush.ObjectsContainerNode.AddChild(currentObjectsNode);
                }

                _objectsNodeCache.Add(objectsNodeName, currentObjectsNode);
            }

            _heightmapImagesCache.TryGetValue(imageZoneInfo.Zone, out var heightmapImage);
            if (heightmapImage == null) {
                heightmapImage = imageZoneInfo.Zone.HeightMapTexture.GetImage();
                _heightmapImagesCache.Add(imageZoneInfo.Zone, heightmapImage);
            }

            Image waterImage = null;
            if (_terraBrush.WaterDefinition != null) {
                _waterImagesCache.TryGetValue(imageZoneInfo.Zone, out waterImage);
                if (waterImage == null) {
                    waterImage = imageZoneInfo.Zone.WaterTexture.GetImage();
                    _waterImagesCache.Add(imageZoneInfo.Zone, waterImage);
                }
            }

            var xPosition = imageZoneInfo.ZoneInfo.ImagePosition.X;
            var yPosition = imageZoneInfo.ZoneInfo.ImagePosition.Y;

            var currentPixel = imageZoneInfo.Image.GetPixel(xPosition, yPosition);
            var newColor = currentPixel;

            if (pixelBrushStrength > 0f) {
                var nodeName = $"{xPosition}_{yPosition}";

                if (toolType == TerrainToolType.ObjectAdd) {
                    var objectFrequency = currentObject.Definition.ObjectFrequency < 1 ? _terraBrush.DefaultObjectFrequency : currentObject.Definition.ObjectFrequency;

                    if (xPosition % objectFrequency == 0 && yPosition % objectFrequency == 0) {
                        var existingNode = currentObjectsNode.GetNodeOrNull<Node3D>(nodeName);
                        if (existingNode == null) {
                            var resultPosition = new Vector3(xPosition, 0, yPosition);
                            if (noiseImage != null) {
                                var noisePixel = noiseImage.GetPixel(xPosition, yPosition).R;
                                var randomValueX = Utils.GetNextFloatWithSeed((int) (noisePixel * 100), -currentObject.Definition.RandomRange, currentObject.Definition.RandomRange);
                                var randomValueZ = Utils.GetNextFloatWithSeed(1 + (int) (noisePixel * 100), -currentObject.Definition.RandomRange, currentObject.Definition.RandomRange);
                                resultPosition += new Vector3(randomValueX, 0, randomValueZ);
                            }

                            var resultImagePosition = new Vector2I((int) Math.Round(resultPosition.X), (int) Math.Round(resultPosition.Z));
                            if (resultImagePosition.X >= 0 && resultImagePosition.X < _terraBrush.ZonesSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < _terraBrush.ZonesSize) {
                                var heightmapPixel = heightmapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                                // Check for hole
                                if (heightmapPixel.G == 0.0) {
                                    var randomItemIndex = Utils.GetNextIntWithSeed((xPosition * 1000) + yPosition, 0, currentObject.Definition.ObjectScenes.Count() - 1);

                                    var newNode = currentObject.Definition.ObjectScenes[randomItemIndex].Instantiate<Node3D>();
                                    newNode.Name = nodeName;
                                    currentObjectsNode.AddChild(newNode);

                                    var waterHeight = waterImage?.GetPixel(resultImagePosition.X, resultImagePosition.Y).R ?? 0;
                                    resultPosition -= new Vector3(_terraBrush.ZonesSize / 2, -((heightmapPixel.R * TerraBrush.HeightMapFactor) - (waterHeight * (_terraBrush.WaterDefinition?.WaterFactor ?? 0))), _terraBrush.ZonesSize / 2);

                                    newNode.Position = resultPosition;

                                    if (currentObject.Definition.RandomYRotation) {
                                        newNode.RotationDegrees = new Vector3(newNode.RotationDegrees.X, Utils.GetNextFloatWithSeed((xPosition * 1000) + yPosition, 0f, 360f), newNode.RotationDegrees.Z);
                                    }
                                }
                            }
                        }

                        newColor = Colors.Red;
                    }
                } else {
                    var existingNode = currentObjectsNode.GetNodeOrNull<Node3D>(nodeName);
                    if (existingNode != null) {
                        existingNode.QueueFree();
                    }

                    newColor = Colors.Transparent;
                }

                imageZoneInfo.Image.SetPixel(xPosition, yPosition, newColor);
            }
        });
    }
}
#endif
