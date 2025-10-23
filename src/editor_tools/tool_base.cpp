#include "tool_base.h"
#include "../misc/zone_utils.h"
#include "../misc/keybind_manager.h"

#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void ToolBase::_bind_methods() {}

ToolBase::ToolBase() {
    _lockedAxis = LockedAxis::LOCKEDAXIS_NONE;
    _lockedAxisValue = Vector2();
}

ToolBase::~ToolBase() {}

void ToolBase::init(TerraBrush *terraBrush, EditorUndoRedoManager *undoRedoManager, bool autoAddZones) {
    ERR_FAIL_COND_MSG(terraBrush == nullptr, "A terrabrush node must be provided");
    ERR_FAIL_COND_MSG(undoRedoManager == nullptr, "A undoRedoManager must be provided");

    _terraBrush = terraBrush;
    _undoRedoManager = undoRedoManager;
    _autoAddZones = autoAddZones;
}

PixelLockedInfo ToolBase::isZonePixelLocked(Ref<ZoneResource> zone, ZoneInfo &zoneInfo) {
    Ref<Image> image = zone->get_lockTexture();
    Color pixel = image->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
    return PixelLockedInfo(pixel.r == 1.0, pixel.r);
}

void ToolBase::addImagesToRedo() {
    for (Ref<Image> image : _modifiedUndoImages) {
        _undoRedoManager->add_do_method(image.ptr(), "set_data", image->get_width(), image->get_height(), image->has_mipmaps(), image->get_format(), image->get_data());
    }
}

int ToolBase::getResolution() const {
    if (getApplyResolution()) {
        return _terraBrush->get_resolution();
    }

    return 1;
}

void ToolBase::addImageToUndo(Ref<Image> image) {
    if (_modifiedUndoImages.count(image) == 0) {
        _modifiedUndoImages.insert(image);

        _undoRedoManager->add_undo_method(image.ptr(), "set_data", image->get_width(), image->get_height(), image->has_mipmaps(), image->get_format(), image->get_data());
    }
}

bool ToolBase::getApplyResolution() const {
    return false;
}

Ref<Image> ToolBase::getToolCurrentImage(Ref<ZoneResource> zone) {
    return nullptr;
}

void ToolBase::forEachBrushPixel(Ref<Image> brushImage, int brushSize, Vector2 imagePosition, std::function<void(ImageZoneInfo&, float)> onBrushPixel, bool ignoreLockedZone) {
    if (_lockedAxis != LockedAxis::LOCKEDAXIS_NONE) {
        if (_lockedAxisValue == Vector2(0, 0)) {
            _lockedAxisValue = Vector2(imagePosition.x, imagePosition.y);
        } else {
            if (_lockedAxis == LockedAxis::LOCKEDAXIS_X) {
                imagePosition = Vector2(imagePosition.x, _lockedAxisValue.y);
            } else {
                imagePosition = Vector2(_lockedAxisValue.x, imagePosition.y);
            }
        }
    }

    float startingX = imagePosition.x - (brushSize / 2);
    float startingY = imagePosition.y - (brushSize / 2);
    ZoneInfo startingZoneInfo = ZoneUtils::getPixelToZoneInfo(startingX, startingY, _terraBrush->get_zonesSize(), getResolution());

    std::unordered_set<int> pointsCache = std::unordered_set<int>();
    for (int x = 0; x < brushSize; x++) {
        for (int y = 0; y < brushSize; y++) {
            int offsetX = x;
            int offsetY = y;
            if (_terraBrush->get_resolution() != 1 && getApplyResolution()) {
                offsetX = (int) Math::floor(Math::remap((float) x, 0, brushSize, 0, (int) Math::ceil(((float) brushSize) / _terraBrush->get_resolution())));
                offsetY = (int) Math::floor(Math::remap((float) y, 0, brushSize, 0, (int) Math::ceil(((float) brushSize) / _terraBrush->get_resolution())));
            }
            ImageZoneInfo imageZoneInfo = getImageZoneInfoForPosition(startingZoneInfo, offsetX, offsetY, ignoreLockedZone);

            if (!imageZoneInfo.zone.is_null()) {
                int zoneKey = imageZoneInfo.zoneInfo.zoneKey;
                Vector2i zoneImagePosition = imageZoneInfo.zoneInfo.imagePosition;
                int positionKey = (zoneImagePosition.x << 8) + zoneImagePosition.y;
                // Create a cache key with the zone and the position
                int zonePositionKey = (zoneKey << 8) + positionKey;

                if (_terraBrush->get_resolution() == 1 || !getApplyResolution() || pointsCache.count(zonePositionKey) == 0) {
                    if (_terraBrush->get_resolution() != 1) {
                        pointsCache.insert(zonePositionKey);
                    }

                    Color brushPixelValue = brushImage->get_pixel(x, y);
                    float brushPixelStrength = brushPixelValue.a * (1.0f - imageZoneInfo.lockedStrength);

                    onBrushPixel(imageZoneInfo, brushPixelStrength);
                }
            }
        }
    }
}

ImageZoneInfo ToolBase::getImageZoneInfoForPosition(ZoneInfo &startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone) {
    ZoneInfo zoneInfo = ZoneUtils::getZoneInfoFromZoneOffset(startingZoneInfo, Vector2i(offsetX, offsetY), _terraBrush->get_zonesSize(), getResolution());
    Ref<ZoneResource> zone = nullptr;
    if (_zonesPositionCache.count(zoneInfo.zoneKey) > 0) {
        zone = _zonesPositionCache[zoneInfo.zoneKey];
    }

    if (zone.is_null()) {
        zone = _terraBrush->get_terrainZones()->getZoneForZoneInfo(zoneInfo);

        if (!zone.is_null()) {
            _zonesPositionCache[zoneInfo.zoneKey] = zone;
        }
    }

    if (zone.is_null() && _autoAddZones) {
        zone = _terraBrush->addNewZone(zoneInfo.zonePosition);

        if (!zone.is_null()) {
            _terraBrush->get_terrain()->addZoneCollision(zone);
            _zonesPositionCache[zoneInfo.zoneKey] = zone;
        }
    }

    if (!zone.is_null()) {
        PixelLockedInfo lockInfo = PixelLockedInfo(false, 0.0f);
        if (!ignoreLockedZone && !zone->get_lockTexture().is_null()) {
            ZoneInfo &lockZoneInfo = zoneInfo;
            if (_terraBrush->get_resolution() != 1 && getApplyResolution()) {
                ZoneInfo resolutionZoneInfo = ZoneInfo();
                resolutionZoneInfo.imagePosition = startingZoneInfo.imagePosition * _terraBrush->get_resolution();

                lockZoneInfo = ZoneUtils::getZoneInfoFromZoneOffset(
                    resolutionZoneInfo,
                    Vector2i(offsetX * _terraBrush->get_resolution(), offsetY * _terraBrush->get_resolution()),
                    _terraBrush->get_zonesSize(),
                    1
                );
            }
            lockInfo = isZonePixelLocked(zone, lockZoneInfo);
        }

        if (!lockInfo.locked) {
            Ref<Image> image = getToolCurrentImage(zone);
            if (!image.is_null()) {
                _terraBrush->get_terrainZones()->addDirtyImage(image);
                addImageToUndo(image);
            }

            return ImageZoneInfo(image, zoneInfo, zone, lockInfo.lockedStrength);
        }
    }

    return ImageZoneInfo();
}

void ToolBase::beforeDeselect() {
    // Nothing to do in the base class
}

String ToolBase::getToolInfo(TerrainToolType toolType) {
    if (_lockedAxis != LockedAxis::LOCKEDAXIS_NONE) {
        return "Locked axis : " + String(_lockedAxis == LockedAxis::LOCKEDAXIS_X ? "x" : "z");
    }

    return "";
}

bool ToolBase::handleInput(TerrainToolType toolType, Ref<InputEvent> event) {
    if (!Input::get_singleton()->is_key_pressed(Key::KEY_CTRL) && Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
        Ref<InputEvent> inputEvent = Object::cast_to<InputEventKey>(event.ptr());

        if (inputEvent->is_action(KeybindManager::StringNames::LockXAxis())) {
            if (_lockedAxis != LockedAxis::LOCKEDAXIS_X) {
                _lockedAxis = LockedAxis::LOCKEDAXIS_X;
            } else {
                _lockedAxis = LockedAxis::LOCKEDAXIS_NONE;
                _lockedAxisValue = Vector2();
            }

            return true;
        } else if (inputEvent->is_action(KeybindManager::StringNames::LockZAxis())) {
            if (_lockedAxis != LockedAxis::LOCKEDAXIS_Z) {
                _lockedAxis = LockedAxis::LOCKEDAXIS_Z;
            } else {
                _lockedAxis = LockedAxis::LOCKEDAXIS_NONE;
                _lockedAxisValue = Vector2();
            }

            return true;
        }
    }

    return false;
}

void ToolBase::beginPaint() {
    _zonesPositionCache = std::unordered_map<int, Ref<ZoneResource>>();
    _modifiedUndoImages = std::unordered_set<Ref<Image>>();
}

void ToolBase::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    // Nothing to do in the base class
}

void ToolBase::endPaint() {
    _zonesPositionCache.clear();

    addImagesToRedo();
    _modifiedUndoImages.clear();

    _lockedAxisValue = Vector2();
}

void ToolBase::set_autoAddZones(bool value) {
    _autoAddZones = value;
}