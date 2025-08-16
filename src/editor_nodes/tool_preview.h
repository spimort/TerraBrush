#ifndef TOOL_PREVIEW_H
#define TOOL_PREVIEW_H

#include "../terra_brush.h"
#include "dock_preview_button.h"

using namespace godot;

class ToolPreview : public DockPreviewButton {
    GDCLASS(ToolPreview, DockPreviewButton)

private:
    TerraBrush::TerrainToolType _toolType;

protected:
    static void _bind_methods();

public:
    ToolPreview();
    ~ToolPreview();

    TerraBrush::TerrainToolType get_toolType() const;
    void set_toolType(const TerraBrush::TerrainToolType value);
};
#endif