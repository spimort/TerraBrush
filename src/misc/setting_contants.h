#ifndef SETTING_CONTANTS_H
#define SETTING_CONTANTS_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class SettingContants {
    public:
        static StringName DecalColor();
        static StringName CustomBrushesFolder();
        static StringName SculptingMultiplier();
        static StringName IconsColor();
};
#endif