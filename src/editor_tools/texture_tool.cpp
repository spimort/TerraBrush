#include "texture_tool.h"

using namespace godot;

void TextureTool::_bind_methods() {}

TextureTool::TextureTool() {}

TextureTool::~TextureTool() {}

void TextureTool::beginPaint() {
    ToolBase::beginPaint();

    _splatmapImagesCache = std::unordered_map<Ref<ImageTexture>, Ref<Image>>();
}

void TextureTool::endPaint() {
    ToolBase::endPaint();

    _splatmapImagesCache.clear();
}

void TextureTool::paint(TerrainToolType toolType, Ref<Image> brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
    if (_selectedTextureIndex < 0) {
        return;
    }

    int splatmapIndex = (int) Math::floor((float) _selectedTextureIndex / 4);
    int colorChannel = _selectedTextureIndex % 4;
    int numberOfSplatmaps = (int) Math::ceil(_terraBrush->get_textureSets()->get_textureSets().size() / 4.0);

    Color transparentColor = Color(1, 1, 1, 0); // Transparent color

    forEachBrushPixel(brushImage, brushSize, imagePosition, ([&](ImageZoneInfo &imageZoneInfo, float pixelBrushStrength) {
        for (int i = 0; i < numberOfSplatmaps; i++) {
            Color splatmapColor = transparentColor;

            if (i != splatmapIndex) {
                splatmapColor = Color(0, 0, 0, 0);
            } else if (colorChannel == 0) {
                splatmapColor = Color(1, 0, 0, 0);
            } else if (colorChannel == 1) {
                splatmapColor = Color(0, 1, 0, 0);
            } else if (colorChannel == 2) {
                splatmapColor = Color(0, 0, 1, 0);
            } else if (colorChannel == 3) {
                splatmapColor = Color(0, 0, 0, 1);
            }

            Ref<ImageTexture> currentSplatmapTexture = imageZoneInfo.zone->get_splatmapsTexture()[i];
            // Since we play with several textures here, the toolbase cannot add the texture to the dirty collection
            _terraBrush->get_terrainZones()->addDirtyImageTexture(currentSplatmapTexture);
            addTextureToUndo(currentSplatmapTexture);

            Ref<Image> currentSplatmapImage = nullptr;
            if (_splatmapImagesCache.count(currentSplatmapTexture) > 0) {
                currentSplatmapImage = _splatmapImagesCache[currentSplatmapTexture];
            }

            if (currentSplatmapImage.is_null()) {
                currentSplatmapImage = currentSplatmapTexture->get_image();
                _splatmapImagesCache[currentSplatmapTexture] = currentSplatmapImage;
            }

            Color currentPixel = currentSplatmapImage->get_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y);
            Color newValue = Color(
                Math::lerp(currentPixel.r, splatmapColor.r, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.g, splatmapColor.g, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.b, splatmapColor.b, pixelBrushStrength * brushStrength),
                Math::lerp(currentPixel.a, splatmapColor.a, pixelBrushStrength * brushStrength)
            );
            currentSplatmapImage->set_pixel(imageZoneInfo.zoneInfo.imagePosition.x, imageZoneInfo.zoneInfo.imagePosition.y, newValue);
        }
    }));

    _terraBrush->get_terrainZones()->updateSplatmapsTextures();
}

void TextureTool::updateSelectedTextureIndex(int value) {
    _selectedTextureIndex = value;
}