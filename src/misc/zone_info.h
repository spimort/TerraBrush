#ifndef ZONE_INFO_H
#define ZONE_INFO_H

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

struct ZoneInfo {
    int zoneKey;
    Vector2i zonePosition;
    Vector2i imagePosition;

    ZoneInfo() {}
    ZoneInfo(int p_zoneKey, Vector2i p_zonePosition, Vector2i p_imagePosition) {
        zoneKey = p_zoneKey;
        zonePosition = p_zonePosition;
        imagePosition = p_imagePosition;
    }
};
#endif