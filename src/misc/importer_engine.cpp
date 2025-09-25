#include "importer_engine.h"
#include "../editor_resources/zones_resource.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/zone_utils.h"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

void ImporterEngine::importTerrain(TerraBrush *terrabrush, ImporterSettings settings) {
    if (!terrabrush->get_dataPath().is_empty() && !DirAccess::dir_exists_absolute(terrabrush->get_dataPath())) {
        DirAccess::make_dir_absolute(terrabrush->get_dataPath());
    }

    if (terrabrush->get_terrainZones().is_null()) {
        terrabrush->set_terrainZones(memnew(ZonesResource));
    }

    // Heightmap
    if (!settings.heightmap.is_null()) {
        std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
            terrabrush,
            settings.heightmap->get_image(),
            ([terrabrush](int zoneX, int zoneY) {
                return ZoneUtils::createHeightmapImage(terrabrush->get_zonesSize(), terrabrush->get_resolution(), Vector2i(zoneX, zoneY), terrabrush->get_dataPath());
            }),
            ([settings](int x, int y, Color pixel, Ref<Image> image) {
                float resultHeight = pixel.r * settings.heightmapScale;
                image->set_pixel(x, y, Color(resultHeight, settings.useGreenChannelForHoles ? pixel.g : 0, 0, 1));
            }),
            true,
            settings.scaleToResolution
        );

        for (ImportImageInfo resultImage : resultImages) {
            getZoneForImageInfo(terrabrush, resultImage)->set_heightMapTexture(resultImage.imageTexture);
        }
    }

    // Splatmaps
    if (settings.splatmaps.size() > 0) {
        for (int i = 0; i < settings.splatmaps.size(); i++) {
            Ref<Texture2D> splatmap = settings.splatmaps[i];

            std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
                terrabrush,
                splatmap->get_image(),
                ([terrabrush, i](int zoneX, int zoneY) {
                    return ZoneUtils::createSplatmapImage(terrabrush->get_zonesSize(), Vector2i(zoneX, zoneY), i, terrabrush->get_dataPath());
                }),
                ([](int x, int y, Color pixel, Ref<Image> image) {
                    image->set_pixel(x, y, pixel);
                })
            );

            for (ImportImageInfo resultImage : resultImages) {
                Ref<ZoneResource> zone = getZoneForImageInfo(terrabrush, resultImage);

                if (zone->get_splatmapsTexture().size() < i + 1) {
                    zone->get_splatmapsTexture().append(resultImage.imageTexture);
                } else {
                    zone->get_splatmapsTexture()[i] = resultImage.imageTexture;
                }
            }
        }
    }

    // Foliages
    if (settings.foliages.size() > 0) {
        for (int i = 0; i < settings.foliages.size(); i++) {
            Ref<Texture2D> foliage = settings.foliages[i];

            std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
                terrabrush,
                foliage->get_image(),
                ([terrabrush, i](int zoneX, int zoneY) {
                    return ZoneUtils::createFoliageImage(terrabrush->get_zonesSize(), Vector2i(zoneX, zoneY), i, terrabrush->get_dataPath());
                }),
                ([](int x, int y, Color pixel, Ref<Image> image) {
                    image->set_pixel(x, y, pixel);
                })
            );

            for (ImportImageInfo resultImage : resultImages) {
                Ref<ZoneResource> zone = getZoneForImageInfo(terrabrush, resultImage);

                if (zone->get_foliagesTexture().size() < i + 1) {
                    zone->get_foliagesTexture().append(resultImage.imageTexture);
                } else {
                    zone->get_foliagesTexture()[i] = resultImage.imageTexture;
                }
            }
        }
    }

    // Objects
    if (settings.objects.size() > 0) {
        for (int i = 0; i < settings.objects.size(); i++) {
            Ref<Texture2D> objectItem = settings.objects[i];

            std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
                terrabrush,
                objectItem->get_image(),
                ([terrabrush, i](int zoneX, int zoneY) {
                    return ZoneUtils::createObjectImage(terrabrush->get_zonesSize(), Vector2i(zoneX, zoneY), i, terrabrush->get_dataPath());
                }),
                ([](int x, int y, Color pixel, Ref<Image> image) {
                    image->set_pixel(x, y, pixel);
                })
            );

            for (ImportImageInfo resultImage : resultImages) {
                Ref<ZoneResource> zone = getZoneForImageInfo(terrabrush, resultImage);

                if (zone->get_objectsTexture().size() < i + 1) {
                    zone->get_objectsTexture().append(resultImage.imageTexture);
                }
                else {
                    zone->get_objectsTexture()[i] = resultImage.imageTexture;
                }
            }
        }
    }

    // Water
    if (!settings.water.is_null()) {
        std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
            terrabrush,
            settings.water->get_image(),
            ([terrabrush](int zoneX, int zoneY) {
                return ZoneUtils::createWaterImage(terrabrush->get_zonesSize(), terrabrush->get_resolution(), Vector2i(zoneX, zoneY), terrabrush->get_dataPath());
            }),
            ([](int x, int y, Color pixel, Ref<Image> image) {
                image->set_pixel(x, y, pixel);
            }),
            true,
            settings.scaleToResolution
        );

        for (ImportImageInfo resultImage : resultImages) {
            getZoneForImageInfo(terrabrush, resultImage)->set_waterTexture(resultImage.imageTexture);
        }
    }

    // Snow
    if (!settings.snow.is_null()) {
        std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
            terrabrush,
            settings.snow->get_image(),
            ([terrabrush](int zoneX, int zoneY) {
                return ZoneUtils::createSnowImage(terrabrush->get_zonesSize(), terrabrush->get_resolution(), Vector2i(zoneX, zoneY), terrabrush->get_dataPath());
            }),
            ([](int x, int y, Color pixel, Ref<Image> image) {
                image->set_pixel(x, y, pixel);
            }),
            true,
            settings.scaleToResolution
        );

        for (ImportImageInfo resultImage : resultImages) {
            getZoneForImageInfo(terrabrush, resultImage)->set_snowTexture(resultImage.imageTexture);
        }
    }

    // MetaInfo
    if (!settings.metaInfo.is_null()) {
        std::vector<ImportImageInfo> resultImages = generateImageTextureForZones(
            terrabrush,
            settings.metaInfo->get_image(),
            ([terrabrush](int zoneX, int zoneY) {
                return ZoneUtils::createMetaInfoImage(terrabrush->get_zonesSize(), terrabrush->get_resolution(), Vector2i(zoneX, zoneY), terrabrush->get_dataPath());
            }),
            ([](int x, int y, Color pixel, Ref<Image> image) {
                image->set_pixel(x, y, pixel);
            }),
            true,
            settings.scaleToResolution
        );

        for (ImportImageInfo resultImage : resultImages) {
            getZoneForImageInfo(terrabrush, resultImage)->set_metaInfoTexture(resultImage.imageTexture);
        }
    }
}

Ref<ZoneResource> ImporterEngine::getZoneForImageInfo(TerraBrush *terrabrush, ImportImageInfo imageInfo) {
    Ref<ZoneResource> zone = nullptr;

    for (Ref<ZoneResource> currentZone : terrabrush->get_terrainZones()->get_zones()) {
        if (currentZone->get_zonePosition().x == imageInfo.zoneX && currentZone->get_zonePosition().y == imageInfo.zoneY) {
            zone = currentZone;
            break;
        }
    }

    if (zone.is_null()) {
        zone = Ref<ZoneResource>(memnew(ZoneResource));
        zone->set_zonePosition(Vector2i(imageInfo.zoneX, imageInfo.zoneY));

        TypedArray<Ref<ZoneResource>> zones = TypedArray<Ref<ZoneResource>>();
        zones.append(zone);
        terrabrush->get_terrainZones()->set_zones(zones);
    }

    return zone;
}

std::vector<ImportImageInfo> ImporterEngine::generateImageTextureForZones(TerraBrush *terrabrush, Ref<Image> image, std::function<Ref<ImageTexture>(int, int)> generateNewImageCallback, std::function<void(int, int, Color, Ref<Image>)> applyPixelToNewImage, bool applyResolution, bool scaleToResolution) {
    if (terrabrush->get_resolution() != 1 && applyResolution && scaleToResolution) {
        Ref<Image> newImage = memnew(Image);
        newImage->copy_from(image);
        newImage->resize(image->get_width() / terrabrush->get_resolution(), image->get_height() / terrabrush->get_resolution());
        image = newImage;
    }

    int resolutionZoneSize = ZoneUtils::getImageSizeForResolution(terrabrush->get_zonesSize(), applyResolution ? terrabrush->get_resolution() : 1);

    int xNumberOfZones = (int) Math::ceil(image->get_width() / (float) resolutionZoneSize);
    int yNumberOfZones = (int) Math::ceil(image->get_height() / (float) resolutionZoneSize);
    std::vector<ImportImageInfo> resultList = std::vector<ImportImageInfo>();

    for (int x = 0; x < xNumberOfZones; x++) {
        for (int y = 0; y < yNumberOfZones; y++) {
            resultList.push_back(generateImageTextureForZone(image, x, y, generateNewImageCallback, applyPixelToNewImage));
        }
    }

    return resultList;
}

ImportImageInfo ImporterEngine::generateImageTextureForZone(Ref<Image> image, int zoneX, int zoneY, std::function<Ref<ImageTexture>(int, int)> generateNewImageCallback, std::function<void(int, int, Color, Ref<Image>)> applyPixelToNewImage) {
    Ref<ImageTexture> newImageTexture = generateNewImageCallback(zoneX, zoneY);
    Ref<Image> newImage = newImageTexture->get_image();
    int newImageSize = newImage->get_width();

    int startingX = newImageSize * zoneX;
    int startingY = newImageSize * zoneY;
    for (int x = 0; x < newImageSize; x++) {
        for (int y = 0; y < newImageSize; y++) {
            int toX = x + startingX;
            int toY = y + startingY;

            Color pixel = Color(0, 0, 0, 1);

            // Try to match the next pixel with the one of the previous zone, for better transition
            if (x == newImageSize - 1) {
                toX += 1;
            }

            if (y == newImageSize - 1) {
                toY += 1;
            }

            if (toX < image->get_width() && toY < image->get_height()) {
                pixel = image->get_pixel(toX, toY);
            }

            applyPixelToNewImage(x, y, pixel, newImage);
        }
    }

    if (!newImageTexture->get_path().is_empty()) {
        ResourceSaver::get_singleton()->save(newImageTexture);
    }

    ImportImageInfo info = ImportImageInfo();
    info.zoneX = zoneX;
    info.zoneY = zoneY;
    info.imageTexture = newImageTexture;
    return info;
}
