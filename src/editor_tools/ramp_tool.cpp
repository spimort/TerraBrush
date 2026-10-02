#include "ramp_tool.h"
#include "../misc/zone_utils.h"
#include "../misc/zone_info.h"
#include "../misc/string_names.h"
#include "../misc/utils.h"
#include "../misc/setting_contants.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

void RampTool::_bind_methods() {}

RampTool::RampTool() {}

RampTool::~RampTool() {}

void RampTool::init(TerraBrush *terraBrush, Ref<ToolUndoRedo> undoRedo, bool autoAddZones) {
    ToolBase::init(terraBrush, undoRedo, autoAddZones);

    updateInitialPointMesh();
}

bool RampTool::getApplyResolution() const {
    return true;
}

String RampTool::getToolInfo(TerrainToolType toolType) {
    return "Select the initial point with click and select another point to paint ramp";
}

void RampTool::beginPaint() {
    ToolBase::beginPaint();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void RampTool::endPaint() {
    ToolBase::endPaint();

    TypedArray<Ref<ZoneResource>> sculptedZonesList = TypedArray<Ref<ZoneResource>>();
    for (Ref<ZoneResource> zone : _sculptedZones) {
        sculptedZonesList.append(zone);
    }
    _terraBrush->updateObjectsHeight(sculptedZonesList);

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();

    _preventPaint = false;
}

Ref<Image> RampTool::getToolCurrentImage(Ref<ZoneResource> zone) {
    return zone->get_heightMapImage();
}

void RampTool::beforeDeselect() {
    clearInitialPointMesh();
}

void RampTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 slopeValue, Vector2 imagePosition) {
    if (_preventPaint) {
        return;
    }

    _preventPaint = true;

    if (_initialPoint == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
        ZoneInfo initialPoint = ZoneUtils::getPixelToZoneInfo(imagePosition.x, imagePosition.y, _terraBrush->get_zonesSize(), _terraBrush->get_resolution());
        ImageZoneInfo imageZoneInfo = getImageZoneInfoForPosition(initialPoint, 0, 0);
        Color currentPixel = imageZoneInfo.image->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);

        _initialPoint = Vector3(imagePosition.x, currentPixel.r, imagePosition.y);

        updateInitialPointMesh();
    } else {
        ZoneInfo targetPoint = ZoneUtils::getPixelToZoneInfo(imagePosition.x, imagePosition.y, _terraBrush->get_zonesSize(), _terraBrush->get_resolution());
        ImageZoneInfo targetImageZoneInfo = getImageZoneInfoForPosition(targetPoint, 0, 0);
        Color targetPixel = targetImageZoneInfo.image->get_pixel(targetImageZoneInfo.zoneInfo.imagePosition.x, targetImageZoneInfo.zoneInfo.imagePosition.y);

        Vector2 fromPosition = Vector2(_initialPoint.x, _initialPoint.z);
        Vector2 toPosition = Vector2(imagePosition.x, imagePosition.y);

        // Draw the ramp
        forEachRampPixel(brushSize, brushImage, targetPixel, fromPosition, toPosition, ([&](Vector2 brushPosition, Color brushPixel, float currentHeight, ImageZoneInfo currentImageZoneInfo) {
            Color currentPixel = currentImageZoneInfo.image->get_pixel(currentImageZoneInfo.zoneInfo.imagePosition.x, currentImageZoneInfo.zoneInfo.imagePosition.y);

            Color newPixel = Color(
                Math::lerp(currentPixel.r, currentHeight, brushPixel.a),
                currentPixel.g,
                currentPixel.b,
                currentPixel.a
            );

            currentImageZoneInfo.image->set_pixel(currentImageZoneInfo.zoneInfo.imagePosition.x, currentImageZoneInfo.zoneInfo.imagePosition.y, newPixel);
            _sculptedZones.insert(currentImageZoneInfo.zone);
        }));

        // Smooth the ramp
        float smoothPasses = ProjectSettings::get_singleton()->get_setting(SettingContants::RampToolSmoothPasses(), SettingContants::RampToolSmoothPassesDefaultValue());
        for (int i = 0; i < smoothPasses; i++) {
            float smoothingMultiplier = ProjectSettings::get_singleton()->get_setting(SettingContants::SmoothingMultiplier(), SettingContants::SmoothingMultiplierDefaultValue());
            forEachRampPixel(brushSize, brushImage, targetPixel, fromPosition, toPosition, ([&](Vector2 brushPosition, Color brushPixel, float currentHeight, ImageZoneInfo currentImageZoneInfo) {
                std::vector<float> directions = std::vector<float>();

                Color currentPixel = currentImageZoneInfo.image->get_pixel(currentImageZoneInfo.zoneInfo.imagePosition.x, currentImageZoneInfo.zoneInfo.imagePosition.y);
                if (Math::abs(currentHeight - currentPixel.r) > 0.005) {
                    directions.push_back(currentPixel.r);

                    ImageZoneInfo neighbourImageZoneInfo = getImageZoneInfoForPosition(currentImageZoneInfo.zoneInfo, -1, 0, true);
                    if (!neighbourImageZoneInfo.zone.is_null()) {
                        directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
                    }

                    neighbourImageZoneInfo = getImageZoneInfoForPosition(currentImageZoneInfo.zoneInfo, 1, 0, true);
                    if (!neighbourImageZoneInfo.zone.is_null()) {
                        directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
                    }

                    neighbourImageZoneInfo = getImageZoneInfoForPosition(currentImageZoneInfo.zoneInfo, 0, -1, true);
                    if (!neighbourImageZoneInfo.zone.is_null()) {
                        directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
                    }

                    neighbourImageZoneInfo = getImageZoneInfoForPosition(currentImageZoneInfo.zoneInfo, 0, 1, true);
                    if (!neighbourImageZoneInfo.zone.is_null()) {
                        directions.push_back(neighbourImageZoneInfo.image->get_pixel(neighbourImageZoneInfo.zoneInfo.imagePosition.x, neighbourImageZoneInfo.zoneInfo.imagePosition.y).r);
                    }

                    float average = 0;
                    for (float directionValue : directions) {
                        average += directionValue;
                    }
                    average /= directions.size();

                    float resultValue = Math::lerp(currentPixel.r, average, smoothingMultiplier);

                    Color newPixel = Color(resultValue, currentPixel.g, currentPixel.b, currentPixel.a);
                    currentImageZoneInfo.image->set_pixel(currentImageZoneInfo.zoneInfo.imagePosition.x, currentImageZoneInfo.zoneInfo.imagePosition.y, newPixel);
                }
            }));
        }

        _initialPoint = Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue);
        updateInitialPointMesh();

        _terraBrush->get_terrainZones()->updateHeightmaps();
    }
}

void RampTool::updateInitialPointMesh() {
    if (_initialPoint == Vector3(Utils::InfinityValue, Utils::InfinityValue, Utils::InfinityValue)) {
        if (_initialPointMesh != nullptr) {
            _initialPointMesh->queue_free();
            _initialPointMesh = nullptr;
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

            Node *container = _terraBrush->get_node_or_null((NodePath) StringNames::RampPointContainer());
            if (container == nullptr) {
                container = memnew(Node3D);
                container->set_name(StringNames::RampPointContainer());
                _terraBrush->add_child(container);
            }

            container->add_child(_initialPointMesh);
        }

        _initialPointMesh->set_global_position(_initialPoint - Vector3(_terraBrush->get_zonesSize() / 2.0f, 0, _terraBrush->get_zonesSize() / 2.0f));
    }
}

void RampTool::clearInitialPointMesh() {
    Node *existingPointContainer = _terraBrush->get_node_or_null((NodePath) StringNames::RampPointContainer());
    if (existingPointContainer != nullptr) {
        existingPointContainer->set_name(StringName(StringNames::RampPointContainer()) + StringName("_temp"));
        existingPointContainer->queue_free();
    }
}

void RampTool::forEachRampPixel(int brushSize, Ref<Image> &brushImage, Color targetPixel, Vector2 fromPosition, Vector2 toPosition, std::function<void(Vector2, Color, float, ImageZoneInfo)> callback) {
    float distance = (toPosition - fromPosition).length();
    Vector2 direction = (toPosition - fromPosition).normalized();

    Vector2 currentPosition = fromPosition;
    Vector2 currentDirection = direction;

    HashSet<Vector2> processedPoints = HashSet<Vector2>();
    while (currentDirection.round() == direction.round()) {
        currentPosition += (direction * 0.1);

        float currentDistance = (toPosition - currentPosition).length();
        float progress = (1.0 - (currentDistance / distance));
        float currentHeight = ((targetPixel.r - _initialPoint.y) * progress) + _initialPoint.y;

        for (int i = 0; i < brushSize; i++) {
            Vector2 brushDirection = direction.rotated(Math::deg_to_rad(90.0)).normalized();
            Vector2 brushPosition = currentPosition + (i - (brushSize / 2.0)) * brushDirection;

            if (processedPoints.has(brushPosition) == 0) {
                Color brushPixel = brushImage->get_pixel(i, i);

                ZoneInfo currentPoint = ZoneUtils::getPixelToZoneInfo(brushPosition.x, brushPosition.y, _terraBrush->get_zonesSize(), _terraBrush->get_resolution());
                ImageZoneInfo currentImageZoneInfo = getImageZoneInfoForPosition(currentPoint, 0, 0);

                if (!currentImageZoneInfo.zone.is_null()) {
                    callback(brushPosition, brushPixel, currentHeight, currentImageZoneInfo);
                }

                processedPoints.insert(brushPosition);
            }
        }

        currentDirection = (toPosition - currentPosition).normalized();
    }
}

Vector3 RampTool::getInitialPoint() {
    return _initialPoint;
}

void RampTool::updateInitialPoint(Vector3 value) {
    _initialPoint = value;
}
