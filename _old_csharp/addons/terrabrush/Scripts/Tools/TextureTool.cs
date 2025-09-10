#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class TextureTool : ToolBase {
    private Dictionary<ImageTexture, Image> _splatmapImagesCache = null;

    public TextureTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint () {
        base.BeginPaint();

        _splatmapImagesCache = new Dictionary<ImageTexture, Image>();
    }

    public override void EndPaint () {
        base.EndPaint();

        _splatmapImagesCache = null;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.TextureSetIndex == null) {
            return;
        }

        var splatmapIndex = Mathf.FloorToInt(_terraBrush.TextureSetIndex.Value / 4);
        var colorChannel = _terraBrush.TextureSetIndex.Value % 4;
        var numberOfSplatmaps = Mathf.CeilToInt(_terraBrush.TextureSets.TextureSets.Count() / 4.0);

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            for (int i = 0; i < numberOfSplatmaps; i++) {
                Color splatmapColor = Colors.Transparent;

                if (i != splatmapIndex) {
                    splatmapColor = new Color(0, 0, 0, 0);
                } else if (colorChannel == 0) {
                    splatmapColor = new Color(1, 0, 0, 0);
                } else if (colorChannel == 1) {
                    splatmapColor = new Color(0, 1, 0, 0);
                } else if (colorChannel == 2) {
                    splatmapColor = new Color(0, 0, 1, 0);
                } else if (colorChannel == 3) {
                    splatmapColor = new Color(0, 0, 0, 1);
                }

                var currentSplatmapTexture = imageZoneInfo.Zone.SplatmapsTexture[i];
                // Since we play with several textures here, the toolbase cannot add the texture to the dirty collection
                _terraBrush.TerrainZones.AddDirtyImageTexture(currentSplatmapTexture);
                AddTextureToUndo(currentSplatmapTexture);

                _splatmapImagesCache.TryGetValue(currentSplatmapTexture, out Image currentSplatmapImage);

                if (currentSplatmapImage == null) {
                    currentSplatmapImage = currentSplatmapTexture.GetImage();
                    _splatmapImagesCache.Add(currentSplatmapTexture, currentSplatmapImage);
                }

                var currentPixel = currentSplatmapImage.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
                var newValue = new Color(
                    Mathf.Lerp(currentPixel.R, splatmapColor.R, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.G, splatmapColor.G, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.B, splatmapColor.B, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.A, splatmapColor.A, pixelBrushStrength * brushStrength)
                );
                currentSplatmapImage.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            }
        });

        _terraBrush.TerrainZones.UpdateSplatmapsTextures();
    }
}
#endif
