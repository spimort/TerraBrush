#include "tool_preview.h"

using namespace godot;

void ToolPreview::_bind_methods() {

}

ToolPreview::ToolPreview() {

}

ToolPreview::~ToolPreview() {}

TerraBrush::TerrainToolType ToolPreview::get_toolType() const {
    return _toolType;
}
void ToolPreview::set_toolType(const TerraBrush::TerrainToolType value) {
    _toolType = value;
}