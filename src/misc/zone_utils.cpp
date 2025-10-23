#include "zone_utils.h"
#include "utils.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

using namespace godot;

Ref<Image> ZoneUtils::createLockImage(int zoneSize, Vector2i zonePosition, bool lockAll) {
    Ref<Image> image = Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RF);
    if (lockAll) {
        image->fill(Color::named("WHITE"));
    }

    return getImageResource(image, String(HeightmapFileName).format(Array::make(zonePosition.x, zonePosition.y)), "");
}

Ref<Image> ZoneUtils::createHeightmapImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath) {
    int imageSize = getImageSizeForResolution(zoneSize, resolution);
    Ref<Image> image = Image::create_empty(imageSize, imageSize, false, Image::Format::FORMAT_RGF);
    return getImageResource(image, String(HeightmapFileName).format(Array::make(zonePosition.x, zonePosition.y)), dataPath);
}

Ref<Image> ZoneUtils::createSplatmapImage(int zoneSize, Vector2i zonePosition, int splatmapIndex, String dataPath) {
    Ref<Image> splatmapImage = Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RGBA8);

    if (splatmapIndex == 0) {
        splatmapImage->fill(Color(1, 0, 0, 0));
    } else {
        splatmapImage->fill(Color(0, 0, 0, 0));
    }

    return getImageResource(splatmapImage, String(SplatmapFileName).format(Array::make(zonePosition.x, zonePosition.y, splatmapIndex)), dataPath);
}

Ref<Image> ZoneUtils::createFoliageImage(int zoneSize, Vector2i zonePosition, int foliageIndex, String dataPath) {
    Ref<Image> image = Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RGBA8);
    return getImageResource(image, String(FoliageFileName).format(Array::make(zonePosition.x, zonePosition.y, foliageIndex)), dataPath);
}

Ref<Image> ZoneUtils::createObjectImage(int zoneSize, Vector2i zonePosition, int objectIndex, String dataPath) {
    Ref<Image> image = Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RGBA8);
    return getImageResource(image, String(ObjectFileName).format(Array::make(zonePosition.x, zonePosition.y, objectIndex)), dataPath);
}

Ref<Image> ZoneUtils::createWaterImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath) {
    int imageSize = getImageSizeForResolution(zoneSize, resolution);
    Ref<Image> waterImage = Image::create_empty(imageSize, imageSize, false, Image::Format::FORMAT_RGBA8);
    waterImage->fill(Color(0, 0.5f, 0.5f, 1));

    return getImageResource(waterImage, String(WaterFileName).format(Array::make(zonePosition.x, zonePosition.y)), dataPath);
}

Ref<Image> ZoneUtils::createSnowImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath) {
    int imageSize = getImageSizeForResolution(zoneSize, resolution);
    Ref<Image> snowImage = Image::create_empty(imageSize, imageSize, false, Image::Format::FORMAT_RGBA8);
    return getImageResource(snowImage, String(SnowFileName).format(Array::make(zonePosition.x, zonePosition.y)), dataPath);
}

Ref<Image> ZoneUtils::createMetaInfoImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath) {
    int imageSize = getImageSizeForResolution(zoneSize, resolution);
    Ref<Image> metaInfoImage = Image::create_empty(imageSize, imageSize, false, Image::Format::FORMAT_RF);
    metaInfoImage->fill(Color(-1, 0, 0, 0));

    return getImageResource(metaInfoImage, String(MetaInfoFileName).format(Array::make(zonePosition.x, zonePosition.y)), dataPath);
}

Ref<Image> ZoneUtils::getImageResource(Ref<Image> image, String filePath, String dataPath) {
    if (!dataPath.is_empty()) {
        String resourcePath = Utils::pathCombineForwardSlash(dataPath, filePath);
        if (!FileAccess::file_exists(resourcePath)) {
            ResourceSaver::get_singleton()->save(image, resourcePath);
            image = ResourceLoader::get_singleton()->load(resourcePath);
        }
    }

    return image;
}

ZoneInfo ZoneUtils::getPixelToZoneInfo(float x, float y, int zonesSize, int resolution) {
    if (zonesSize % 2 == 0) {
        x -= 0.5f;
        y -= 0.5f;
    }

    int zoneXPosition = (int) Math::floor(x / (zonesSize - 1));
    int zoneYPosition = (int) Math::floor(y / (zonesSize - 1));
    Vector2i zonePosition = Vector2i(zoneXPosition, zoneYPosition);

    int zoneBrushXPosition = (int) Math::round(((x / (zonesSize - 1)) - zoneXPosition) * (zonesSize - 1));
    int zoneBrushYPosition = (int) Math::round(((y / (zonesSize - 1)) - zoneYPosition) * (zonesSize - 1));

    int resolutionZoneBrushXPosition = zoneBrushXPosition;
    int resolutionZoneBrushYPosition = zoneBrushYPosition;

    if (resolution != 1) {
        int imageSize = getImageSizeForResolution(zonesSize, resolution);

        resolutionZoneBrushXPosition = (int) Math::round(Math::remap((float) resolutionZoneBrushXPosition, 0, zonesSize - 1, 0, imageSize - 1));
        resolutionZoneBrushYPosition = (int) Math::round(Math::remap((float) resolutionZoneBrushYPosition, 0, zonesSize - 1, 0, imageSize - 1));
    }

    // This is just a unique key that combines the x and y, perfect to keep the zone info in cache.
    int zoneKey = (zonePosition.x << 8) + zonePosition.y;

    return ZoneInfo(zoneKey, zonePosition, Vector2i(resolutionZoneBrushXPosition, resolutionZoneBrushYPosition));
}

ZoneInfo ZoneUtils::getZoneInfoFromZoneOffset(ZoneInfo &startingZone, Vector2i offset, int zonesSize, int resolution) {
    int resolutionSize = getImageSizeForResolution(zonesSize, resolution);
    Vector2 pixelPosition = Vector2(startingZone.imagePosition.x + offset.x, startingZone.imagePosition.y + offset.y);
    int zoneXPosition = (int) Math::floor(pixelPosition.x / resolutionSize);
    int zoneYPosition = (int) Math::floor(pixelPosition.y / resolutionSize);

    int zoneBrushXPosition = (int) Math::round(((pixelPosition.x / resolutionSize) - zoneXPosition) * resolutionSize);
    int zoneBrushYPosition = (int) Math::round(((pixelPosition.y / resolutionSize) - zoneYPosition) * resolutionSize);

    int resolutionZoneBrushXPosition = zoneBrushXPosition;
    int resolutionZoneBrushYPosition = zoneBrushYPosition;

    // This is just a unique key that combines the x and y, perfect to keep the zone info in cache.
    Vector2i absoluteZonePosition = Vector2i(startingZone.zonePosition.x + zoneXPosition, startingZone.zonePosition.y + zoneYPosition);
    int zoneKey = (absoluteZonePosition.x << 8) + absoluteZonePosition.y;

    return ZoneInfo(zoneKey, absoluteZonePosition, Vector2i(resolutionZoneBrushXPosition, resolutionZoneBrushYPosition));
}

int ZoneUtils::getImageSizeForResolution(int zoneSize, float resolution) {
    return (int) Math::ceil(zoneSize / resolution);
}
