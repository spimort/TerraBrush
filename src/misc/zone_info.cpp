#include "zone_info.h"

using namespace godot;

void ZoneInfo::_bind_methods() {}

ZoneInfo::ZoneInfo() {}

ZoneInfo::~ZoneInfo() {}

void ZoneInfo::init(int zoneKey, Vector2i zonePosition, Vector2i imagePosition) {
    _zoneKey = zoneKey;
    _zonePosition = zonePosition;
    _imagePosition = imagePosition;
}

int ZoneInfo::get_zoneKey() const {
    return _zoneKey;
}
void ZoneInfo::set_zoneKey(const int value) {
    _zoneKey = value;
}

Vector2i ZoneInfo::get_zonePosition() const {
    return _zonePosition;
}
void ZoneInfo::set_zonePosition(const Vector2i value) {
    _zonePosition = value;
}

Vector2i ZoneInfo::get_imagePosition() const {
    return _imagePosition;
}
void ZoneInfo::set_imagePosition(const Vector2i value) {
    _imagePosition = value;
}
