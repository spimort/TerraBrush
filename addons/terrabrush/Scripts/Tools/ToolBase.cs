using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public abstract class ToolBase {
    protected delegate void OnBrushPixel(Image image, float pixelBrushStrength, Vector2I relativeImagePosition, Vector2I absoluteImagePosition, Dictionary<ZoneResource, Image> imagesCache);

    public abstract void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition);

    protected void ForEachBrushPixel(TerraBrush terraBrush, Image brushImage, int brushSize, Vector2 imagePosition, OnBrushPixel onBrushPixel) {
        var imagesCache = new Dictionary<ZoneResource, Image>();

        for (var x = 0; x < brushSize; x++) {
            for (var y = 0; y < brushSize; y++) {
                var xPosition = (int) imagePosition.X - (x - brushSize / 2);
                var yPosition = (int) imagePosition.Y - (y - brushSize / 2);

                var imageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition, yPosition, imagesCache);
                if (imageZoneInfo != null) {
                    var brushPixelValue = brushImage.GetPixel(x, y);
                    var colorValue = brushPixelValue.A;

                    onBrushPixel(
                        imageZoneInfo.Image,
                        colorValue,
                        new Vector2I(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y),
                        new Vector2I(xPosition, yPosition),
                        imagesCache
                    );
                }
            }
        }
    }

    protected ZoneInfo GetPixelToZoneInfo(int x, int y, int terrainSize) {
        var zoneXPosition = Mathf.FloorToInt(x / (float) terrainSize);
        var zoneYPosition = Mathf.FloorToInt(y / (float) terrainSize);
        var zoneBrushXPosition = (terrainSize * zoneXPosition * -1) + x;
        var zoneBrushYPosition = (terrainSize * zoneYPosition * -1) + y;

        return new ZoneInfo() {
            ZonePosition = new Vector2I(zoneXPosition, zoneYPosition),
            ImagePosition = new Vector2I(zoneBrushXPosition, zoneBrushYPosition)
        };
    }

    protected ImageZoneInfo GetImageZoneInfoForPosition(TerraBrush terraBrush, int x, int y, Dictionary<ZoneResource, Image> imagesCache) {
        var zoneInfo = GetPixelToZoneInfo(x, y, terraBrush.TerrainSize);
        var zone = terraBrush.TerrainZones?.Zones?.FirstOrDefault(x => x.ZonePosition.X == zoneInfo.ZonePosition.X && x.ZonePosition.Y == zoneInfo.ZonePosition.Y);
        if (zone != null) {
            var image = imagesCache.ContainsKey(zone) ? imagesCache[zone] : null;
            if (image == null) {
                image = zone.HeightMapTexture.GetImage();
                imagesCache.Add(zone, image);
            }

            return new ImageZoneInfo() {
                Image = image,
                ZoneInfo = zoneInfo
            };
        }

        return null;
    }

    public class ZoneInfo {
        public Vector2I ZonePosition { get;set; }
        public Vector2I ImagePosition { get;set; }
    }

    public class ImageZoneInfo {
        public Image Image { get;set; }
        public ZoneInfo ZoneInfo { get;set; }
    }
}
