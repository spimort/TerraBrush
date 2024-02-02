using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public abstract class ToolBase {
    private Dictionary<ZoneResource, Image> _imagesCache;
    private Dictionary<int, ZoneResource> _zonesPositionCache;

    protected delegate void OnBrushPixel(ImageZoneInfo imageZoneInfo, float pixelBrushStrength, Vector2I absoluteImagePosition);

    public virtual void BeginPaint() {
        _imagesCache = new Dictionary<ZoneResource, Image>();
        _zonesPositionCache = new Dictionary<int, ZoneResource>();
    }

    public abstract void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition);

    public virtual void EndPaint() {
        _imagesCache = null;
        _zonesPositionCache = null;
    }

    protected virtual ImageTexture GetToolCurrentImageTexture(TerraBrush terraBrush, ZoneResource zone) {
        return null;
    }

    protected void ForEachBrushPixel(TerraBrush terraBrush, Image brushImage, int brushSize, Vector2 imagePosition, OnBrushPixel onBrushPixel) {
        for (var x = 0; x < brushSize; x++) {
            for (var y = 0; y < brushSize; y++) {
                var xPosition = (int) imagePosition.X - (x - brushSize / 2);
                var yPosition = (int) imagePosition.Y - (y - brushSize / 2);

                var imageZoneInfo = GetImageZoneInfoForPosition(terraBrush, xPosition, yPosition);
                if (imageZoneInfo != null) {
                    var brushPixelValue = brushImage.GetPixel(x, y);
                    var brushPixelStrength = brushPixelValue.A;

                    onBrushPixel(
                        imageZoneInfo,
                        brushPixelStrength,
                        new Vector2I(xPosition, yPosition)
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

    protected ImageZoneInfo GetImageZoneInfoForPosition(TerraBrush terraBrush, int x, int y) {
        var zoneInfo = GetPixelToZoneInfo(x, y, terraBrush.TerrainSize);
        var zoneKey = (zoneInfo.ZonePosition.X << 8) + zoneInfo.ZonePosition.Y;

        _zonesPositionCache.TryGetValue(zoneKey, out ZoneResource zone);

        if (zone == null) {
            zone = terraBrush.TerrainZones?.Zones?.FirstOrDefault(x => x.ZonePosition.X == zoneInfo.ZonePosition.X && x.ZonePosition.Y == zoneInfo.ZonePosition.Y);

            if (zone != null) {
                _zonesPositionCache.Add(zoneKey, zone);
            }
        }

        if (zone == null) {
            zone = terraBrush.TerrainZones.AddNewZone(terraBrush, zoneInfo.ZonePosition);

            if (zone != null) {
                _zonesPositionCache.Add(zoneKey, zone);
            }
        }

        if (zone != null) {
            _imagesCache.TryGetValue(zone, out Image image);

            if (image == null) {
                var imageResource = GetToolCurrentImageTexture(terraBrush, zone);

                if (imageResource != null) {
                    image = imageResource.GetImage();
                    _imagesCache.Add(zone, image);
                }
            }

            return new ImageZoneInfo() {
                Image = image,
                ZoneInfo = zoneInfo,
                Zone = zone
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
        public ZoneResource Zone { get;set; }
    }
}
