#ifndef ZONE_UTILS_H
#define ZONE_UTILS_H

#include "zone_info.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class ZoneUtils {
    private:
        static constexpr const char* HeightmapFileName = "Heightmap_{0}_{1}.res";
        static constexpr const char* ColorFileName = "Color_{0}_{1}.res";
        static constexpr const char* SplatmapFileName = "Splatmap_{0}_{1}_{2}.res";
        static constexpr const char* FoliageFileName = "Foliage_{0}_{1}_{2}.res";
        static constexpr const char* ObjectFileName = "Object_{0}_{1}_{2}.res";
        static constexpr const char* WaterFileName = "Water_{0}_{1}.res";
        static constexpr const char* SnowFileName = "Snow_{0}_{1}.res";
        static constexpr const char* MetaInfoFileName = "MetaInfo_{0}_{1}.res";

    public:
        static Ref<Image> createLockImage(int zoneSize, Vector2i zonePosition, bool lockAll = false);
        static Ref<Image> createHeightmapImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<Image> createColorImage(int zoneSize, Vector2i zonePosition, String dataPath);
        static Ref<Image> createSplatmapImage(int zoneSize, Vector2i zonePosition, int splatmapIndex, String dataPath);
        static Ref<Image> createFoliageImage(int zoneSize, Vector2i zonePosition, int foliageIndex, String dataPath);
        static Ref<Image> createObjectImage(int zoneSize, Vector2i zonePosition, int objectIndex, String dataPath);
        static Ref<Image> createWaterImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<Image> createSnowImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<Image> createMetaInfoImage(int zoneSize, float resolution, Vector2i zonePosition, String dataPath);
        static Ref<Image> getImageResource(Ref<Image> image, String filePath, String dataPath);
        static ZoneInfo getPixelToZoneInfo(float x, float y, int zonesSize, int resolution);
        static ZoneInfo getZoneInfoFromZoneOffset(ZoneInfo &startingZone, Vector2i offset, int zonesSize, int resolution);
        static int getImageSizeForResolution(int zoneSize, float resolution);
};
#endif