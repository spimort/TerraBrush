#ifndef ZONE_INFO_H
#define ZONE_INFO_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class ZoneInfo : public RefCounted{
    GDCLASS(ZoneInfo, RefCounted);

private:
    int _zoneKey = 0;
    Vector2i _zonePosition = Vector2i();
    Vector2i _imagePosition = Vector2i();

protected:
    static void _bind_methods();

public:
    ZoneInfo();
    ~ZoneInfo();

    void init(int zoneKey, Vector2i zonePosition, Vector2i imagePosition);

    int get_zoneKey() const;
    void set_zoneKey(const int value);
    Vector2i get_zonePosition() const;
    void set_zonePosition(const Vector2i value);
    Vector2i get_imagePosition() const;
    void set_imagePosition(const Vector2i value);
};
#endif