#ifndef TOOL_BASE_H
#define TOOL_BASE_H

#include "../misc/enums.h"
#include "../misc/zone_info.h"
#include "../editor_resources/zone_resource.h"
#include "../terra_brush.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

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

class ImageZoneInfo : public RefCounted {
    GDCLASS(ImageZoneInfo, RefCounted)

private:
    Ref<Image> _image;
    Ref<ZoneInfo> _zoneInfo;
    Ref<ZoneResource> _zone;
    float _lockedStrength;

protected:
    static void _bind_methods();

public:
    ImageZoneInfo();
    ~ImageZoneInfo();

    Ref<Image> get_image() const;
    void set_image(const Ref<Image> &value);

    Ref<ZoneInfo> get_zoneInfo() const;
    void set_zoneInfo(const Ref<ZoneInfo> &value);

    Ref<ZoneResource> get_zone() const;
    void set_zone(const Ref<ZoneResource> &value);

    float get_lockedStrength() const;
    void set_lockedStrength(const float value);
};

class ToolBase : public RefCounted {
    GDCLASS(ToolBase, RefCounted)

private:
    TerraBrush *_terraBrush;
    EditorUndoRedoManager *_undoRedoManager;

    LockedAxis _lockedAxis;
    Vector2 _lockedAxisValue;
    TypedDictionary<Ref<ZoneResource>, Ref<Image>> _imagesCache;
    TypedDictionary<int, Ref<ZoneResource>> _zonesPositionCache;
    TypedArray<Ref<ImageTexture>> _modifiedUndoTextures;
    bool _autoAddZones;

    PixelLockedInfo isZonePixelLocked(Ref<ZoneResource> zone, Ref<ZoneInfo> zoneInfo);
    void addImagesToRedo();
    Ref<Image> getUndoRedoImageFromTexture(Ref<ImageTexture> imageTexture);
    int getResolution();

protected:
    static void _bind_methods();

    virtual bool getApplyResolution();
    virtual Ref<ImageTexture> getToolCurrentImageTexture(Ref<ZoneResource> zone);
    void forEachBrushPixel(Ref<Image> brushImage, int brushSize, Vector2 imagePosition, Callable onBrushPixel, bool ignoreLockedZone = false);
    void addTextureToUndo(Ref<ImageTexture> texture);
    Ref<ImageZoneInfo> getImageZoneInfoForPosition(Ref<ZoneInfo> startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone = false);

public:
    ToolBase();
    ~ToolBase();

    void init(TerraBrush *terraBrush, EditorUndoRedoManager *undoRedoManager, bool autoAddZones);
    virtual void beforeDeselect();
    virtual String getToolInfo(TerrainToolType toolType);
    virtual bool handleInput(TerrainToolType toolType, Ref<InputEvent> event);
    virtual void beginPaint();
    virtual void paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition);
    virtual void endPaint();

    void set_autoAddZones(bool value);
};
#endif