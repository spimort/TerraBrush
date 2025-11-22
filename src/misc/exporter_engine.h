#ifndef EXPORTER_ENGINE_CONTANTS_H
#define EXPORTER_ENGINE_CONTANTS_H

#include "../terra_brush.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class ExporterEngine {
    public:
        static void exportTerrain(TerraBrush *terrabrush, String dataPath);
};
#endif