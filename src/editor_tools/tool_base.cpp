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

PixelLockedInfo ToolBase::isZonePixelLocked(Ref<ZoneResource> zone, Ref<ZoneInfo> zoneInfo) {
    Ref<Image> image = zone->get_lockTexture()->get_image();
    Color pixel = image->get_pixel(zoneInfo->get_imagePosition().x, zoneInfo->get_imagePosition().y);
    return PixelLockedInfo(pixel.r == 1.0, pixel.r);
}

void ToolBase::addImagesToRedo() {
    for (Ref<ImageTexture> imageTexture : _modifiedUndoTextures) {
        _undoRedoManager->add_do_method(imageTexture.ptr(), "update", getUndoRedoImageFromTexture(imageTexture));
    }
}

Ref<Image> ToolBase::getUndoRedoImageFromTexture(Ref<ImageTexture> imageTexture) {
    Ref<Image> image = memnew(Image);
    image->set_data(imageTexture->get_width(), imageTexture->get_height(), imageTexture->get_image()->has_mipmaps(), imageTexture->get_format(), imageTexture->get_image()->get_data());

    return image;
}

int ToolBase::getResolution() {
    if (getApplyResolution()) {
        return _terraBrush->get_resolution();
    }

    return 1;
}

void ToolBase::addTextureToUndo(Ref<ImageTexture> texture) {
    if (!_modifiedUndoTextures.has(texture)) {
        _modifiedUndoTextures.append(texture);

        _undoRedoManager->add_undo_method(texture.ptr(), "update", getUndoRedoImageFromTexture(texture));
    }
}

bool ToolBase::getApplyResolution() {
    return false;
}

Ref<ImageTexture> ToolBase::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return nullptr;
}

void ToolBase::forEachBrushPixel(Ref<Image> brushImage, int brushSize, Vector2 imagePosition, Callable onBrushPixel, bool ignoreLockedZone) {
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
    Ref<ZoneInfo> startingZoneInfo = ZoneUtils::getPixelToZoneInfo(startingX, startingY, _terraBrush->get_zonesSize(), getResolution());

    HashSet<String> pointsCache = HashSet<String>();
    for (int x = 0; x < brushSize; x++) {
        for (int y = 0; y < brushSize; y++) {
            int offsetX = x;
            int offsetY = y;
            if (_terraBrush->get_resolution() != 1 && getApplyResolution()) {
                offsetX = (int) Math::floor(Math::remap((float) x, 0, brushSize, 0, (int) Math::ceil(((float) brushSize) / _terraBrush->get_resolution())));
                offsetY = (int) Math::floor(Math::remap((float) y, 0, brushSize, 0, (int) Math::ceil(((float) brushSize) / _terraBrush->get_resolution())));
            }
            Ref<ImageZoneInfo> imageZoneInfo = getImageZoneInfoForPosition(startingZoneInfo, offsetX, offsetY, ignoreLockedZone);

            if (!imageZoneInfo.is_null()) {
                int zoneKey = imageZoneInfo->get_zoneInfo()->get_zoneKey();
                Vector2i zoneImagePosition = imageZoneInfo->get_zoneInfo()->get_imagePosition();
                int positionKey = (zoneImagePosition.x << 8) + zoneImagePosition.y;
                // Create a cache key with the zone and the position
                String zonePositionKey = String::num_int64(zoneKey) + "_" + String::num_int64(positionKey);

                if (_terraBrush->get_resolution() == 1 || !getApplyResolution() || !pointsCache.has(zonePositionKey)) {
                    if (_terraBrush->get_resolution() != 1) {
                        pointsCache.insert(zonePositionKey);
                    }

                    Color brushPixelValue = brushImage->get_pixel(x, y);
                    float brushPixelStrength = brushPixelValue.a * (1.0f - imageZoneInfo->get_lockedStrength());

                    onBrushPixel.call(imageZoneInfo, brushPixelStrength);
                }
            }
        }
    }
}

Ref<ImageZoneInfo> ToolBase::getImageZoneInfoForPosition(Ref<ZoneInfo> startingZoneInfo, int offsetX, int offsetY, bool ignoreLockedZone) {
    Ref<ZoneInfo> zoneInfo = ZoneUtils::getZoneInfoFromZoneOffset(startingZoneInfo, Vector2i(offsetX, offsetY), _terraBrush->get_zonesSize(), getResolution());
    Ref<ZoneResource> zone;
    if (_zonesPositionCache.has(zoneInfo->get_zoneKey())) {
        zone = _zonesPositionCache[zoneInfo->get_zoneKey()];
    }

    if (zone.is_null()) {
        zone = _terraBrush->get_terrainZones()->getZoneForZoneInfo(zoneInfo);

        if (!zone.is_null()) {
            _zonesPositionCache[zoneInfo->get_zoneKey()] = zone;
        }
    }

    if (zone.is_null() && _autoAddZones) {
        zone = _terraBrush->addNewZone(zoneInfo->get_zonePosition());

        if (!zone.is_null()) {
            _terraBrush->get_terrain()->addZoneCollision(zone);
            _zonesPositionCache[zoneInfo->get_zoneKey()] = zone;
        }
    }

    if (!zone.is_null()) {
        PixelLockedInfo lockInfo = PixelLockedInfo(false, 0.0f);
        if (!ignoreLockedZone && !zone->get_lockTexture().is_null()) {
            Ref<ZoneInfo> lockZoneInfo = zoneInfo;
            if (_terraBrush->get_resolution() != 1 && getApplyResolution()) {
                Ref<ZoneInfo> resolutionZoneInfo = memnew(ZoneInfo);
                resolutionZoneInfo->set_imagePosition(startingZoneInfo->get_imagePosition() * _terraBrush->get_resolution());

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
            Ref<Image> image;
            if (_imagesCache.has(zone)) {
                image = _imagesCache[zone];
            }
            Ref<ImageTexture> imageTexture = getToolCurrentImageTexture(zone);

            if (!imageTexture.is_null()) {
                if (image.is_null()) {
                    image = imageTexture->get_image();
                    _imagesCache[zone] = image;
                }

                _terraBrush->get_terrainZones()->addDirtyImageTexture(imageTexture);
                addTextureToUndo(imageTexture);
            }

            Ref<ImageZoneInfo> imageZoneInfo = memnew(ImageZoneInfo);
            imageZoneInfo->set_image(image);
            imageZoneInfo->set_zoneInfo(zoneInfo);
            imageZoneInfo->set_zone(zone);
            imageZoneInfo->set_lockedStrength(lockInfo.lockedStrength);

            return imageZoneInfo;
        }
    }

    return nullptr;
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
    _imagesCache = TypedDictionary<Ref<ZoneResource>, Ref<Image>>();
    _zonesPositionCache = TypedDictionary<int, Ref<ZoneResource>>();
    _modifiedUndoTextures = TypedArray<Ref<ImageTexture>>();
}

void ToolBase::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    // Nothing to do in the base class
}

void ToolBase::endPaint() {
    _imagesCache.clear();
    _zonesPositionCache.clear();

    addImagesToRedo();
    _modifiedUndoTextures.clear();

    _lockedAxisValue = Vector2();
}

void ToolBase::set_autoAddZones(bool value) {
    _autoAddZones = value;
}

// ImageZoneInfo class

void ImageZoneInfo::_bind_methods() {}

ImageZoneInfo::ImageZoneInfo() {}

ImageZoneInfo::~ImageZoneInfo() {}

Ref<Image> ImageZoneInfo::get_image() const {
    return _image;
}
void ImageZoneInfo::set_image(const Ref<Image> &value) {
    _image = value;
}

Ref<ZoneInfo> ImageZoneInfo::get_zoneInfo() const {
    return _zoneInfo;
}
void ImageZoneInfo::set_zoneInfo(const Ref<ZoneInfo> &value) {
    _zoneInfo = value;
}

Ref<ZoneResource> ImageZoneInfo::get_zone() const {
    return _zone;
}
void ImageZoneInfo::set_zone(const Ref<ZoneResource> &value) {
    _zone = value;
}

float ImageZoneInfo::get_lockedStrength() const {
    return _lockedStrength;
}
void ImageZoneInfo::set_lockedStrength(const float value) {
    _lockedStrength = value;
}