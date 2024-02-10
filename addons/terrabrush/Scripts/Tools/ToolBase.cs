using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public abstract class ToolBase {
    private Dictionary<ZoneResource, Image> _imagesCache;
    private Dictionary<int, ZoneResource> _zonesPositionCache;
    protected TerraBrush _terraBrush;
    private List<ImageTexture> _modifiedUndoTextures;

    protected delegate void OnBrushPixel(ImageZoneInfo imageZoneInfo, float pixelBrushStrength, Vector2I absoluteImagePosition);

    public ToolBase(TerraBrush terraBrush) {
        _terraBrush = terraBrush;
    }

    public virtual void BeginPaint() {
        _imagesCache = new();
        _zonesPositionCache = new();
        _modifiedUndoTextures = new();
    }

    public abstract void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition);

    public virtual void EndPaint() {
        _imagesCache = null;
        _zonesPositionCache = null;

        AddImagesToRedo();
        _modifiedUndoTextures = null;
    }

    protected virtual ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return null;
    }

    protected void ForEachBrushPixel(Image brushImage, int brushSize, Vector2 imagePosition, OnBrushPixel onBrushPixel) {
        for (var x = 0; x < brushSize; x++) {
            for (var y = 0; y < brushSize; y++) {
                var xPosition = (int) imagePosition.X - (x - brushSize / 2);
                var yPosition = (int) imagePosition.Y - (y - brushSize / 2);

                var imageZoneInfo = GetImageZoneInfoForPosition(xPosition, yPosition);
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

    protected ImageZoneInfo GetImageZoneInfoForPosition(int x, int y) {
        var zoneInfo = ZoneUtils.GetPixelToZoneInfo(x, y, _terraBrush.ZonesSize);
        _zonesPositionCache.TryGetValue(zoneInfo.ZoneKey, out ZoneResource zone);

        if (zone == null) {
            zone = _terraBrush.TerrainZones?.GetZoneForZoneInfo(zoneInfo);

            if (zone != null) {
                _zonesPositionCache.Add(zoneInfo.ZoneKey, zone);
            }
        }

        if (zone == null && _terraBrush.AutoAddZones) {
            zone = _terraBrush.TerrainZones.AddNewZone(_terraBrush, zoneInfo.ZonePosition);

            if (zone != null) {
                _terraBrush.Terrain.AddZoneCollision(zone);
                _zonesPositionCache.Add(zoneInfo.ZoneKey, zone);
            }
        }

        if (zone != null) {
            _imagesCache.TryGetValue(zone, out Image image);
            var imageTexture = GetToolCurrentImageTexture(zone);

            if (imageTexture != null) {
                if (image == null) {
                    image = imageTexture.GetImage();
                    _imagesCache.Add(zone, image);
                }

                _terraBrush.TerrainZones.AddDirtyImageTexture(imageTexture);
                AddTextureToUndo(imageTexture);
            }

            return new ImageZoneInfo() {
                Image = image,
                ZoneInfo = zoneInfo,
                Zone = zone
            };
        }

        return null;
    }

    protected void AddTextureToUndo(ImageTexture texture) {
        if (!_modifiedUndoTextures.Contains(texture)) {
            _modifiedUndoTextures.Add(texture);

            _terraBrush.UndoRedo.AddUndoMethod(texture, "update", GetUndoRedoImageFromTexture(texture));
        }
    }

    private void AddImagesToRedo() {
        foreach (var imageTexture in _modifiedUndoTextures) {
            _terraBrush.UndoRedo.AddDoMethod(imageTexture, "update", GetUndoRedoImageFromTexture(imageTexture));
        }
    }

    private Image GetUndoRedoImageFromTexture(ImageTexture imageTexture) {
        var image = new Image();
        image.SetData(imageTexture.GetWidth(), imageTexture.GetHeight(), imageTexture.GetImage().HasMipmaps(), imageTexture.GetFormat(), imageTexture.GetImage().GetData());

        return image;
    }

    public class ImageZoneInfo {
        public Image Image { get;set; }
        public ZoneInfo ZoneInfo { get;set; }
        public ZoneResource Zone { get;set; }
    }
}
