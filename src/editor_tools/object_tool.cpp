#include "object_tool.h"
#include "../nodes/objects_base.h"
#include "../editor_resources/object_resource.h"
#include "../editor_resources/object_definition_resource.h"

using namespace godot;

void ObjectTool::_bind_methods() {}

ObjectTool::ObjectTool() {}

ObjectTool::~ObjectTool() {}

Ref<ImageTexture> ObjectTool::getToolCurrentImageTexture(Ref<ZoneResource> zone) {
    return zone->get_objectsTexture()[_selectedObjectIndex];
}

void ObjectTool::beginPaint() {
    ToolBase::beginPaint();

    _heightmapImagesCache =  std::unordered_map<Ref<ZoneResource>, Ref<Image>>();
    _waterImagesCache =  std::unordered_map<Ref<ZoneResource>, Ref<Image>>();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void ObjectTool::endPaint() {
    ToolBase::endPaint();

    _heightmapImagesCache =  std::unordered_map<Ref<ZoneResource>, Ref<Image>>();
    _waterImagesCache =  std::unordered_map<Ref<ZoneResource>, Ref<Image>>();

    _sculptedZones = std::unordered_set<Ref<ZoneResource>>();
}

void ObjectTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_selectedObjectIndex < 0) {
        return;
    }

    Ref<ObjectResource> currentObject = _terraBrush->get_objects()[_selectedObjectIndex];
    if (currentObject->get_definition()->get_objectScenes().size() == 0 || currentObject->get_hide()) {
        return;
    }

    Ref<ImageTexture> noiseTexture = currentObject->get_definition()->get_noiseTexture().is_null() ? _terraBrush->get_defaultNoise() : currentObject->get_definition()->get_noiseTexture();
    Ref<Image> noiseImage = nullptr;
    if (!noiseTexture.is_null()) {
        noiseImage = noiseTexture->get_image();
    }

    ObjectsBase *objectsNode = Object::cast_to<ObjectsBase>(_terraBrush->get_objectsContainer()->get_node_or_null(String::num_int64(_selectedObjectIndex)));

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        Ref<Image> heightmapImage = nullptr;
        if (_heightmapImagesCache.count(imageZoneInfo.zone) > 0) {
            heightmapImage = _heightmapImagesCache[imageZoneInfo.zone];
        }

        if (heightmapImage.is_null()) {
            heightmapImage = imageZoneInfo.zone->get_heightMapTexture()->get_image();
            _heightmapImagesCache[imageZoneInfo.zone] = heightmapImage;
        }

        Ref<Image> waterImage = nullptr;
        if (!_terraBrush->get_waterDefinition().is_null()) {
            if (_waterImagesCache.count(imageZoneInfo.zone) > 0) {
                waterImage = _waterImagesCache[imageZoneInfo.zone];
            }

            if (waterImage.is_null()) {
                waterImage = imageZoneInfo.zone->get_waterTexture()->get_image();
                _waterImagesCache[imageZoneInfo.zone] = waterImage;
            }
        }

        int xPosition = imageZoneInfo.zoneInfo.imagePosition.x;
        int yPosition = imageZoneInfo.zoneInfo.imagePosition.y;

        Color currentPixel = imageZoneInfo.image->get_pixel(xPosition, yPosition);
        Color newColor = currentPixel;

        if (pixelBrushStrength > 0) {
            newColor = toolType == TerrainToolType::TERRAINTOOLTYPE_OBJECTADD ? Color(1, 0, 0, 1) : Color(1, 1, 1, 0);

            objectsNode->addRemoveObjectFromTool(toolType == TerrainToolType::TERRAINTOOLTYPE_OBJECTADD, xPosition, yPosition, imageZoneInfo.zone, heightmapImage, waterImage, noiseImage);
            imageZoneInfo.image->set_pixel(xPosition, yPosition, newColor);

            _sculptedZones.insert(imageZoneInfo.zone);
        }
    }));

    objectsNode->updateMeshesFromTool();
}

void ObjectTool::updateSelectedObjectIndex(int value) {
    _selectedObjectIndex = value;
}