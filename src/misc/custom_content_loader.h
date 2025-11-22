#ifndef CUSTOM_CONTENT_LOADER_H
#define CUSTOM_CONTENT_LOADER_H

#include "../terra_brush.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class CustomContentLoader {
    public:
        static void addBrushesPreviewToParent(Node *parentNode, Callable onSelect, bool useCircleIcon = false);
        static void addTexturesPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon = false);
        static void addFoliagesPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon = false);
        static void addObjectsPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon = false);
        static void addMetaInfoLayersPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon = false);
};
#endif