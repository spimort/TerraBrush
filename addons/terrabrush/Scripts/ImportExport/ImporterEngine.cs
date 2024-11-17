#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ImporterSettings : GodotObject {
    public Texture2D Heightmap { get;set; }
    public bool UseGreenChannelForHoles { get;set;}
    public float? MinHeight { get;set; }
    public float? MaxHeight { get;set; }
}

public static class ImporterEngine {
    public static void ImportTerrain(TerraBrushTool terrabrush, ImporterSettings settings) {
        if (!string.IsNullOrWhiteSpace(terrabrush.DataPath) && !DirAccess.DirExistsAbsolute(terrabrush.DataPath)) {
            DirAccess.MakeDirAbsolute(terrabrush.DataPath);
        }

        terrabrush.TerrainZones??= new ZonesResource();
        terrabrush.TerrainZones.Zones??= new ZoneResource[] {};

        if (settings.Heightmap != null) {
            var resultImages = GenerateImageTextureForZones(
                terrabrush,
                settings.Heightmap.GetImage(),
                (zoneX, zoneY) => {
                    return ZoneUtils.CreateHeightmapImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), terrabrush.DataPath);
                },
                (x, y, pixel, image) => {
                    var resultHeight = pixel.R;
                    if (settings.MinHeight != null || settings.MaxHeight != null) {
                        resultHeight = Mathf.Remap(pixel.R, 0f, 1f, settings.MinHeight.GetValueOrDefault(), settings.MaxHeight.GetValueOrDefault());
                    }
                    image.SetPixel(x, y, new Color(resultHeight, 0, 0, 1));
                }
            );

            foreach (var resultImage in resultImages) {
                GetZoneForImageInfo(terrabrush, resultImage).HeightMapTexture = resultImage.ImageTexture;
            }
        }
    }

    private static ZoneResource GetZoneForImageInfo(TerraBrushTool terraBrush, ImportImageInfo imageInfo) {
        var zone = terraBrush.TerrainZones.Zones.FirstOrDefault(x => x.ZonePosition.X == imageInfo.ZoneX && x.ZonePosition.Y == imageInfo.ZoneY);
        if (zone == null) {
            zone = new ZoneResource() {
                ZonePosition = new Vector2I(imageInfo.ZoneX, imageInfo.ZoneY)
            };

            terraBrush.TerrainZones.Zones = new List<ZoneResource>(terraBrush.TerrainZones.Zones) {
                zone
            }.ToArray();
        }

        return zone;
    }

    private static List<ImportImageInfo> GenerateImageTextureForZones(TerraBrushTool terrabrush, Image image, Func<int, int, ImageTexture> generateNewImageCallback, Action<int, int, Color, Image> applyPixelToNewImage) {
        var xNumberOfZones = Mathf.CeilToInt(image.GetWidth() / terrabrush.ZonesSize);
        var yNumberOfZones = Mathf.CeilToInt(image.GetHeight() / terrabrush.ZonesSize);
        var resultList = new List<ImportImageInfo>();

        for (var x = 0; x < xNumberOfZones; x++) {
            for (var y = 0; y < yNumberOfZones; y++) {
                resultList.Add(GenerateImageTextureForZone(image, x, y, generateNewImageCallback, applyPixelToNewImage));
            }
        }

        return resultList;
    }

    private static ImportImageInfo GenerateImageTextureForZone(Image image, int zoneX, int zoneY, Func<int, int, ImageTexture> generateNewImageCallback, Action<int, int, Color, Image> applyPixelToNewImage) {
        var newImageTexture = generateNewImageCallback(zoneX, zoneY);
        var newImage = newImageTexture.GetImage();
        var newImageSize = newImage.GetWidth();

        var startingX = newImageSize * zoneX;
        var startingY = newImageSize * zoneY;
        for (var x = 0; x < newImageSize; x++) {
            for (var y = 0; y < newImageSize; y++) {
                var toX = x + startingX;
                var toY = y + startingY;

                var pixel = Colors.Black;
                if (toX < image.GetWidth() && toY < image.GetHeight()) {
                    pixel = image.GetPixel(toX, toY);
                }

                applyPixelToNewImage(x, y, pixel, newImage);
            }
        }

        if (!string.IsNullOrWhiteSpace(newImageTexture.ResourcePath)) {
            ResourceSaver.Save(newImageTexture);
        }

        return new ImportImageInfo {
            ZoneX = zoneX,
            ZoneY = zoneY,
            ImageTexture = newImageTexture
        };
    }

    internal class ImportImageInfo {
        public int ZoneX { get;set; }
        public int ZoneY { get;set; }
        public ImageTexture ImageTexture { get;set; }
    }
}
#endif
