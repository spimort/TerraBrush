#if TOOLS
using System.Collections.Generic;
using Godot;

namespace TerraBrush;

public class ObjectTool : ToolBase {
    private HashSet<ZoneResource> _sculptedZones;
    private Dictionary<ZoneResource, Image> _heightmapImagesCache = null;
    private Dictionary<ZoneResource, Image> _waterImagesCache = null;

    public ObjectTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint() {
        base.BeginPaint();

        _heightmapImagesCache = new Dictionary<ZoneResource, Image>();
        _waterImagesCache = new Dictionary<ZoneResource, Image>();

        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _heightmapImagesCache = null;
        _waterImagesCache = null;

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

        var objectsNode = _terraBrush.ObjectsContainerNode.GetNode<IObjectsNode>($"{_terraBrush.ObjectIndex}");

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
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
                newColor = toolType == TerrainToolType.ObjectAdd ? Colors.Red : Colors.Transparent;

                objectsNode.AddRemoveObjectFromTool(toolType == TerrainToolType.ObjectAdd, xPosition, yPosition, imageZoneInfo.Zone, heightmapImage, waterImage, noiseImage);
                imageZoneInfo.Image.SetPixel(xPosition, yPosition, newColor);

                _sculptedZones.Add(imageZoneInfo.Zone);
            }
        });

        objectsNode.UpdateMeshesFromTool();
    }
}
#endif
