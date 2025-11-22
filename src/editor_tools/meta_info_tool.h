#ifndef META_INFO_TOOL_H
#define META_INFO_TOOL_H

#include "tool_base.h"
#include "../editor_resources/zone_resource.h"
#include "../misc/hash_utils.h"

using namespace godot;

class MetaInfoTool : public ToolBase{
    GDCLASS(MetaInfoTool, ToolBase);

private:
    int _selectedMetaInfoIndex = -1;

protected:
    static void _bind_methods();

    Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone) override;

public:
    MetaInfoTool();
    ~MetaInfoTool();

    void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) override;

    void updateSelectedMetaInfoIndex(int value);
};
#endif