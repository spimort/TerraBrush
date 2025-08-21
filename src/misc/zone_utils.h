#ifndef ZONE_UTILS_H
#define ZONE_UTILS_H

#include "zone_info.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class ZoneUtils {
    private:
        static constexpr const char* HeightmapFileName = "Heightmap_{0}_{1}.res";
        static constexpr const char* SplatmapFileName = "Splatmap_{0}_{1}_{2}.res";
        static constexpr const char* FoliageFileName = "Foliage_{0}_{1}_{2}.res";
        static constexpr const char* ObjectFileName = "Object_{0}_{1}_{2}.res";
        static constexpr const char* WaterFileName = "Water_{0}_{1}.res";
        static constexpr const char* SnowFileName = "Snow_{0}_{1}.res";
        static constexpr const char* MetaInfoFileName = "MetaInfo_{0}_{1}.res";

    public:
        static Ref<ImageTexture> createLockImage(int zoneSize, Vector2i zonePosition, bool lockAll = false);
        static Ref<ImageTexture> createHeightmapImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<ImageTexture> createSplatmapImage(int zoneSize, Vector2i zonePosition, int splatmapIndex, String dataPath);
        static Ref<ImageTexture> createFoliageImage(int zoneSize, Vector2i zonePosition, int foliageIndex, String dataPath);
        static Ref<ImageTexture> createObjectImage(int zoneSize, Vector2i zonePosition, int objectIndex, String dataPath);
        static Ref<ImageTexture> createWaterImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<ImageTexture> createSnowImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<ImageTexture> createMetaInfoImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<ImageTexture> getImageTextureResource(Ref<Image> image, String filePath, String dataPath);
        static ZoneInfo getPixelToZoneInfo(float x, float y, int zonesSize, int resolution);
        static ZoneInfo getZoneInfoFromZoneOffset(ZoneInfo startingZone, Vector2i offset, int zonesSize, int resolution);
        static int getImageSizeForResolution(int zoneSize, float resolution);
};
#endif