#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public partial class ImporterSettings : GodotObject {
    public Texture2D Heightmap { get;set; }
    public bool UseGreenChannelForHoles { get;set;}
    public float HeightmapScale { get;set; }
    public Texture2D[] Splatmaps { get;set;}
    public Texture2D[] Foliages { get;set;}
    public Texture2D[] Objects { get;set;}
    public Texture2D Water { get;set;}
    public Texture2D Snow { get;set;}
}

public static class ImporterEngine {
    public static void ImportTerrain(TerraBrushTool terrabrush, ImporterSettings settings) {
        if (!string.IsNullOrWhiteSpace(terrabrush.DataPath) && !DirAccess.DirExistsAbsolute(terrabrush.DataPath)) {
            DirAccess.MakeDirAbsolute(terrabrush.DataPath);
        }

        terrabrush.TerrainZones??= new ZonesResource();
        terrabrush.TerrainZones.Zones??= new ZoneResource[] {};

        // Heightmap
        if (settings.Heightmap != null) {
            var resultImages = GenerateImageTextureForZones(
                terrabrush,
                settings.Heightmap.GetImage(),
                (zoneX, zoneY) => {
                    return ZoneUtils.CreateHeightmapImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), terrabrush.DataPath);
                },
                (x, y, pixel, image) => {
                    var resultHeight = pixel.R * settings.HeightmapScale;
                    image.SetPixel(x, y, new Color(resultHeight, settings.UseGreenChannelForHoles ? pixel.G : 0, 0, 1));
                }
            );

            foreach (var resultImage in resultImages) {
                GetZoneForImageInfo(terrabrush, resultImage).HeightMapTexture = resultImage.ImageTexture;
            }
        }

        // Splatmaps
        if (settings.Splatmaps?.Length > 0) {
            for (var i = 0; i < settings.Splatmaps.Length; i++) {
                var splatmap = settings.Splatmaps[i];

                var resultImages = GenerateImageTextureForZones(
                    terrabrush,
                    splatmap.GetImage(),
                    (zoneX, zoneY) => {
                        return ZoneUtils.CreateSplatmapImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), i, terrabrush.DataPath);
                    },
                    (x, y, pixel, image) => {
                        image.SetPixel(x, y, pixel);
                    }
                );

                foreach (var resultImage in resultImages) {
                    var zone = GetZoneForImageInfo(terrabrush, resultImage);
                    zone.SplatmapsTexture ??= new ImageTexture [] {};
                    if (zone.SplatmapsTexture.Length < i + 1) {
                        zone.SplatmapsTexture = zone.SplatmapsTexture.Append(resultImage.ImageTexture).ToArray();
                    } else {
                        zone.SplatmapsTexture[i] = resultImage.ImageTexture;
                    }
                }
            }
        }

        // Foliages
        if (settings.Foliages?.Length > 0) {
            for (var i = 0; i < settings.Foliages.Length; i++) {
                var foliage = settings.Foliages[i];

                var resultImages = GenerateImageTextureForZones(
                    terrabrush,
                    foliage.GetImage(),
                    (zoneX, zoneY) => {
                        return ZoneUtils.CreateFoliageImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), i, terrabrush.DataPath);
                    },
                    (x, y, pixel, image) => {
                        image.SetPixel(x, y, pixel);
                    }
                );

                foreach (var resultImage in resultImages) {
                    var zone = GetZoneForImageInfo(terrabrush, resultImage);
                    zone.FoliagesTexture ??= new ImageTexture [] {};
                    if (zone.FoliagesTexture.Length < i + 1) {
                        zone.FoliagesTexture = zone.FoliagesTexture.Append(resultImage.ImageTexture).ToArray();
                    } else {
                        zone.FoliagesTexture[i] = resultImage.ImageTexture;
                    }
                }
            }
        }

        // Objects
        if (settings.Objects?.Length > 0) {
            for (var i = 0; i < settings.Objects.Length; i++) {
                var objectItem = settings.Objects[i];

                var resultImages = GenerateImageTextureForZones(
                    terrabrush,
                    objectItem.GetImage(),
                    (zoneX, zoneY) => {
                        return ZoneUtils.CreateObjectImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), i, terrabrush.DataPath);
                    },
                    (x, y, pixel, image) => {
                        image.SetPixel(x, y, pixel);
                    }
                );

                foreach (var resultImage in resultImages) {
                    var zone = GetZoneForImageInfo(terrabrush, resultImage);
                    zone.ObjectsTexture ??= new ImageTexture [] {};
                    if (zone.ObjectsTexture.Length < i + 1) {
                        zone.ObjectsTexture = zone.ObjectsTexture.Append(resultImage.ImageTexture).ToArray();
                    } else {
                        zone.ObjectsTexture[i] = resultImage.ImageTexture;
                    }
                }
            }
        }

        // Water
        if (settings.Water != null) {
            var resultImages = GenerateImageTextureForZones(
                terrabrush,
                settings.Water.GetImage(),
                (zoneX, zoneY) => {
                    return ZoneUtils.CreateWaterImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), terrabrush.DataPath);
                },
                (x, y, pixel, image) => {
                    image.SetPixel(x, y, pixel);
                }
            );

            foreach (var resultImage in resultImages) {
                GetZoneForImageInfo(terrabrush, resultImage).WaterTexture = resultImage.ImageTexture;
            }
        }

        // Snow
        if (settings.Snow != null) {
            var resultImages = GenerateImageTextureForZones(
                terrabrush,
                settings.Snow.GetImage(),
                (zoneX, zoneY) => {
                    return ZoneUtils.CreateSnowImage(terrabrush.ZonesSize, new Vector2I(zoneX, zoneY), terrabrush.DataPath);
                },
                (x, y, pixel, image) => {
                    image.SetPixel(x, y, pixel);
                }
            );

            foreach (var resultImage in resultImages) {
                GetZoneForImageInfo(terrabrush, resultImage).SnowTexture = resultImage.ImageTexture;
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
