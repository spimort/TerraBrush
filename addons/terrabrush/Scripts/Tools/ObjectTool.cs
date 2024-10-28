#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class ObjectTool : ToolBase {
    private HashSet<ZoneResource> _sculptedZones;
    private Dictionary<ZoneResource, Image> _heightmapImagesCache = null;
    private Dictionary<ZoneResource, Image> _waterImagesCache = null;
    private PackedScene _objectItemPackedScene;
    private PackedScene _objectsZonePackedScene;

    public ObjectTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint() {
        base.BeginPaint();

        _heightmapImagesCache = new Dictionary<ZoneResource, Image>();
        _waterImagesCache = new Dictionary<ZoneResource, Image>();
        _objectsZonePackedScene = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ObjectsZone.tscn");
        _objectItemPackedScene = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ObjectItem.tscn");

        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _heightmapImagesCache = null;
        _waterImagesCache = null;

        if (_terraBrush.ObjectIndex.HasValue) {
            _terraBrush.UpdateZoneObjects(_terraBrush.ObjectIndex.Value, _sculptedZones.ToList());
        }

        _sculptedZones = null;
    }

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.ObjectsTexture[_terraBrush.ObjectIndex.Value];
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.ObjectIndex == null) {
            return;
        }

        var currentObject = _terraBrush.Objects[_terraBrush.ObjectIndex.Value];
        if (currentObject?.Definition?.ObjectScenes == null || currentObject.Hide) {
            return;
        }

        var noiseTexture = currentObject.Definition?.NoiseTexture ?? _terraBrush.DefaultNoise;
        Image noiseImage = null;
        if (noiseTexture != null) {
            noiseImage = noiseTexture.GetImage();
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            // var zoneIndex = Array.IndexOf(_terraBrush.TerrainZones.Zones, imageZoneInfo.Zone);
            // var objectsAndZoneNodeName = $"{_terraBrush.ObjectIndex.Value}_{zoneIndex}";

            // _objectsNodeCache.TryGetValue(objectsAndZoneNodeName, out ObjectsZone currentObjectsNode);
            // if (currentObjectsNode == null) {
            //     var objectsNode = _terraBrush.ObjectsContainerNode.GetNodeOrNull<Node3D>($"{_terraBrush.ObjectIndex.Value}");
            //     if (objectsNode == null) {
            //         objectsNode = new Node3D {
            //             Name = $"{_terraBrush.ObjectIndex.Value}"
            //         };
            //         _terraBrush.ObjectsContainerNode.AddChild(objectsNode);
            //     }

            //     currentObjectsNode = objectsNode.GetNodeOrNull<ObjectsZone>($"{zoneIndex}");
            //     if (currentObjectsNode == null) {
            //         currentObjectsNode = _objectsZonePackedScene.Instantiate<ObjectsZone>();

            //         currentObjectsNode.Name = $"{zoneIndex}";
            //         currentObjectsNode.Position = new Vector3(imageZoneInfo.Zone.ZonePosition.X * _terraBrush.ZonesSize, 0, imageZoneInfo.Zone.ZonePosition.Y * _terraBrush.ZonesSize);
            //         currentObjectsNode.MaximumDistance = currentObject.Definition.MaximumDistance;
            //         currentObjectsNode.ZonesSize = _terraBrush.ZonesSize;
            //         objectsNode.AddChild(currentObjectsNode);
            //     }

            //     _objectsNodeCache.Add(objectsAndZoneNodeName, currentObjectsNode);
            // }

            // _heightmapImagesCache.TryGetValue(imageZoneInfo.Zone, out var heightmapImage);
            // if (heightmapImage == null) {
            //     heightmapImage = imageZoneInfo.Zone.HeightMapTexture.GetImage();
            //     _heightmapImagesCache.Add(imageZoneInfo.Zone, heightmapImage);
            // }

            // Image waterImage = null;
            // if (_terraBrush.WaterDefinition != null) {
            //     _waterImagesCache.TryGetValue(imageZoneInfo.Zone, out waterImage);
            //     if (waterImage == null) {
            //         waterImage = imageZoneInfo.Zone.WaterTexture.GetImage();
            //         _waterImagesCache.Add(imageZoneInfo.Zone, waterImage);
            //     }
            // }

            var xPosition = imageZoneInfo.ZoneInfo.ImagePosition.X;
            var yPosition = imageZoneInfo.ZoneInfo.ImagePosition.Y;

            var currentPixel = imageZoneInfo.Image.GetPixel(xPosition, yPosition);
            var newColor = currentPixel;

            if (pixelBrushStrength > 0f) {
                // var nodeName = $"{xPosition}_{yPosition}";
                newColor = toolType == TerrainToolType.ObjectAdd ? Colors.Red : Colors.Transparent;

                // var existingNode = currentObjectsNode.ObjectsContainer.GetNodeOrNull<Node3D>(nodeName);
                // if (toolType != TerrainToolType.ObjectAdd || existingNode == null) {
                //     var objectNodeItem = currentObjectsNode.GetParent<Objects>();
                //     objectNodeItem.CalculateObjectPresenceForPixel(
                //         heightmapImage,
                //         waterImage,
                //         noiseImage,
                //         xPosition,
                //         yPosition,
                //         newColor,
                //         result => {
                //             var newNode =  _objectItemPackedScene.Instantiate<ObjectItem>();
                //             newNode.Name = nodeName;
                //             newNode.Position = result.ResultPosition;
                //             newNode.RotationDegrees = result.ResultRotation;
                //             newNode.MaximumDistance = currentObject.Definition.MaximumDistance;
                //             newNode.ObjectPackedScene = currentObject.Definition.ObjectScenes[result.ResultPackedSceneIndex];

                //             currentObjectsNode.AddObject(newNode);

                //             newNode.AddChild(newNode.ObjectPackedScene.Instantiate());
                //         },
                //         () => {
                //             existingNode?.QueueFree();
                //         }
                //     );
                // }

                imageZoneInfo.Image.SetPixel(xPosition, yPosition, newColor);

                _sculptedZones.Add(imageZoneInfo.Zone);
            }
        });
    }
}
#endif
