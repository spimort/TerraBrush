#include "zone_resource.h"

using namespace godot;

void ZoneResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_zonePosition"), &ZoneResource::get_zonePosition);
    ClassDB::bind_method(D_METHOD("set_zonePosition", "value"), &ZoneResource::set_zonePosition);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "zonePosition"), "set_zonePosition", "get_zonePosition");

    ClassDB::bind_method(D_METHOD("get_heightMapImage"), &ZoneResource::get_heightMapImage);
    ClassDB::bind_method(D_METHOD("set_heightMapImage", "value"), &ZoneResource::set_heightMapImage);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "heightMapImage", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_heightMapImage", "get_heightMapImage");

    ClassDB::bind_method(D_METHOD("get_splatmapsImage"), &ZoneResource::get_splatmapsImage);
    ClassDB::bind_method(D_METHOD("set_splatmapsImage", "value"), &ZoneResource::set_splatmapsImage);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "splatmapsImage", PROPERTY_HINT_TYPE_STRING, "24/17:Image"), "set_splatmapsImage", "get_splatmapsImage");

    ClassDB::bind_method(D_METHOD("get_foliagesImage"), &ZoneResource::get_foliagesImage);
    ClassDB::bind_method(D_METHOD("set_foliagesImage", "value"), &ZoneResource::set_foliagesImage);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "foliagesImage", PROPERTY_HINT_TYPE_STRING, "24/17:Image"), "set_foliagesImage", "get_foliagesImage");

    ClassDB::bind_method(D_METHOD("get_objectsImage"), &ZoneResource::get_objectsImage);
    ClassDB::bind_method(D_METHOD("set_objectsImage", "value"), &ZoneResource::set_objectsImage);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "objectsImage", PROPERTY_HINT_TYPE_STRING, "24/17:Image"), "set_objectsImage", "get_objectsImage");

    ClassDB::bind_method(D_METHOD("get_waterImage"), &ZoneResource::get_waterImage);
    ClassDB::bind_method(D_METHOD("set_waterImage", "value"), &ZoneResource::set_waterImage);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "waterImage", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_waterImage", "get_waterImage");

    ClassDB::bind_method(D_METHOD("get_snowImage"), &ZoneResource::get_snowImage);
    ClassDB::bind_method(D_METHOD("set_snowImage", "value"), &ZoneResource::set_snowImage);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "snowImage", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_snowImage", "get_snowImage");

    ClassDB::bind_method(D_METHOD("get_metaInfoImage"), &ZoneResource::get_metaInfoImage);
    ClassDB::bind_method(D_METHOD("set_metaInfoImage", "value"), &ZoneResource::set_metaInfoImage);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "metaInfoImage", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_metaInfoImage", "get_metaInfoImage");
}

ZoneResource::ZoneResource() {
    _lockImage = Ref<Image>(nullptr);
    _zonePosition = Vector2i();
    _heightMapImage = Ref<Image>(nullptr);
    _splatmapsImage = TypedArray<Ref<Image>>();
    _foliagesImage = TypedArray<Ref<Image>>();
    _objectsImage = TypedArray<Ref<Image>>();
    _waterImage = Ref<Image>(nullptr);
    _snowImage = Ref<Image>(nullptr);
    _metaInfoImage = Ref<Image>(nullptr);
}

ZoneResource::~ZoneResource() {}

Ref<Image> ZoneResource::get_lockTexture() const {
    return _lockImage;
}
void ZoneResource::set_lockTexture(const Ref<Image> &value) {
    _lockImage = value;
}

Vector2i ZoneResource::get_zonePosition() const {
    return _zonePosition;
}
void ZoneResource::set_zonePosition(const Vector2i value) {
    _zonePosition = value;
}

Ref<Image> ZoneResource::get_heightMapImage() const {
    return _heightMapImage;
}
void ZoneResource::set_heightMapImage(const Ref<Image> &value) {
    _heightMapImage = value;
}

TypedArray<Ref<Image>> ZoneResource::get_splatmapsImage() const {
    return _splatmapsImage;
}
void ZoneResource::set_splatmapsImage(const TypedArray<Ref<Image>> value) {
    _splatmapsImage = value;
}

TypedArray<Ref<Image>> ZoneResource::get_foliagesImage() const {
    return _foliagesImage;
}
void ZoneResource::set_foliagesImage(const TypedArray<Ref<Image>> value) {
    _foliagesImage = value;
}

TypedArray<Ref<Image>> ZoneResource::get_objectsImage() const {
    return _objectsImage;
}
void ZoneResource::set_objectsImage(const TypedArray<Ref<Image>> value) {
    _objectsImage = value;
}

Ref<Image> ZoneResource::get_waterImage() const {
    return _waterImage;
}
void ZoneResource::set_waterImage(const Ref<Image> &value) {
    _waterImage = value;
}

Ref<Image> ZoneResource::get_snowImage() const {
    return _snowImage;
}
void ZoneResource::set_snowImage(const Ref<Image> &value) {
    _snowImage = value;
}

Ref<Image> ZoneResource::get_metaInfoImage() const {
    return _metaInfoImage;
}
void ZoneResource::set_metaInfoImage(const Ref<Image> &value) {
    _metaInfoImage = value;
}