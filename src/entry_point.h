#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_terrabrush_module(ModuleInitializationLevel level);
void uninitialize_terrabrush_module(ModuleInitializationLevel level);

#endif // ENTRY_POINT_H