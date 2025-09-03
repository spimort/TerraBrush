#ifndef ZONE_INFO_H
#define ZONE_INFO_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

struct ZoneInfo {
    int zoneKey = 0;
    Vector2i zonePosition = Vector2i();
    Vector2i imagePosition = Vector2i();

    ZoneInfo() {}
    ZoneInfo(int p_zoneKey, Vector2i p_zonePosition, Vector2i p_imagePosition) {
        zoneKey = p_zoneKey;
        zonePosition = p_zonePosition;
        imagePosition = p_imagePosition;
    }
};
#endif