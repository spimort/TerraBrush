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

void ToolBase::init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones, int maxBrushSize) {
    ERR_FAIL_COND_MSG(terraBrush == nullptr, "A terrabrush node must be provided");

    _terraBrush = terraBrush;
    _undoRedo = undoRedo;
    _autoAddZones = autoAddZones;
    _maxBrushSize = maxBrushSize;
}

PixelLockedInfo ToolBase::isZonePixelLocked(Ref<ZoneResource> zone, ZoneInfo &zoneInfo) {
    Ref<Image> image = zone->get_lockTexture();
    Color pixel = image->get_pixel(zoneInfo.imagePosition.x, zoneInfo.imagePosition.y);
    return PixelLockedInfo(pixel.r == 1.0, pixel.r);
}

void ToolBase::addImagesToRedo() {
    if (!_undoRedo->canUndoRedo()) {
        return;
    }

    for (Ref<Image> image : _modifiedUndoImages) {
        _undoRedo->add_do_method(image.ptr(), "set_data", image->get_width(), image->get_height(), image->has_mipmaps(), image->get_format(), image->get_data());
    }
}

int ToolBase::getResolution() const {
    if (getApplyResolution()) {
        return _terraBrush->get_resolution();
    }

    return 1;
}

void ToolBase::addImageToUndo(Ref<Image> image) {
    if (!_undoRedo->canUndoRedo()) {
        return;
    }

    if (_modifiedUndoImages.count(image) == 0) {
        _modifiedUndoImages.insert(image);

        _undoRedo->add_undo_method(image.ptr(), "set_data", image->get_width(), image->get_height(), image->has_mipmaps(), image->get_format(), image->get_data());
    }
}

bool ToolBase::getApplyResolution() const {
    return false;
}

Ref<Image> ToolBase::getToolCurrentImage(Ref<ZoneResource> zone) {
    return nullptr;
}

void ToolBase::forEachBrushPixel(Ref<Image> brushImage, int brushSize, Vector2 slopeValue, Vector2 imagePosition, std::function<void(ImageZoneInfo&, float)> onBrushPixel, bool ignoreLockedZone) {
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

    float startingX = imagePosition.x - (brushSize / 2.0);
    float startingY = imagePosition.y - (brushSize / 2.0);
    ZoneInfo startingZoneInfo = ZoneUtils::getPixelToZoneInfo(startingX, startingY, _terraBrush->get_zonesSize(), getResolution());

    std::unordered_set<uint64_t> pointsCache = std::unordered_set<uint64_t>();
    std::unordered_map<uint64_t, float> slopesCache = std::unordered_map<uint64_t, float>();
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
                // Create a cache key with the zone and the position
                uint64_t zonePositionKey = getZonePositionKeyForZoneInfo(imageZoneInfo.zoneInfo);

                if (_terraBrush->get_resolution() == 1 || !getApplyResolution() || pointsCache.count(zonePositionKey) == 0) {
                    if (_terraBrush->get_resolution() != 1) {
                        pointsCache.insert(zonePositionKey);
                    }

                    bool skipPixel = false;
                    if (slopeValue.x != 0.0 || slopeValue.y != 1.0) {
                        float slope = 0.0;
                        if (slopesCache.count(zonePositionKey) == 0) {
                            ZoneInfo hLZone = getImageZoneInfoForPosition(startingZoneInfo, offsetX - 1.0, offsetY, ignoreLockedZone).zoneInfo;
                            ZoneInfo hRZone = getImageZoneInfoForPosition(startingZoneInfo, offsetX + 1.0, offsetY, ignoreLockedZone).zoneInfo;
                            ZoneInfo hBZone = getImageZoneInfoForPosition(startingZoneInfo, offsetX, offsetY - 1.0, ignoreLockedZone).zoneInfo;
                            ZoneInfo hFZone = getImageZoneInfoForPosition(startingZoneInfo, offsetX, offsetY + 1.0, ignoreLockedZone).zoneInfo;

                            slope = getSlopeForZoneInfo(hLZone, hRZone, hBZone, hFZone);

                            slopesCache[zonePositionKey] = slope;
                        } else {
                            slope = slopesCache[zonePositionKey];
                        }

                        skipPixel = slope < slopeValue.x || slope > slopeValue.y;
                    }

                    if (!skipPixel) {
                        Color brushPixelValue = brushImage->get_pixel(x, y);
                        float brushPixelStrength = brushPixelValue.a * (1.0f - imageZoneInfo.lockedStrength);

                        onBrushPixel(imageZoneInfo, brushPixelStrength);
                    }
                }
            }
        }
    }
}

TypedArray<Ref<ZoneResource>> ToolBase::paintComputeShaderWithBrush(Ref<ComputeShaderInstance> compuateShaderInstance, int imageBinding, Image::Format imageFormat, int brushBinding, Ref<Image> brushImage, int brushSize, Vector2 slopeValue, Vector2 imagePosition) {
    Ref<Image> toolBrushImages = Image::create_empty(_maxBrushSize, _maxBrushSize, false, imageFormat);
    int startingX = imagePosition.x - (brushSize / 2.0);
    int startingY = imagePosition.y - (brushSize / 2.0);

    int endingX = imagePosition.x + (brushSize / 2.0);
    int endingY = imagePosition.y + (brushSize / 2.0);

    ZoneInfo startingZoneInfo = ZoneUtils::getPixelToZoneInfo(startingX, startingY, _terraBrush->get_zonesSize(), getResolution());
    ZoneInfo endingZoneInfo = ZoneUtils::getPixelToZoneInfo(endingX, endingY, _terraBrush->get_zonesSize(), getResolution());

    std::unordered_map<Ref<Image>, BrushToImageRegion> brushToImageRegions = std::unordered_map<Ref<Image>, BrushToImageRegion>();

    TypedArray<Ref<ZoneResource>> paintedZones = TypedArray<Ref<ZoneResource>>();

    int currentLeft = 0;
    for (int xZone = startingZoneInfo.zonePosition.x; xZone <= endingZoneInfo.zonePosition.x; xZone++) {
        int currentTop = 0;

        int xStartPosition = 0;
        int xEndPosition = _terraBrush->get_zonesSize();

        // If we are on the first or last row, the y is the begenning or the ending of the zone
        if (xZone == startingZoneInfo.zonePosition.x) {
            xStartPosition = startingZoneInfo.imagePosition.x;
        } else if (xZone == endingZoneInfo.zonePosition.x) {
            xEndPosition = endingZoneInfo.imagePosition.x;
        }
        int width = xEndPosition - xStartPosition;

        for (int yZone = startingZoneInfo.zonePosition.y; yZone <= endingZoneInfo.zonePosition.y; yZone++) {
            Vector2i zonePosition = Vector2i(xZone, yZone);
            int zoneKey = ZoneUtils::getZoneKey(zonePosition);

            ZoneInfo zoneInfo = ZoneInfo(zoneKey, zonePosition, Vector2());
            Ref<ZoneResource> zone = _terraBrush->get_terrainZones()->getZoneForZoneInfo(zoneInfo);

            int yStartPosition = 0;
            int yEndPosition = _terraBrush->get_zonesSize();

            // Same thing, but for the column
            if (yZone == startingZoneInfo.zonePosition.y) {
                yStartPosition = startingZoneInfo.imagePosition.y;
            } else if (yZone == endingZoneInfo.zonePosition.y) {
                yEndPosition = endingZoneInfo.imagePosition.y;
            }

            int height = yEndPosition - yStartPosition;
            if (zone.is_null()) {
                // Auto add zone thing
            } else {
                Ref<Image> toolImage = getToolCurrentImage(zone);
                if (!toolImage.is_null()) {
                    if (!toolImage.is_null()) {
                        _terraBrush->get_terrainZones()->addDirtyImage(toolImage);
                        addImageToUndo(toolImage);
                    }

                    toolBrushImages->blend_rect(toolImage, Rect2(xStartPosition, yStartPosition, width, height), Vector2i(currentLeft, currentTop));

                    BrushToImageRegion region;
                    region.imagePosition = Vector2i(xStartPosition, yStartPosition);
                    region.brushRegion = Rect2(currentLeft, currentTop, width, height);
                    brushToImageRegions[toolImage] = region;

                    paintedZones.append(zone);
                }
            }

            currentTop += height;
        }

        currentLeft += width;
    }

    compuateShaderInstance->updateImageUniform(imageBinding, toolBrushImages);

    Ref<Image> resizedBrushImage = Image::create_empty(_maxBrushSize, _maxBrushSize, brushImage->has_mipmaps(), brushImage->get_format());
    resizedBrushImage->blend_rect(brushImage, Rect2i(0, 0, brushSize, brushSize), Vector2i(0, 0));
    compuateShaderInstance->updateImageUniform(brushBinding, resizedBrushImage);

    PackedByteArray result = compuateShaderInstance->getImageResult(imageBinding);
    toolBrushImages->set_data(toolBrushImages->get_width(), toolBrushImages->get_height(), toolBrushImages->has_mipmaps(), toolBrushImages->get_format(), result);

    for (const auto &zoneRegionPair : brushToImageRegions) {
        zoneRegionPair.first->blend_rect(toolBrushImages, zoneRegionPair.second.brushRegion, zoneRegionPair.second.imagePosition);
    }

    return paintedZones;
}

ImageZoneInfo ToolBase::getImageZoneInfoForPosition(ZoneInfo &startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone) {
    ZoneInfo zoneInfo = ZoneUtils::getZoneInfoFromZoneOffset(startingZoneInfo, Vector2i(offsetX, offsetY), _terraBrush->get_zonesSize(), getResolution());
    Ref<ZoneResource> zone = _terraBrush->get_terrainZones()->getZoneForZoneInfo(zoneInfo);

    if (zone.is_null() && _autoAddZones) {
        zone = _terraBrush->addNewZone(zoneInfo.zonePosition);

        if (!zone.is_null()) {
            _terraBrush->get_terrain()->addZoneCollision(zone);
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
    _modifiedUndoImages = std::unordered_set<Ref<Image>>();
    _heightsCache = std::unordered_map<uint64_t, float>();
}

void ToolBase::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    // Nothing to do in the base class
}

void ToolBase::endPaint() {
    addImagesToRedo();
    _modifiedUndoImages.clear();

    _lockedAxisValue = Vector2();

    _heightsCache.clear();
}

void ToolBase::set_autoAddZones(bool value) {
    _autoAddZones = value;
}

uint64_t ToolBase::getZonePositionKeyForZoneInfo(ZoneInfo &zoneInfo) const {
    int zoneKey = zoneInfo.zoneKey;
    uint64_t zonePositionKey = (static_cast<uint64_t>(zoneKey) << 32) | (static_cast<uint64_t>(zoneInfo.imagePosition.x) << 16) | static_cast<uint64_t>(zoneInfo.imagePosition.y);

    return zonePositionKey;
}

float ToolBase::getHeightForZoneInfo(ZoneInfo &zoneInfo) {
    uint64_t zonePositionKey = getZonePositionKeyForZoneInfo(zoneInfo);

    if (_heightsCache.count(zonePositionKey) == 0) {
        float height = _terraBrush->getHeightForZoneInfo(zoneInfo, false);
        _heightsCache[zonePositionKey] = height;

        return height;
    } else {
        return _heightsCache[zonePositionKey];
    }
}

float ToolBase::getSlopeForZoneInfo(ZoneInfo &hLZone, ZoneInfo &hRZone, ZoneInfo &hBZone, ZoneInfo &hFZone) {
	float hL = getHeightForZoneInfo(hLZone);
	float hR = getHeightForZoneInfo(hRZone);
	float hB = getHeightForZoneInfo(hBZone);
	float hF = getHeightForZoneInfo(hFZone);

    Vector3 normal = _terraBrush->getNormalForHeights(hL, hR, hB, hF);

    return 1.0 - normal.dot(Vector3(0.0, 1.0, 0.0));
}
