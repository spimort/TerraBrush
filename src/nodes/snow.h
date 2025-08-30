#ifndef SNOW_H
#define SNOW_H

#include "../editor_resources/zone_resource.h"
#include "../editor_resources/snow_resource.h"
#include "../nodes/clipmap.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Snow : public Node3D {
    GDCLASS(Snow, Node3D);

private:
    const float DeCompressSpeed = 0.5f;
    const float DeCompressOffsetSpeed = 2.0f;

    TypedDictionary<Ref<ZoneResource>, Dictionary> _compressedPositions;
    TypedDictionary<Ref<ZoneResource>, Ref<Image>> _imagesCache;

    Clipmap *_clipmap;

    int _zonesSize;
    int _resolution;
    Ref<ZonesResource> _terrainZones;
    Ref<SnowResource> _snowDefinition;
    int _lodLevels;
    int _lodRowsPerLevel;
    float _lodInitialCellWidth;

    Ref<Image> getImageForZone(Ref<ZoneResource> &zone);

protected:
    static void _bind_methods();

public:
    Snow();
    ~Snow();

    void _ready() override;
    void _physics_process(const double delta) override;

    int get_zonesSize() const;
    void set_zonesSize(const int value);

    int get_resolution() const;
    void set_resolution(const int value);

    Ref<ZonesResource> get_terrainZones() const;
    void set_terrainZones(const Ref<ZonesResource> &value);

    Ref<SnowResource> get_snowDefinition() const;
    void set_snowDefinition(const Ref<SnowResource> &value);

    int get_lodLevels() const;
    void set_lodLevels(const int value);

    int get_lodRowsPerLevel() const;
    void set_lodRowsPerLevel(const int value);

    float get_lodInitialCellWidth() const;
    void set_lodInitialCellWidth(const float value);

    Clipmap *get_clipmap() const;

    void updateSnow();
    void addCompressedSnow(float x, float y);
};
#endif