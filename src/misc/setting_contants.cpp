#include "setting_contants.h"

using namespace godot;

const StringName SettingContants::DecalColor() { return "terraBrush/brushes/decal_color"; }
const StringName SettingContants::CustomBrushesFolder() { return "terraBrush/brushes/custom_brushes_folder"; }
const StringName SettingContants::SculptingMultiplier() { return "terraBrush/brushes/sculpting_multiplier"; }
const StringName SettingContants::IconsColor() { return "terraBrush/interface/icons_color"; }

const Color SettingContants::DecalColorDefaultValue() { return Color::html("#244ab580"); }
const StringName SettingContants::CustomBrushesFolderDefaultValue() { return "res://TerraBrush_CustomBrushes"; }
const int SettingContants::SculptingMultiplierDefaultValue() { return 10; }
const Color SettingContants::IconsColorDefaultValue() { return Color::html("#00151F"); }