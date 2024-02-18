using Godot;

namespace TerraBrush;

public static class ZoneUtils {
    private const string HeightmapFileName = "Heightmap_{0}_{1}.res";
    private const string SplatmapFileName = "Splatmap_{0}_{1}_{2}.res";
    private const string FoliageFileName = "Foliage_{0}_{1}_{2}.res";
    private const string ObjectFileName = "Object_{0}_{1}_{2}.res";
    private const string WaterFileName = "Water_{0}_{1}.res";
    private const string SnowFileName = "Snow_{0}_{1}.res";

    public static ImageTexture CreateHeightmapImage(int zoneSize, Vector2I zonePosition, string dataPath) {
        var image = Image.Create(zoneSize, zoneSize, false, Image.Format.Rf);
        return GetImageTextureResource(image, string.Format(HeightmapFileName, zonePosition.X, zonePosition.Y), dataPath);
    }

    public static ImageTexture CreateSplatmapImage(int zoneSize, Vector2I zonePosition, int splatmapIndex, string dataPath) {
        var splatmapImage = Image.Create(zoneSize, zoneSize, false, Image.Format.Rgba8);

        if (splatmapIndex == 0) {
            splatmapImage.Fill(new Color(1, 0, 0, 0));
        } else {
            splatmapImage.Fill(new Color(0, 0, 0, 0));
        }

        return GetImageTextureResource(splatmapImage, string.Format(SplatmapFileName, zonePosition.X, zonePosition.Y, splatmapIndex), dataPath);
    }

    public static ImageTexture CreateFoliageImage(int zoneSize, Vector2I zonePosition, int foliageIndex, string dataPath) {
        var image = Image.Create(zoneSize, zoneSize, false, Image.Format.Rgba8);
        return GetImageTextureResource(image, string.Format(FoliageFileName, zonePosition.X, zonePosition.Y, foliageIndex), dataPath);
    }

    public static ImageTexture CreateObjectImage(int zoneSize, Vector2I zonePosition, int objectIndex, string dataPath) {
        var image = Image.Create(zoneSize, zoneSize, false, Image.Format.Rgba8);
        return GetImageTextureResource(image, string.Format(ObjectFileName, zonePosition.X, zonePosition.Y, objectIndex), dataPath);
    }

    public static ImageTexture CreateWaterImage(int zoneSize, Vector2I zonePosition, string dataPath) {
        var waterImage = Image.Create(zoneSize, zoneSize, false, Image.Format.Rgba8);
        waterImage.Fill(new Color(0, 0.5f, 0.5f, 1));

        return GetImageTextureResource(waterImage, string.Format(WaterFileName, zonePosition.X, zonePosition.Y), dataPath);
    }

    public static ImageTexture CreateSnowImage(int zoneSize, Vector2I zonePosition, string dataPath) {
        var snowImage = Image.Create(zoneSize, zoneSize, false, Image.Format.Rgba8);
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

    public static ZoneInfo GetPixelToZoneInfo(int x, int y, int zonesSize) {
        var zoneXPosition = Mathf.FloorToInt(x / (float) zonesSize);
        var zoneYPosition = Mathf.FloorToInt(y / (float) zonesSize);
        var zoneBrushXPosition = (zonesSize * zoneXPosition * -1) + x;
        var zoneBrushYPosition = (zonesSize * zoneYPosition * -1) + y;

        var zonePosition = new Vector2I(zoneXPosition, zoneYPosition);
        // This is just a unique key that combines the x and y, perfect to keep the zone info in cache.
        var zoneKey = (zonePosition.X << 8) + zonePosition.Y;

        return new ZoneInfo() {
            ZoneKey = zoneKey,
            ZonePosition = zonePosition,
            ImagePosition = new Vector2I(zoneBrushXPosition, zoneBrushYPosition)
        };
    }
}

public class ZoneInfo {
    public int ZoneKey { get;set; }
    public Vector2I ZonePosition { get;set; }
    public Vector2I ImagePosition { get;set; }
}
