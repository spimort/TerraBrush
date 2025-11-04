#ifndef TOOL_BASE_H
#define TOOL_BASE_H

#include "../misc/enums.h"
#include "../misc/zone_info.h"
#include "../editor_resources/zone_resource.h"
#include "../terra_brush.h"
#include "../misc/hash_utils.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

#include <map>
#include <functional>

using namespace godot;

struct PixelLockedInfo {
    bool locked = false;
    float lockedStrength = 0;

    PixelLockedInfo() {}
    PixelLockedInfo(bool p_locked, float p_lockedStrength) {
        locked = p_locked;
        lockedStrength = p_lockedStrength;
    }
};

struct ImageZoneInfo {
    Ref<Image> image = nullptr;
    ZoneInfo zoneInfo = ZoneInfo();
    Ref<ZoneResource> zone = nullptr;
    float lockedStrength = 0;

    ImageZoneInfo() {}
    ImageZoneInfo(Ref<Image> p_image, ZoneInfo &p_zoneInfo, Ref<ZoneResource> p_zone, float p_lockedStrength) {
        image = p_image;
        zoneInfo = p_zoneInfo;
        zone = p_zone;
        lockedStrength = p_lockedStrength;
    }
};

class ToolBase : public RefCounted {
    GDCLASS(ToolBase, RefCounted);

private:
    EditorUndoRedoManager *_undoRedoManager;

    LockedAxis _lockedAxis = LockedAxis::LOCKEDAXIS_NONE;
    Vector2 _lockedAxisValue = Vector2();
    std::unordered_map<int, Ref<ZoneResource>> _zonesPositionCache = std::unordered_map<int, Ref<ZoneResource>>();
    std::unordered_set<Ref<Image>> _modifiedUndoImages = std::unordered_set<Ref<Image>>();
    bool _autoAddZones = false;

    PixelLockedInfo isZonePixelLocked(Ref<ZoneResource> zone, ZoneInfo &zoneInfo);
    void addImagesToRedo();
    int getResolution() const;

protected:
    TerraBrush *_terraBrush = nullptr;

    static void _bind_methods();

    virtual bool getApplyResolution() const;
    virtual Ref<Image> getToolCurrentImage(Ref<ZoneResource> zone);
    void forEachBrushPixel(Ref<Image> brushImage, int brushSize, Vector2 imagePosition, std::function<void(ImageZoneInfo&, float)> onBrushPixel, bool ignoreLockedZone = false);
    void addImageToUndo(Ref<Image> image);
    ImageZoneInfo getImageZoneInfoForPosition(ZoneInfo &startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone = false);

public:
    ToolBase();
    ~ToolBase();

    virtual void init(TerraBrush *terraBrush, EditorUndoRedoManager *undoRedoManager, bool autoAddZones);
    virtual void beforeDeselect();
    virtual String getToolInfo(TerrainToolType toolType);
    virtual bool handleInput(TerrainToolType toolType, Ref<InputEvent> event);
    virtual void beginPaint();
    virtual void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition);
    virtual void endPaint();

    void set_autoAddZones(bool value);
};
#endif