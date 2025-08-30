#ifndef ZONES_RESOURCE_H
#define ZONES_RESOURCE_H

#include "zone_resource.h"
#include "../misc/zone_info.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/templates/hash_set.hpp>

using namespace godot;

class ZonesResource : public Resource {
    GDCLASS(ZonesResource, Resource)

private:
    HashSet<Ref<ImageTexture>> _dirtyImageTextures;

    Ref<Texture2DArray> _lockTextures;
    Ref<Texture2DArray> _heightmapTextures;
    Ref<Texture2DArray> _splatmapsTextures;
    TypedArray<Ref<Texture2DArray>> _foliagesTextures;
    Ref<Texture2DArray> _objectsTextures; // This should be TypedArray<Ref<Texture2DArray>> like foliages but it's unused for now. Might remove it later.
    Ref<Texture2DArray> _waterTextures;
    Ref<Texture2DArray> _snowTextures;
    Ref<Texture2DArray> _metaInfoTextures;
    Ref<ImageTexture> _zonesMap;

    TypedArray<Ref<ZoneResource>> _zones;

    void saveImageResource(Ref<ImageTexture> image);

protected:
    static void _bind_methods();

public:
    ZonesResource();
    ~ZonesResource();

    Ref<Texture2DArray> get_lockTextures() const;
    Ref<Texture2DArray> get_heightmapTextures() const;
    Ref<Texture2DArray> get_splatmapsTextures() const;
    TypedArray<Ref<Texture2DArray>> get_foliagesTextures() const;
    Ref<Texture2DArray> get_objectsTextures() const;
    Ref<Texture2DArray> get_waterTextures() const;
    Ref<Texture2DArray> get_snowTextures() const;
    Ref<Texture2DArray> get_metaInfoTextures() const;
    Ref<ImageTexture> get_zonesMap() const;

    TypedArray<Ref<ZoneResource>> get_zones() const;
    void set_zones(const TypedArray<Ref<ZoneResource>> value);

    void updateLockTexture(int zoneSize);
    void updateHeightmaps();
    void updateSplatmapsTextures();
    void initializeFoliageTextures(int foliageCount);
    void updateFoliagesTextures();
    void updateFoliagesTextures(int foliageIndex);
    void updateObjectsTextures();
    void updateWaterTextures();
    void updateZoneWaterTexture(Ref<ZoneResource> zone);
    void updateSnowTextures();
    void updateZoneSnowTexture(Ref<ZoneResource> zone);
    void updateMetaInfoTextures();
    void updateZoneMetaInfoTexture(Ref<ZoneResource> zone);
    void saveResources();
    void updateZonesMap();
    void addDirtyImageTexture(Ref<ImageTexture> imageTexture);
    void updateImageTextures(int zoneSize);
    Ref<ZoneResource> getZoneForZoneInfo(Ref<ZoneInfo> zoneInfo);
};
#endif