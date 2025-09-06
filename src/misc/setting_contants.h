#ifndef SETTING_CONTANTS_H
#define SETTING_CONTANTS_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class SettingContants {
    public:
        const static StringName DecalColor();
        const static StringName CustomBrushesFolder();
        const static StringName SculptingMultiplier();
        const static StringName IconsColor();
};
#endif