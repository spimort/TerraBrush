using System;
using Godot;

namespace TerraBrush;

public static class ZoneUtils {
    private const string HeightmapFileName = "Heightmap_{0}_{1}.res";
    private const string SplatmapFileName = "Splatmap_{0}_{1}_{2}.res";
    private const string FoliageFileName = "Foliage_{0}_{1}_{2}.res";
    private const string ObjectFileName = "Object_{0}_{1}_{2}.res";
    private const string WaterFileName = "Water_{0}_{1}.res";
    private const string SnowFileName = "Snow_{0}_{1}.res";

    public static ImageTexture CreateLockImage(int zoneSize, Vector2I zonePosition, bool lockAll = false) {
        var image = GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rf);
        if (lockAll) {
            image.Fill(Colors.White);
        }

        return GetImageTextureResource(image, string.Format(HeightmapFileName, zonePosition.X, zonePosition.Y), "");
    }

    public static ImageTexture CreateHeightmapImage(int zoneSize, float resolution, Vector2I zonePosition, string dataPath) {
        var imageSize = GetImageSizeForResolution(zoneSize, resolution);
        var image = GodotAgnostic.ImageCreateEmpty(imageSize, imageSize, false, Image.Format.Rgf);
        return GetImageTextureResource(image, string.Format(HeightmapFileName, zonePosition.X, zonePosition.Y), dataPath);
    }

    public static ImageTexture CreateSplatmapImage(int zoneSize, Vector2I zonePosition, int splatmapIndex, string dataPath) {
        var splatmapImage = GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rgba8);

        if (splatmapIndex == 0) {
            splatmapImage.Fill(new Color(1, 0, 0, 0));
        } else {
            splatmapImage.Fill(new Color(0, 0, 0, 0));
        }

        return GetImageTextureResource(splatmapImage, string.Format(SplatmapFileName, zonePosition.X, zonePosition.Y, splatmapIndex), dataPath);
    }

    public static ImageTexture CreateFoliageImage(int zoneSize, Vector2I zonePosition, int foliageIndex, string dataPath) {
        var image = GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rgba8);
        return GetImageTextureResource(image, string.Format(FoliageFileName, zonePosition.X, zonePosition.Y, foliageIndex), dataPath);
    }

    public static ImageTexture CreateObjectImage(int zoneSize, Vector2I zonePosition, int objectIndex, string dataPath) {
        var image = GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rgba8);
        return GetImageTextureResource(image, string.Format(ObjectFileName, zonePosition.X, zonePosition.Y, objectIndex), dataPath);
    }

    public static ImageTexture CreateWaterImage(int zoneSize, float resolution, Vector2I zonePosition, string dataPath) {
        var imageSize = GetImageSizeForResolution(zoneSize, resolution);
        var waterImage = GodotAgnostic.ImageCreateEmpty(imageSize, imageSize, false, Image.Format.Rgba8);
        waterImage.Fill(new Color(0, 0.5f, 0.5f, 1));

        return GetImageTextureResource(waterImage, string.Format(WaterFileName, zonePosition.X, zonePosition.Y), dataPath);
    }

    public static ImageTexture CreateSnowImage(int zoneSize, float resolution, Vector2I zonePosition, string dataPath) {
        var imageSize = GetImageSizeForResolution(zoneSize, resolution);
        var snowImage = GodotAgnostic.ImageCreateEmpty(imageSize, imageSize, false, Image.Format.Rgba8);
        return GetImageTextureResource(snowImage, string.Format(SnowFileName, zonePosition.X, zonePosition.Y), dataPath);
    }

    private static ImageTexture GetImageTextureResource(Image image, string filePath, string dataPath) {
        ImageTexture imageTexture;
        if (string.IsNullOrWhiteSpace(dataPath)) {
            imageTexture = ImageTexture.CreateFromImage(image);
        } else {
            var resourcePath = Utils.PathCombineForwardSlash(dataPath, filePath);
            if (Godot.FileAccess.FileExists(resourcePath)) {
                imageTexture = ResourceLoader.Load<ImageTexture>(resourcePath);
                imageTexture.SetImage(image);
            } else {
                imageTexture = ImageTexture.CreateFromImage(image);
                ResourceSaver.Save(imageTexture, resourcePath);
                imageTexture = ResourceLoader.Load<ImageTexture>(resourcePath);
            }
        }

        return imageTexture;
    }

    public static ZoneInfo GetPixelToZoneInfo(float x, float y, int zonesSize, int resolution) {
        if (zonesSize % 2 == 0) {
            x -= 0.5f;
            y -= 0.5f;
        }

        var zoneXPosition = Mathf.FloorToInt(x / (zonesSize - 1));
        var zoneYPosition = Mathf.FloorToInt(y / (zonesSize - 1));
        var zonePosition = new Vector2I(zoneXPosition, zoneYPosition);

        var zoneBrushXPosition = Mathf.RoundToInt(((x / (zonesSize - 1)) - zoneXPosition) * (zonesSize - 1));
        var zoneBrushYPosition = Mathf.RoundToInt(((y / (zonesSize - 1)) - zoneYPosition) * (zonesSize - 1));

        var resolutionZoneBrushXPosition = zoneBrushXPosition;
        var resolutionZoneBrushYPosition = zoneBrushYPosition;

        if (resolution != 1) {
            var imageSize = GetImageSizeForResolution(zonesSize, resolution);

            resolutionZoneBrushXPosition = Mathf.RoundToInt(Mathf.Remap(resolutionZoneBrushXPosition, 0, zonesSize - 1, 0, imageSize - 1));
            resolutionZoneBrushYPosition = Mathf.RoundToInt(Mathf.Remap(resolutionZoneBrushYPosition, 0, zonesSize - 1, 0, imageSize - 1));
        }

        // This is just a unique key that combines the x and y, perfect to keep the zone info in cache.
        var zoneKey = (zonePosition.X << 8) + zonePosition.Y;

        return new ZoneInfo() {
            ZoneKey = zoneKey,
            ZonePosition = zonePosition,
            ImagePosition = new Vector2I(resolutionZoneBrushXPosition, resolutionZoneBrushYPosition)
        };
    }

    public static ZoneInfo GetZoneInfoFromZoneOffset(ZoneInfo startingZone, Vector2I offset, int zonesSize, int resolution) {
        var resolutionSize = GetImageSizeForResolution(zonesSize, resolution);

        var pixelPosition = new Vector2(startingZone.ImagePosition.X + offset.X, startingZone.ImagePosition.Y + offset.Y);
        var zoneXPosition = Mathf.FloorToInt(pixelPosition.X / resolutionSize);
        var zoneYPosition = Mathf.FloorToInt(pixelPosition.Y / resolutionSize);

        var zoneBrushXPosition = Mathf.RoundToInt(((pixelPosition.X / resolutionSize) - zoneXPosition) * resolutionSize);
        var zoneBrushYPosition = Mathf.RoundToInt(((pixelPosition.Y / resolutionSize) - zoneYPosition) * resolutionSize);

        var resolutionZoneBrushXPosition = zoneBrushXPosition;
        var resolutionZoneBrushYPosition = zoneBrushYPosition;

        // This is just a unique key that combines the x and y, perfect to keep the zone info in cache.
        var absoluteZonePosition = new Vector2I(startingZone.ZonePosition.X + zoneXPosition, startingZone.ZonePosition.Y + zoneYPosition);
        var zoneKey = (absoluteZonePosition.X << 8) + absoluteZonePosition.Y;

        return new ZoneInfo() {
            ZoneKey = zoneKey,
            ZonePosition = absoluteZonePosition,
            ImagePosition = new Vector2I(resolutionZoneBrushXPosition, resolutionZoneBrushYPosition)
        };
    }

    public static int GetImageSizeForResolution(int zoneSize, float resolution) {
        return Mathf.CeilToInt(zoneSize / resolution);
    }
}

public class ZoneInfo {
    public int ZoneKey { get;set; }
    public Vector2I ZonePosition { get;set; }
    public Vector2I ImagePosition { get;set; }
}
