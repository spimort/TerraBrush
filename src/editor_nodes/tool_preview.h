#ifndef TOOL_PREVIEW_H
#define TOOL_PREVIEW_H

#include "../terra_brush.h"
#include "dock_preview_button.h"
#include "../misc/enums.h"

using namespace godot;

class ToolPreview : public DockPreviewButton {
    GDCLASS(ToolPreview, DockPreviewButton)

private:
    TerrainToolType _toolType;

protected:
    static void _bind_methods();

public:
    ToolPreview();
    ~ToolPreview();

    TerrainToolType get_toolType() const;
    void set_toolType(const TerrainToolType value);
};
#endif