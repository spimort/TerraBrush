#ifndef IMPORTER_ENGINE_H
#define IMPORTER_ENGINE_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

struct ImporterSettings {
    Ref<Texture2D> heightmap = nullptr;
    bool useGreenChannelForHoles = false;
    bool scaleToResolution = true;
    float heightmapScale = 1.0;
    TypedArray<Ref<Texture2D>> splatmaps = TypedArray<Ref<Texture2D>>();
    TypedArray<Ref<Texture2D>> foliages = TypedArray<Ref<Texture2D>>();
    TypedArray<Ref<Texture2D>> objects = TypedArray<Ref<Texture2D>>();
    Ref<Texture2D> water = nullptr;
    Ref<Texture2D> snow = nullptr;
    Ref<Texture2D> metaInfo = nullptr;
};
#endif