#if TOOLS
using System;
using System.Collections.Generic;
using Godot;

namespace TerraBrush;

enum LockedAxis {
    None,
    X,
    Z
}

public abstract class ToolBase {
    private LockedAxis? _lockedAxis = null;
    private Vector2? _lockedAxisValue = null;
    private Dictionary<ZoneResource, Image> _imagesCache;
    private Dictionary<int, ZoneResource> _zonesPositionCache;
    protected TerraBrush _terraBrush;
    private List<ImageTexture> _modifiedUndoTextures;

    protected virtual bool ApplyResolution => false;

    protected delegate void OnBrushPixel(ImageZoneInfo imageZoneInfo, float pixelBrushStrength);

    public ToolBase(TerraBrush terraBrush) {
        _terraBrush = terraBrush;
    }

    public virtual void BeforeDeselect() {}

    public virtual string GetToolInfo(TerrainToolType toolType) {
        if (_lockedAxis != null) {
            return $"Locked axis : {_lockedAxis}";
        }

        return string.Empty;
    }

    public virtual bool HandleInput(TerrainToolType toolType, InputEvent @event) {
        if (@event is InputEventKey inputEvent && !Input.IsKeyPressed(Key.Ctrl)) {
            if (inputEvent.IsAction(KeybindManager.StringNames.LockXAxis)) {
                if (_lockedAxis != LockedAxis.X) {
                    _lockedAxis = LockedAxis.X;
                } else {
                    _lockedAxis = LockedAxis.None;
                    _lockedAxisValue = null;
                }

                return true;
            } else if (inputEvent.IsAction(KeybindManager.StringNames.LockZAxis)) {
                if (_lockedAxis != LockedAxis.Z) {
                    _lockedAxis = LockedAxis.Z;
                } else {
                    _lockedAxis = LockedAxis.None;
                    _lockedAxisValue = null;
                }

                return true;
            }
        }

        return false;
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

        _lockedAxisValue = null;
    }

    protected virtual ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return null;
    }

    protected void ForEachBrushPixel(Image brushImage, int brushSize, Vector2 imagePosition, OnBrushPixel onBrushPixel, bool ignoreLockedZone = false) {
        if (_lockedAxis != null && _lockedAxis != LockedAxis.None) {
            if (_lockedAxisValue == null) {
                _lockedAxisValue = new Vector2(imagePosition.X, imagePosition.Y);
            } else {
                if (_lockedAxis == LockedAxis.X) {
                    imagePosition = new Vector2(imagePosition.X, _lockedAxisValue.Value.Y);
                } else {
                    imagePosition = new Vector2(_lockedAxisValue.Value.X, imagePosition.Y);
                }
            }
        }

        var startingX = imagePosition.X - (brushSize / 2);
        var startingY = imagePosition.Y - (brushSize / 2);
        var startingZoneInfo = ZoneUtils.GetPixelToZoneInfo(startingX, startingY, _terraBrush.ZonesSize, GetResolution());

        var pointsCache = new HashSet<string>();
        for (var x = 0; x < brushSize; x++) {
            for (var y = 0; y < brushSize; y++) {
                var offsetX = x;
                var offsetY = y;
                if (_terraBrush.Resolution != 1 && ApplyResolution) {
                    offsetX = Mathf.FloorToInt(Mathf.Remap(x, 0, brushSize, 0, Mathf.CeilToInt(brushSize / _terraBrush.Resolution)));
                    offsetY = Mathf.FloorToInt(Mathf.Remap(y, 0, brushSize, 0, Mathf.CeilToInt(brushSize / _terraBrush.Resolution)));
                }
                var imageZoneInfo = GetImageZoneInfoForPosition(startingZoneInfo, offsetX, offsetY, ignoreLockedZone);

                if (imageZoneInfo != null) {
                    var zoneKey = imageZoneInfo.ZoneInfo.ZoneKey;
                    var zoneImagePosition = imageZoneInfo.ZoneInfo?.ImagePosition ?? new Vector2I(0, 0);
                    var positionKey = (zoneImagePosition.X << 8) + zoneImagePosition.Y;
                    // Create a cache key with the zone and the position
                    var zonePositionKey = $"{zoneKey}_{positionKey}";

                    if (_terraBrush.Resolution == 1 || !ApplyResolution || !pointsCache.Contains(zonePositionKey)) {
                        if (_terraBrush.Resolution != 1) {
                            pointsCache.Add(zonePositionKey);
                        }

                        var brushPixelValue = brushImage.GetPixel(x, y);
                        var brushPixelStrength = brushPixelValue.A * (1.0f - imageZoneInfo.LockedStrength);

                        onBrushPixel(
                            imageZoneInfo,
                            brushPixelStrength
                        );
                    }
                }
            }
        }
    }

    protected ImageZoneInfo GetImageZoneInfoForPosition(ZoneInfo startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone = false) {
        var zoneInfo = ZoneUtils.GetZoneInfoFromZoneOffset(startingZoneInfo, new Vector2I(offsetX, offsetY), _terraBrush.ZonesSize, GetResolution());
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
            (bool locked, float lockedStrength) lockInfo = (false, 0.0f);
            if (!ignoreLockedZone && zone.LockTexture != null) {
                var lockZoneInfo = zoneInfo;
                if (_terraBrush.Resolution != 1 && ApplyResolution) {
                    lockZoneInfo = ZoneUtils.GetZoneInfoFromZoneOffset(
                        new ZoneInfo() {
                            ImagePosition = startingZoneInfo.ImagePosition * _terraBrush.Resolution
                        },
                        new Vector2I(offsetX * _terraBrush.Resolution, offsetY * _terraBrush.Resolution),
                        _terraBrush.ZonesSize,
                        1
                    );
                }
                lockInfo = IsZonePixelLocked(zone, lockZoneInfo);
            }

            if (!lockInfo.locked) {
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
                    Zone = zone,
                    LockedStrength = lockInfo!.lockedStrength
                };
            }
        }

        return null;
    }

    private (bool locked, float lockedStrength) IsZonePixelLocked(ZoneResource zone, ZoneInfo zoneInfo) {
        var image = zone.LockTexture.GetImage();
        var pixel = image.GetPixel(zoneInfo.ImagePosition.X, zoneInfo.ImagePosition.Y);
        return (pixel.R == 1.0, pixel.R);
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

    private int GetResolution() {
        if (ApplyResolution) {
            return _terraBrush.Resolution;
        }

        return 1;
    }

    public class ImageZoneInfo {
        public Image Image { get;set; }
        public ZoneInfo ZoneInfo { get;set; }
        public ZoneResource Zone { get;set; }
        public float LockedStrength { get;set; }
    }
}
#endif
