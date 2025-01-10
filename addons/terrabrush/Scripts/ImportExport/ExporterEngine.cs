#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public static class ExporterEngine {
    public static void ExportTerrain(TerraBrushTool terrabrush, string dataPath) {
        if ((terrabrush.TerrainZones?.Zones?.Length).GetValueOrDefault() == 0) {
            return;
        }

        if (string.IsNullOrWhiteSpace(dataPath)) {
            return;
        }

        var minZoneX = terrabrush.TerrainZones.Zones.Min(x => x.ZonePosition.X);
        var maxZoneX = terrabrush.TerrainZones.Zones.Max(x => x.ZonePosition.X);
        var minZoneY = terrabrush.TerrainZones.Zones.Min(x => x.ZonePosition.Y);
        var maxZoneY = terrabrush.TerrainZones.Zones.Max(x => x.ZonePosition.Y);

        var resolutionZoneSize = ZoneUtils.GetImageSizeForResolution(terrabrush.ZonesSize, terrabrush.Resolution);
        var resolutionWidth = (maxZoneX + 1 - minZoneX) * resolutionZoneSize;
        var resolutionHeight = (maxZoneY + 1 - minZoneY) * resolutionZoneSize;

        var width = (maxZoneX + 1 - minZoneX) * terrabrush.ZonesSize;
        var height = (maxZoneY + 1 - minZoneY) * terrabrush.ZonesSize;

        var firstZone = terrabrush.TerrainZones.Zones[0];

        var resultHeightmapImage = Image.CreateEmpty(resolutionWidth, resolutionHeight, false, firstZone.HeightMapTexture.GetFormat());
        var resultSplatmapsImages = firstZone.SplatmapsTexture?.Select(x => Image.CreateEmpty(width, height, false, x.GetFormat())).ToList();
        var resultFoliagesImages = firstZone.FoliagesTexture?.Select(x => Image.CreateEmpty(width, height, false, x.GetFormat())).ToList();
        var resultObjectsImages = firstZone.ObjectsTexture?.Select(x => Image.CreateEmpty(width, height, false, x.GetFormat())).ToList();
        var resultWaterImage = firstZone.WaterTexture == null ? null : Image.CreateEmpty(resolutionWidth, resolutionHeight, false, firstZone.WaterTexture.GetFormat());
        var resultSnowImage = firstZone.SnowTexture == null ? null : Image.CreateEmpty(resolutionWidth, resolutionHeight, false, firstZone.SnowTexture.GetFormat());

        for (var zoneX = minZoneX; zoneX <= maxZoneX; zoneX++) {
            for (var zoneY = minZoneY; zoneY <= maxZoneY; zoneY++) {
                var zone = terrabrush.TerrainZones.Zones.FirstOrDefault(zone => zone.ZonePosition.X == zoneX && zone.ZonePosition.Y == zoneY);
                Image heightMapImage = null;
                List<Image> splatmapsImages = null;
                List<Image> foliagesImages = null;
                List<Image> objectsImages = null;
                Image waterImage = null;
                Image snowImage = null;

                if (zone != null) {
                    heightMapImage = zone.HeightMapTexture.GetImage();
                    splatmapsImages = zone.SplatmapsTexture?.Select(x => x.GetImage()).ToList();
                    foliagesImages = zone.FoliagesTexture?.Select(x => x.GetImage()).ToList();
                    objectsImages = zone.ObjectsTexture?.Select(x => x.GetImage()).ToList();
                    waterImage = zone.WaterTexture?.GetImage();
                    snowImage = zone.SnowTexture?.GetImage();
                }

                // Process the images that works with resolution
                for (var x = 0; x < resolutionZoneSize; x++) {
                    for (var y = 0; y < resolutionZoneSize; y++) {
                        var globalX = x + (zoneX - minZoneX) * resolutionZoneSize;
                        var globalY = y + (zoneY - minZoneY) * resolutionZoneSize;

                        resultHeightmapImage.SetPixel(globalX, globalY, heightMapImage == null ? Colors.Black : heightMapImage.GetPixel(x, y));
                        resultWaterImage?.SetPixel(globalX, globalY, waterImage == null ? new Color(0, 0.5f, 0.5f, 1f) : waterImage.GetPixel(x, y));
                        resultSnowImage?.SetPixel(globalX, globalY, snowImage == null ? Colors.Black : snowImage.GetPixel(x, y));
                    }
                }

                // Process the images that does not work with resolution
                for (var x = 0; x < terrabrush.ZonesSize; x++) {
                    for (var y = 0; y < terrabrush.ZonesSize; y++) {
                        var globalX = x + (zoneX - minZoneX) * terrabrush.ZonesSize;
                        var globalY = y + (zoneY - minZoneY) * terrabrush.ZonesSize;

                        if (splatmapsImages != null) {
                            for (var itemIndex = 0; itemIndex < splatmapsImages.Count; itemIndex++) {
                                var itemItem = splatmapsImages[itemIndex];
                                resultSplatmapsImages[itemIndex].SetPixel(globalX, globalY, itemItem.GetPixel(x, y));
                            }
                        }
                        if (foliagesImages != null) {
                            for (var itemIndex = 0; itemIndex < foliagesImages.Count; itemIndex++) {
                                var itemItem = foliagesImages[itemIndex];
                                resultFoliagesImages[itemIndex].SetPixel(globalX, globalY, itemItem.GetPixel(x, y));
                            }
                        }
                        if (objectsImages != null) {
                            for (var itemIndex = 0; itemIndex < objectsImages.Count; itemIndex++) {
                                var itemItem = objectsImages[itemIndex];
                                resultObjectsImages[itemIndex].SetPixel(globalX, globalY, itemItem.GetPixel(x, y));
                            }
                        }
                    }
                }
            }
        }

        resultHeightmapImage.SaveExr($"{dataPath}/heightmap.exr");
        resultSplatmapsImages?.Select((item, index) => new {Item = item, Index = index}).ToList().ForEach(item => {
            item.Item.SavePng($"{dataPath}/splatmap_{item.Index}.png");
        });
        resultFoliagesImages?.Select((item, index) => new {Item = item, Index = index}).ToList().ForEach(item => {
            item.Item.SavePng($"{dataPath}/foliage_{item.Index}.png");
        });
        resultObjectsImages?.Select((item, index) => new {Item = item, Index = index}).ToList().ForEach(item => {
            item.Item.SavePng($"{dataPath}/object_{item.Index}.png");
        });
        resultWaterImage?.SavePng($"{dataPath}/water.png");
        resultSnowImage?.SavePng($"{dataPath}/snow.png");
    }
}
#endif
