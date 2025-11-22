#include "exporter_engine.h"
#include "zone_utils.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ExporterEngine::exportTerrain(TerraBrush *terrabrush, String dataPath) {
    if (terrabrush->get_terrainZones().is_null() || terrabrush->get_terrainZones()->get_zones().size() == 0) {
        return;
    }

    if (dataPath.is_empty()) {
        return;
    }

    int minZoneX = 0;
    int maxZoneX = 0;
    int minZoneY = 0;
    int maxZoneY = 0;

    for (Ref<ZoneResource> zone : terrabrush->get_terrainZones()->get_zones()) {
        if (zone->get_zonePosition().x < minZoneX) {
            minZoneX = zone->get_zonePosition().x;
        }
        if (zone->get_zonePosition().x > maxZoneX) {
            maxZoneX = zone->get_zonePosition().x;
        }
        if (zone->get_zonePosition().y < minZoneY) {
            minZoneY = zone->get_zonePosition().y;
        }
        if (zone->get_zonePosition().y > maxZoneY) {
            maxZoneY = zone->get_zonePosition().y;
        }
    }

    int resolutionZoneSize = ZoneUtils::getImageSizeForResolution(terrabrush->get_zonesSize(), terrabrush->get_resolution());
    int resolutionWidth = (maxZoneX + 1 - minZoneX) * resolutionZoneSize;
    int resolutionHeight = (maxZoneY + 1 - minZoneY) * resolutionZoneSize;

    int width = (maxZoneX + 1 - minZoneX) * terrabrush->get_zonesSize();
    int height = (maxZoneY + 1 - minZoneY) * terrabrush->get_zonesSize();

    Ref<ZoneResource> firstZone = terrabrush->get_terrainZones()->get_zones()[0];

    Ref<Image> resultHeightmapImage = Image::create_empty(resolutionWidth, resolutionHeight, false, firstZone->get_heightMapImage()->get_format());
    std::vector<Ref<Image>> resultSplatmapsImages = std::vector<Ref<Image>>();
    for (Ref<Image> splatmapTexture : firstZone->get_splatmapsImage()) {
        resultSplatmapsImages.push_back(Image::create_empty(width, height, false, splatmapTexture->get_format()));
    }

    std::vector<Ref<Image>> resultFoliagesImages = std::vector<Ref<Image>>();
    for (Ref<Image> foliageTexture : firstZone->get_foliagesImage()) {
        resultFoliagesImages.push_back(Image::create_empty(width, height, false, foliageTexture->get_format()));
    }

    std::vector<Ref<Image>> resultObjectsImages = std::vector<Ref<Image>>();
    for (Ref<Image> objectTexture : firstZone->get_objectsImage()) {
        resultObjectsImages.push_back(Image::create_empty(width, height, false, objectTexture->get_format()));
    }

    Ref<Image> resultWaterImage = nullptr;
    if (!firstZone->get_waterImage().is_null()) {
        resultWaterImage = Image::create_empty(width, height, false, firstZone->get_waterImage()->get_format());
    }

    Ref<Image> resultSnowImage = nullptr;
    if (!firstZone->get_snowImage().is_null()) {
        resultSnowImage = Image::create_empty(width, height, false, firstZone->get_snowImage()->get_format());
    }

     Ref<Image> resultMetaInfoImage = nullptr;
    if (!firstZone->get_metaInfoImage().is_null()) {
        resultMetaInfoImage = Image::create_empty(width, height, false, firstZone->get_metaInfoImage()->get_format());
    }

    for (int zoneX = minZoneX; zoneX <= maxZoneX; zoneX++) {
        for (int zoneY = minZoneY; zoneY <= maxZoneY; zoneY++) {
            Ref<ZoneResource> zone = nullptr;
            for (Ref<ZoneResource> currentZone : terrabrush->get_terrainZones()->get_zones()) {
                if (currentZone->get_zonePosition().x == zoneX && currentZone->get_zonePosition().y == zoneY) {
                    zone = currentZone;
                    break;
                }
            }

            Ref<Image> heightMapImage = nullptr;
            std::vector<Ref<Image>> splatmapsImages = std::vector<Ref<Image>>();
            std::vector<Ref<Image>> foliagesImages = std::vector<Ref<Image>>();
            std::vector<Ref<Image>> objectsImages = std::vector<Ref<Image>>();
            Ref<Image> waterImage = nullptr;
            Ref<Image> snowImage = nullptr;
            Ref<Image> metaInfoImage = nullptr;

            if (!zone.is_null()) {
                heightMapImage = zone->get_heightMapImage();

                for (Ref<Image> splatmapTexture : zone->get_splatmapsImage()) {
                    splatmapsImages.push_back(splatmapTexture);
                }

                for (Ref<Image> foliageTexture : zone->get_foliagesImage()) {
                    foliagesImages.push_back(foliageTexture);
                }

                for (Ref<Image> objectTexture : zone->get_objectsImage()) {
                    objectsImages.push_back(objectTexture);
                }

                if (!zone->get_waterImage().is_null()) {
                    waterImage = zone->get_waterImage();
                }

                if (!zone->get_snowImage().is_null()) {
                    snowImage = zone->get_snowImage();
                }

                if (!zone->get_metaInfoImage().is_null()) {
                    metaInfoImage = zone->get_metaInfoImage();
                }
            }

            // Process the images that works with resolution
            for (int x = 0; x < resolutionZoneSize; x++) {
                for (int y = 0; y < resolutionZoneSize; y++) {
                    int globalX = x + (zoneX - minZoneX) * resolutionZoneSize;
                    int globalY = y + (zoneY - minZoneY) * resolutionZoneSize;

                    resultHeightmapImage->set_pixel(globalX, globalY, heightMapImage.is_null() ? Color(0, 0, 0, 1) : heightMapImage->get_pixel(x, y));

                    if (!resultWaterImage.is_null()) {
                        resultWaterImage->set_pixel(globalX, globalY, waterImage.is_null() ? Color(0, 0.5f, 0.5f, 1.0f) : waterImage->get_pixel(x, y));
                    }

                    if (!resultSnowImage.is_null()) {
                        resultSnowImage->set_pixel(globalX, globalY, snowImage.is_null() ? Color(0, 0, 0, 1) : snowImage->get_pixel(x, y));
                    }

                    if (!resultMetaInfoImage.is_null()) {
                        resultMetaInfoImage->set_pixel(globalX, globalY, metaInfoImage.is_null() ? Color(-1, 0, 0, 0) : metaInfoImage->get_pixel(x, y));
                    }
                }
            }

            // Process the images that does not work with resolution
            for (int x = 0; x < terrabrush->get_zonesSize(); x++) {
                for (int y = 0; y < terrabrush->get_zonesSize(); y++) {
                    int globalX = x + (zoneX - minZoneX) * terrabrush->get_zonesSize();
                    int globalY = y + (zoneY - minZoneY) * terrabrush->get_zonesSize();

                    if (splatmapsImages.size() > 0) {
                        for (int itemIndex = 0; itemIndex < splatmapsImages.size(); itemIndex++) {
                            Ref<Image> itemItem = splatmapsImages[itemIndex];
                            resultSplatmapsImages[itemIndex]->set_pixel(globalX, globalY, itemItem->get_pixel(x, y));
                        }
                    }
                    if (foliagesImages.size() > 0) {
                        for (int itemIndex = 0; itemIndex < foliagesImages.size(); itemIndex++) {
                            Ref<Image> itemItem = foliagesImages[itemIndex];
                            resultFoliagesImages[itemIndex]->set_pixel(globalX, globalY, itemItem->get_pixel(x, y));
                        }
                    }
                    if (objectsImages.size() > 0) {
                        for (int itemIndex = 0; itemIndex < objectsImages.size(); itemIndex++) {
                            Ref<Image> itemItem = objectsImages[itemIndex];
                            resultObjectsImages[itemIndex]->set_pixel(globalX, globalY, itemItem->get_pixel(x, y));
                        }
                    }
                }
            }
        }
    }

    resultHeightmapImage->save_exr(dataPath + "/heightmap.exr");

    for (int i = 0; i < resultSplatmapsImages.size(); i++) {
        Ref<Image> itemImage = resultSplatmapsImages[i];
        itemImage->save_png(dataPath + "/splatmap_" + String::num_int64(i) + ".png");
    }

    for (int i = 0; i < resultFoliagesImages.size(); i++) {
        Ref<Image> itemImage = resultFoliagesImages[i];
        itemImage->save_png(dataPath + "/foliage_" + String::num_int64(i) + ".png");
    }

    for (int i = 0; i < resultObjectsImages.size(); i++) {
        Ref<Image> itemImage = resultObjectsImages[i];
        itemImage->save_png(dataPath + "/object_" + String::num_int64(i) + ".png");
    }

    if (!resultWaterImage.is_null()) {
        resultWaterImage->save_png(dataPath + "/water.png");
    }

    if (!resultSnowImage.is_null()) {
        resultSnowImage->save_png(dataPath + "/snow.png");
    }

    if (!resultMetaInfoImage.is_null()) {
        resultMetaInfoImage->save_exr(dataPath + "/metainfo.exr");
    }
}