#include "set_angle_tool.h"
#include "../misc/zone_utils.h"
#include "../misc/zone_info.h"
#include "../misc/string_names.h"
#include "../misc/utils.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;

void SetAngleTool::_bind_methods() {}

SetAngleTool::SetAngleTool() {}

SetAngleTool::~SetAngleTool() {}

void SetAngleTool::init(TerraBrush *terraBrush, EditorUndoRedoManager *undoRedoManager, bool autoAddZones) {
    ToolBase::init(terraBrush, undoRedoManager, autoAddZones);

    updateInitialPointMesh();
}

bool SetAngleTool::getApplyResolution() const {
    return true;
}

String SetAngleTool::getToolInfo(TerrainToolType toolType) {
    String initialValue = "";

    if (_setAngleInitialPoint == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
        initialValue = "Select the initial point with CTRL + click";
    }

    if (_setAngleValue == 0) {
        initialValue = initialValue + "\nSelect angle with CTRL + (mouse wheel or +/-)";
    }

    return (initialValue + "\nAngle : " + String::num_real(_setAngleValue) + "\n").strip_edges();
}

bool SetAngleTool::handleInput(TerrainToolType toolType, Ref<InputEvent> event) {
    if (Input::get_singleton()->is_key_pressed(Key::KEY_CTRL)) {
        float increment = 1;
        int roundFactor = 0;

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
            _setAngleValue += incrementValue;
            double factor = Math::pow(10.0, roundFactor);
            updateSetAngleValue((float) Math::round(_setAngleValue * factor) / factor);
            return true;
        }
    }

    return ToolBase::handleInput(toolType, event);
}

void SetAngleTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void SetAngleTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

Ref<Image> SetAngleTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    return zone->get_heightMapImage();
}

void SetAngleTool::beforeDeselect() {
    clearInitialPointMesh();
}

void SetAngleTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (Input::get_singleton()->is_key_pressed(Key::KEY_CTRL)) {
        ZoneInfo initialPoint = ZoneUtils::getPixelToZoneInfo(imagePosition.x, imagePosition.y, _terraBrush->get_zonesSize(), _terraBrush->get_resolution());
        ImageZoneInfo imageZoneInfo = getImageZoneInfoForPosition(initialPoint, 0, 0);
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        _setAngleInitialPoint = Vector3(imagePosition.x, currentPixel.r, imagePosition.y);

        updateSetAngleValue(_setAngleValue);
        updateInitialPointMesh();

        return;
    }

    if (_setAngleInitialPoint == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
        return;
    }

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Vector2i absolutePosition = (imageZoneInfo.zoneInfo.imagePosition * _terraBrush->get_resolution()) + (imageZoneInfo.zoneInfo.zonePosition * (_terraBrush->get_zonesSize() - 1));
        float distanceToStartingPoint = Vector2(_setAngleInitialPoint.x, _setAngleInitialPoint.z).distance_to(absolutePosition);
        float angleHeight = (float) (distanceToStartingPoint * Math::tan(Math::deg_to_rad(_setAngleValue))) + _setAngleInitialPoint.y;

        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
        Color newValue = Color(
            Math::lerp(currentPixel.r, angleHeight, pixelBrushStrength * brushStrength),
            currentPixel.g,
            currentPixel.b,
            currentPixel.a
        );

        imageZoneInfo.image->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        _sculptedZones.insert(imageZoneInfo.zone);
    }));

    _terraBrush->get_terrainZones()->updateHeightmaps();
}

void SetAngleTool::updateInitialPointMesh() {
    if (_setAngleInitialPoint == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
        if (_initialPointMesh != nullptr) {
            _initialPointMesh->queue_free();
        }
    } else {
        if (_initialPointMesh == nullptr) {
            Ref<StandardMaterial3D> material = memnew(StandardMaterial3D);
            material->set_transparency(BaseMaterial3D::Transparency::TRANSPARENCY_ALPHA);
            material->set_albedo(Color::html("#0055c9c7"));

            Ref<SphereMesh> sphereMesh = memnew(SphereMesh);
            sphereMesh->set_radius(4);
            sphereMesh->set_height(8);

            MeshInstance3D *pointMesh = memnew(MeshInstance3D);
            pointMesh->set_mesh(sphereMesh);
            pointMesh->set_material_override(material);

            _initialPointMesh = pointMesh;

            Node *container = _terraBrush->get_node_or_null((NodePath) StringNames::SetAnglePointContainer());
            if (container == nullptr) {
                container = memnew(Node3D);
                container->set_name(StringNames::SetAnglePointContainer());
                _terraBrush->add_child(container);
            }

            container->add_child(_initialPointMesh);
        }

        _initialPointMesh->set_global_position(_setAngleInitialPoint - Vector3(_terraBrush->get_zonesSize() / 2.0f, 0, _terraBrush->get_zonesSize() / 2.0f));
    }
}

void SetAngleTool::clearInitialPointMesh() {
    Node *existingPointContainer = _terraBrush->get_node_or_null((NodePath) StringNames::SetAnglePointContainer());
    if (existingPointContainer != nullptr) {
        existingPointContainer->set_name(StringName(StringNames::SetAnglePointContainer()) + StringName("_temp"));
        existingPointContainer->queue_free();
    }
}

float SetAngleTool::getSetAngleValue() {
    return _setAngleValue;
}

Vector3 SetAngleTool::getSetAngleInitialPoint() {
    return _setAngleInitialPoint;
}

void SetAngleTool::updateSetAngleValue(float value) {
    value = Math::clamp(value, -MaxAngle, MaxAngle);

    _setAngleValue = value;
}

void SetAngleTool::updateSetAngleInitialPoint(Vector3 value) {
    _setAngleInitialPoint = value;
}