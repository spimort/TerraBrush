#ifndef IMPORTER_ENGINE_H
#define IMPORTER_ENGINE_H

#include "../terra_brush.h"

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <vector>
#include <functional>

using namespace godot;

struct ImporterSettings {
    Ref<Texture2D> heightmap = nullptr;
    bool useGreenChannelForHoles = false;
    bool scaleToResolution = true;
    float heightmapScale = 1.0;
    TypedArray<Ref<Texture2D>> splatmaps = TypedArray<Ref<Texture2D>>();
    TypedArray<Ref<Texture2D>> foliages = TypedArray<Ref<Texture2D>>();
    TypedArray<Ref<Texture2D>> objects = TypedArray<Ref<Texture2D>>();
    Ref<Texture2D> water = nullptr;
    Ref<Texture2D> snow = nullptr;
    Ref<Texture2D> metaInfo = nullptr;
};

struct ImportImageInfo {
    int zoneX;
    int zoneY;
    Ref<Image> image;
};

class ImporterEngine {
    public:
        static void importTerrain(TerraBrush *terrabrush, ImporterSettings settings);
        static Ref<ZoneResource> getZoneForImageInfo(TerraBrush *terrabrush, ImportImageInfo imageInfo);
        static std::vector<ImportImageInfo> generateImageTextureForZones(TerraBrush *terrabrush, Ref<Image> image, std::function<Ref<Image>(int, int)> generateNewImageCallback, std::function<void(int, int, Color, Ref<Image>)> applyPixelToNewImage, bool applyResolution = false, bool scaleToResolution = true);
        static ImportImageInfo generateImageTextureForZone(Ref<Image> image, int zoneX, int zoneY, std::function<Ref<Image>(int, int)> generateNewImageCallback, std::function<void(int, int, Color, Ref<Image>)> applyPixelToNewImage);
};
#endif