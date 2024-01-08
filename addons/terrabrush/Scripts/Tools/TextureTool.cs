using System.Linq;
using Godot;

namespace TerraBrush;

public class TextureTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.TextureSetIndex == null) {
            return;
        }

        var splatmapIndex = Mathf.FloorToInt(terraBrush.TextureSetIndex.Value / 4);
        var splatmapImages = terraBrush.Splatmaps.Select(texture => texture.GetImage()).ToList();
        var colorChannel = terraBrush.TextureSetIndex.Value % 4;

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            for (int i = 0; i < splatmapImages.Count(); i++) {
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

                var currentSplatmapImage = splatmapImages[i];

                var currentPixel = currentSplatmapImage.GetPixel(xPosition, yPosition);
                var newValue = new Color(
                    Mathf.Lerp(currentPixel.R, splatmapColor.R, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.G, splatmapColor.G, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.B, splatmapColor.B, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.A, splatmapColor.A, pixelBrushStrength * brushStrength)
                );
                currentSplatmapImage.SetPixel(xPosition, yPosition, newValue);
            }
        });

        for (int i = 0; i < splatmapImages.Count(); i++) {
            terraBrush.Splatmaps[i].Update(splatmapImages[i]);
        }

        terraBrush.Terrain.TerrainSplatmapsUpdated(splatmapImages);
        terraBrush.UpdateFoliagesGroudTexture();
    }
}
