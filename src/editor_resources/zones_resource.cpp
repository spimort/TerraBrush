#include "zones_resource.h"
#include "../misc/zone_info.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>

using namespace godot;

void ZonesResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_zones"), &ZonesResource::get_zones);
    ClassDB::bind_method(D_METHOD("set_zones", "value"), &ZonesResource::set_zones);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "zones", PROPERTY_HINT_TYPE_STRING, "24/17:ZoneResource"), "set_zones", "get_zones");
}

ZonesResource::ZonesResource() {
    _dirtyImages = std::unordered_set<Ref<Image>>();

    _lockTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _heightmapTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _colorTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _splatmapsTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _foliagesTextures = TypedArray<Ref<Texture2DArray>>();
    _objectsTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _waterTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _snowTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _metaInfoTextures = Ref<Texture2DArray>(memnew(Texture2DArray));
    _zonesMap = Ref<ImageTexture>(memnew(ImageTexture));

    _zones = TypedArray<Ref<ZoneResource>>();
}

ZonesResource::~ZonesResource() {}

Ref<Texture2DArray> ZonesResource::get_lockTextures() const {
    return _lockTextures;
}
Ref<Texture2DArray> ZonesResource::get_heightmapTextures() const {
    return _heightmapTextures;
}
Ref<Texture2DArray> ZonesResource::get_colorTextures() const {
    return _colorTextures;
}
Ref<Texture2DArray> ZonesResource::get_splatmapsTextures() const {
    return _splatmapsTextures;
}
TypedArray<Ref<Texture2DArray>> ZonesResource::get_foliagesTextures() const {
    return _foliagesTextures;
}
Ref<Texture2DArray> ZonesResource::get_objectsTextures() const {
    return _objectsTextures;
}
Ref<Texture2DArray> ZonesResource::get_waterTextures() const {
    return _waterTextures;
}
Ref<Texture2DArray> ZonesResource::get_snowTextures() const {
    return _snowTextures;
}
Ref<Texture2DArray> ZonesResource::get_metaInfoTextures() const {
    return _metaInfoTextures;
}
Ref<ImageTexture> ZonesResource::get_zonesMap() const {
    return _zonesMap;
}

TypedArray<Ref<ZoneResource>> ZonesResource::get_zones() const {
    return _zones;
}
void ZonesResource::set_zones(const TypedArray<Ref<ZoneResource>> value) {
    _zones = value;
}

void ZonesResource::updateLockTexture(int zoneSize) {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (zone->get_lockTexture().is_null()) {
            images.append(Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RF));
        } else {
            images.append(zone->get_lockTexture());
        }
    }

    if (images.size() > 0) {
        _lockTextures->create_from_images(images);
    }
}

void ZonesResource::updateHeightmaps() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        images.append(zone->get_heightMapImage());
    }

    if (images.size() > 0) {
        _heightmapTextures->create_from_images(images);
    }
}

void ZonesResource::updateColorTextures(int zoneSize) {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (zone->get_colorImage().is_null()) {
            images.append(Image::create_empty(zoneSize, zoneSize, false, Image::Format::FORMAT_RGBA8));
        } else {
            images.append(zone->get_colorImage());
        }
    }

    if (images.size() > 0) {
        _colorTextures->create_from_images(images);
    }
}

void ZonesResource::updateSplatmapsTextures() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (zone->get_splatmapsImage().size() > 0) {
            for (Ref<Image> splatmap : zone->get_splatmapsImage()) {
                images.append(splatmap);
            }
        }
    }

    if (images.size() > 0) {
        _splatmapsTextures->create_from_images(images);
    }
}

void ZonesResource::initializeFoliageTextures(int foliageCount) {
    _foliagesTextures = TypedArray<Ref<Texture2DArray>>();
    for (int i = 0; i < foliageCount; i++) {
        _foliagesTextures.append(memnew(Texture2DArray));
    }
}

void ZonesResource::updateFoliagesTextures() {
    if (_foliagesTextures.size() <= 0) return;

    for (int i = 0; i < _foliagesTextures.size(); i++) {
        updateFoliagesTextures(i);
    }
}

void ZonesResource::updateFoliagesTextures(int foliageIndex) {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        TypedArray<Ref<Image>> foliagesTexture = zone->get_foliagesImage();
        images.append(Ref<Image>(foliagesTexture[foliageIndex]));
    }

    if (images.size() > 0) {
        Ref<Texture2DArray>(_foliagesTextures[foliageIndex])->create_from_images(images);
    }
}

void ZonesResource::updateObjectsTextures() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (zone->get_objectsImage().size() > 0) {
            for (Ref<Image> objectTexture : zone->get_objectsImage()) {
                images.append(objectTexture);
            }
        }
    }

    if (images.size() > 0) {
        _objectsTextures->create_from_images(images);
    }
}

void ZonesResource::updateWaterTextures() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (!zone->get_waterImage().is_null()) {
            images.append(zone->get_waterImage());
        }
    }

    if (images.size() > 0) {
        _waterTextures->create_from_images(images);
    }
}

void ZonesResource::updateZoneWaterTexture(Ref<ZoneResource> zone) {
    int zoneIndex = _zones.find(zone);
    _waterTextures->update_layer(zone->get_waterImage(), zoneIndex);
}

void ZonesResource::updateSnowTextures() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (!zone->get_snowImage().is_null()) {
            images.append(zone->get_snowImage());
        }
    }

    if (images.size() > 0) {
        _snowTextures->create_from_images(images);
    }
}

void ZonesResource::updateZoneSnowTexture(Ref<ZoneResource> zone) {
    int zoneIndex = _zones.find(zone);
    _snowTextures->update_layer(zone->get_snowImage(), zoneIndex);
}

void ZonesResource::updateMetaInfoTextures() {
    TypedArray<Ref<Image>> images = TypedArray<Ref<Image>>();
    for (Ref<ZoneResource> zone : _zones) {
        if (!zone->get_metaInfoImage().is_null()) {
            images.append(zone->get_metaInfoImage());
        }
    }

    if (images.size() > 0) {
        _metaInfoTextures->create_from_images(images);
    }
}

void ZonesResource::updateZoneMetaInfoTexture(Ref<ZoneResource> zone) {
    int zoneIndex = _zones.find(zone);
    _metaInfoTextures->update_layer(zone->get_metaInfoImage(), zoneIndex);
}

void ZonesResource::saveResources() {
    if (_dirtyImages.size() == 0) {
        return;
    }

    for (Ref<Image> dirtyImageResource : _dirtyImages) {
        saveImageResource(dirtyImageResource);
    }

    _dirtyImages.clear();
}

void ZonesResource::saveImageResource(Ref<Image> image) {
    if (!image->get_path().is_empty() && FileAccess::file_exists(image->get_path())) {
        ResourceSaver::get_singleton()->save(image, image->get_path());
    }
}

void ZonesResource::updateZonesMap() {
    TypedArray<Vector2> zonePositions = TypedArray<Vector2>();
    int maxX = 0;
    int maxY = 0;
    for (Ref<ZoneResource> zone : _zones) {
        Vector2 zonePosition = zone->get_zonePosition();
        zonePositions.append(zonePosition);

        maxX = Math::max(maxX, (int) Math::abs(zonePosition.x));
        maxY = Math::max(maxY, (int) Math::abs(zonePosition.y));
    }

    Ref<Image> zonesMap = Image::create_empty((maxX * 2) + 1, (maxY * 2) + 1, false, Image::Format::FORMAT_RF);
    zonesMap->fill(Color(-1, 0, 0, 0));
    for (int i = 0; i < zonePositions.size(); i++) {
        Vector2 position = zonePositions[i];
        zonesMap->set_pixel(position.x + maxX, position.y + maxY, Color(i, 0, 0, 0));
    }

    _zonesMap->set_image(zonesMap);
}

void ZonesResource::addDirtyImage(Ref<Image> imageTexture) {
    _dirtyImages.insert(imageTexture);
}

void ZonesResource::updateImageImages(int zoneSize) {
    if (Engine::get_singleton()->is_editor_hint()) {
        updateLockTexture(zoneSize);
    }
    updateHeightmaps();
    updateColorTextures(zoneSize);
    updateSplatmapsTextures();
    updateFoliagesTextures();
    updateObjectsTextures();
    updateWaterTextures();
    updateSnowTextures();
    updateMetaInfoTextures();
    updateZonesMap();
}

Ref<ZoneResource> ZonesResource::getZoneForZoneInfo(ZoneInfo zoneInfo) {
    if (_zones.size() == 0) {
        return nullptr;
    }

    for (Ref<ZoneResource> zone : _zones) {
        if (zone.is_null()) {
            return nullptr;
        }

        if (zone->get_zonePosition().x == zoneInfo.zonePosition.x && zone->get_zonePosition().y == zoneInfo.zonePosition.y) {
            return zone;
        }
    }

    return nullptr;
}