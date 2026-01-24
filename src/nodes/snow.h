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
    TypedDictionary<Ref<ZoneResource>, Dictionary> _compressedPositions = TypedDictionary<Ref<ZoneResource>, Dictionary>();

    Clipmap *_clipmap = nullptr;

    int _zonesSize = 0;
    int _resolution = 0;
    Ref<ZonesResource> _terrainZones = nullptr;
    Ref<SnowResource> _snowDefinition = nullptr;
    int _lodLevels = 0;
    int _lodRowsPerLevel = 0;
    float _lodInitialCellWidth = 0;

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