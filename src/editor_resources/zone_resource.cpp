#include "zone_resource.h"

using namespace godot;

void ZoneResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_zonePosition"), &ZoneResource::get_zonePosition);
    ClassDB::bind_method(D_METHOD("set_zonePosition", "value"), &ZoneResource::set_zonePosition);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "zonePosition"), "set_zonePosition", "get_zonePosition");

    ClassDB::bind_method(D_METHOD("get_heightMapTexture"), &ZoneResource::get_heightMapTexture);
    ClassDB::bind_method(D_METHOD("set_heightMapTexture", "value"), &ZoneResource::set_heightMapTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "heightMapTexture", PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"), "set_heightMapTexture", "get_heightMapTexture");

    ClassDB::bind_method(D_METHOD("get_splatmapsTexture"), &ZoneResource::get_splatmapsTexture);
    ClassDB::bind_method(D_METHOD("set_splatmapsTexture", "value"), &ZoneResource::set_splatmapsTexture);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "splatmapsTexture", PROPERTY_HINT_TYPE_STRING, "24/17:ImageTexture"), "set_splatmapsTexture", "get_splatmapsTexture");

    ClassDB::bind_method(D_METHOD("get_foliagesTexture"), &ZoneResource::get_foliagesTexture);
    ClassDB::bind_method(D_METHOD("set_foliagesTexture", "value"), &ZoneResource::set_foliagesTexture);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "foliagesTexture", PROPERTY_HINT_TYPE_STRING, "24/17:ImageTexture"), "set_foliagesTexture", "get_foliagesTexture");

    ClassDB::bind_method(D_METHOD("get_objectsTexture"), &ZoneResource::get_objectsTexture);
    ClassDB::bind_method(D_METHOD("set_objectsTexture", "value"), &ZoneResource::set_objectsTexture);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "objectsTexture", PROPERTY_HINT_TYPE_STRING, "24/17:ImageTexture"), "set_objectsTexture", "get_objectsTexture");

    ClassDB::bind_method(D_METHOD("get_waterTexture"), &ZoneResource::get_waterTexture);
    ClassDB::bind_method(D_METHOD("set_waterTexture", "value"), &ZoneResource::set_waterTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterTexture", PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"), "set_waterTexture", "get_waterTexture");

    ClassDB::bind_method(D_METHOD("get_snowTexture"), &ZoneResource::get_snowTexture);
    ClassDB::bind_method(D_METHOD("set_snowTexture", "value"), &ZoneResource::set_snowTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowTexture", PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"), "set_snowTexture", "get_snowTexture");

    ClassDB::bind_method(D_METHOD("get_metaInfoTexture"), &ZoneResource::get_metaInfoTexture);
    ClassDB::bind_method(D_METHOD("set_metaInfoTexture", "value"), &ZoneResource::set_metaInfoTexture);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "metaInfoTexture", PROPERTY_HINT_RESOURCE_TYPE, "ImageTexture"), "set_metaInfoTexture", "get_metaInfoTexture");
}

ZoneResource::ZoneResource() {
    _lockTexture = Ref<ImageTexture>(nullptr);
    _zonePosition = Vector2i();
    _heightMapTexture = Ref<ImageTexture>(nullptr);
    _splatmapsTexture = TypedArray<Ref<ImageTexture>>();
    _foliagesTexture = TypedArray<Ref<ImageTexture>>();
    _objectsTexture = TypedArray<Ref<ImageTexture>>();
    _waterTexture = Ref<ImageTexture>(nullptr);
    _snowTexture = Ref<ImageTexture>(nullptr);
    _metaInfoTexture = Ref<ImageTexture>(nullptr);
}

ZoneResource::~ZoneResource() {}

Ref<ImageTexture> ZoneResource::get_lockTexture() const {
    return _lockTexture;
}
void ZoneResource::set_lockTexture(const Ref<ImageTexture> &value) {
    _lockTexture = value;
}

Vector2i ZoneResource::get_zonePosition() const {
    return _zonePosition;
}
void ZoneResource::set_zonePosition(const Vector2i value) {
    _zonePosition = value;
}

Ref<ImageTexture> ZoneResource::get_heightMapTexture() const {
    return _heightMapTexture;
}
void ZoneResource::set_heightMapTexture(const Ref<ImageTexture> &value) {
    _heightMapTexture = value;
}

TypedArray<Ref<ImageTexture>> ZoneResource::get_splatmapsTexture() const {
    return _splatmapsTexture;
}
void ZoneResource::set_splatmapsTexture(const TypedArray<Ref<ImageTexture>> value) {
    _splatmapsTexture = value;
}

TypedArray<Ref<ImageTexture>> ZoneResource::get_foliagesTexture() const {
    return _foliagesTexture;
}
void ZoneResource::set_foliagesTexture(const TypedArray<Ref<ImageTexture>> value) {
    _foliagesTexture = value;
}

TypedArray<Ref<ImageTexture>> ZoneResource::get_objectsTexture() const {
    return _objectsTexture;
}
void ZoneResource::set_objectsTexture(const TypedArray<Ref<ImageTexture>> value) {
    _objectsTexture = value;
}

Ref<ImageTexture> ZoneResource::get_waterTexture() const {
    return _waterTexture;
}
void ZoneResource::set_waterTexture(const Ref<ImageTexture> &value) {
    _waterTexture = value;
}

Ref<ImageTexture> ZoneResource::get_snowTexture() const {
    return _snowTexture;
}
void ZoneResource::set_snowTexture(const Ref<ImageTexture> &value) {
    _snowTexture = value;
}

Ref<ImageTexture> ZoneResource::get_metaInfoTexture() const {
    return _metaInfoTexture;
}
void ZoneResource::set_metaInfoTexture(const Ref<ImageTexture> &value) {
    _metaInfoTexture = value;
}

// TODO : GDExtension
void ZoneResource::initializeImagesForTerrain() {
//     HeightMapTexture = ZoneUtils.CreateHeightmapImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);

//     var numberOfSplatmaps = Mathf.CeilToInt((terraBrush.TextureSets?.TextureSets?.Count ?? 0) / 4.0f);
//     var splatmaps = new GodotArray<ImageTexture>();
//     for (var i = 0; i < numberOfSplatmaps; i++) {
//         splatmaps.Add(ZoneUtils.CreateSplatmapImage(terraBrush.ZonesSize, ZonePosition, i, terraBrush.DataPath));
//     }
//     SplatmapsTexture = splatmaps;

//     if (terraBrush.Foliages != null) {
//         FoliagesTexture = new GodotArray<ImageTexture>([..terraBrush.Foliages.Select((foliage, index) => {
//             return ZoneUtils.CreateFoliageImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
//         })]);
//     }

//     if (terraBrush.Objects != null) {
//         ObjectsTexture = new GodotArray<ImageTexture>([..terraBrush.Objects.Select((objectItem, index) => {
//             return ZoneUtils.CreateObjectImage(terraBrush.ZonesSize, ZonePosition, index, terraBrush.DataPath);
//         })]);
//     }

//     if (terraBrush.WaterDefinition != null) {
//         WaterTexture = ZoneUtils.CreateWaterImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
//     }

//     if (terraBrush.SnowDefinition != null) {
//         SnowTexture = ZoneUtils.CreateSnowImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
//     }

//     if (terraBrush.MetaInfoLayers?.Count > 0) {
//         MetaInfoTexture = ZoneUtils.CreateMetaInfoImage(terraBrush.ZonesSize, terraBrush.Resolution, ZonePosition, terraBrush.DataPath);
//     }
}