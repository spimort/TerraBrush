#include "tool_preview.h"
#include "../misc/enums.h"

using namespace godot;

void ToolPreview::_bind_methods() {

}

ToolPreview::ToolPreview() {

}

ToolPreview::~ToolPreview() {}

TerrainToolType ToolPreview::get_toolType() const {
    return _toolType;
}
void ToolPreview::set_toolType(const TerrainToolType value) {
    _toolType = value;
}