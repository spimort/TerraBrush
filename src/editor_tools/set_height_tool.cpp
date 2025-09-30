#include "set_height_tool.h"
#include "../misc/zone_utils.h"
#include "../misc/zone_info.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

using namespace godot;

void SetHeightTool::_bind_methods() {}

SetHeightTool::SetHeightTool() {}

SetHeightTool::~SetHeightTool() {}

bool SetHeightTool::getApplyResolution() {
    return true;
}

String SetHeightTool::getToolInfo(TerrainToolType toolType) {
    String initialValue = "";
    if (_setHeightValue == 0) {
        initialValue = "Select height with CTRL + (click or mouse wheel or +/-)\nUse CTRL + ALT to set the increment to 0.1";
    }

    return (initialValue + "\nHeight : " + String::num_real(_setHeightValue)).strip_edges();
}

bool SetHeightTool::handleInput(TerrainToolType toolType, Ref<InputEvent> event) {
    if (Input::get_singleton()->is_key_pressed(Key::KEY_CTRL)) {
        float increment = 1;
        int roundFactor = 0;
        if (Input::get_singleton()->is_key_pressed(Key::KEY_ALT)) {
            increment = 0.1f;
            roundFactor = 1;
        }

        float incrementValue = 0;
        if (Object::cast_to<InputEventMouseButton>(event.ptr()) != nullptr) {
            Ref<InputEventMouseButton> inputMouseButton = Object::cast_to<InputEventMouseButton>(event.ptr());
            if (inputMouseButton->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_UP) {
                incrementValue = increment;
            } else if (inputMouseButton->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_DOWN) {
                incrementValue = -increment;
            }
        }

        if (Object::cast_to<InputEventKey>(event.ptr()) != nullptr) {
            Ref<InputEventKey> inputEvent = Object::cast_to<InputEventKey>(event.ptr());
            if (inputEvent->get_keycode() == Key::KEY_EQUAL) {
                incrementValue = increment;
            } else if (inputEvent->get_keycode() == Key::KEY_MINUS) {
                incrementValue = -increment;
            }
        }

        if (incrementValue != 0) {
            _setHeightValue += incrementValue;
            double factor = Math::pow(10.0, roundFactor);
            updateSetHeightValue((float) Math::round(_setHeightValue * factor) / factor);
            return true;
        }
    }

    return ToolBase::handleInput(toolType, event);
}

void SetHeightTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void SetHeightTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

Ref<ImageTexture> SetHeightTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_heightMapTexture();
}

void SetHeightTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (Input::get_singleton()->is_key_pressed(Key::KEY_CTRL)) {
        ZoneInfo initialPoint = ZoneUtils::getPixelToZoneInfo(imagePosition.x, imagePosition.y, _terraBrush->get_zonesSize(), _terraBrush->get_resolution());
        ImageZoneInfo &imageZoneInfo = getImageZoneInfoForPosition(initialPoint, 0, 0, true);
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        updateSetHeightValue(_setHeightValue = currentPixel.r);
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        Color newValue = Color(
            Math::lerp(currentPixel.r, _setHeightValue, pixelBrushStrength * brushStrength),
            currentPixel.g,
            currentPixel.b,
            currentPixel.a
        );

        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));

    _terraBrush->get_terrainZones()->updateHeightmaps();
}

float SetHeightTool::getSetHeightValue() {
    return _setHeightValue;
}

void SetHeightTool::updateSetHeightValue(float value) {
    _setHeightValue = value;
}