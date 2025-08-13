#ifndef SETTING_CONTANTS_H
#define SETTING_CONTANTS_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class SettingContants {
    public:
        static StringName get_decalColor();
        static StringName get_customBrushesFolder();
        static StringName get_sculptingMultiplier();
        static StringName get_iconsColor();
};
#endif