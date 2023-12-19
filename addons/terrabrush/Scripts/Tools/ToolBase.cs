using System;
using Godot;

namespace TerraBrush;

public abstract class ToolBase {
    protected delegate void OnBrushPixel(float pixelBrushStrength, int xPosition, int yPosition);

    public abstract void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition);

    protected void ForEachBrushPixel(TerraBrush terraBrush, Image brushImage, int brushSize, Vector2 imagePosition, OnBrushPixel onBrushPixel) {
        for (var x = 0; x < brushSize; x++) {
            for (var y = 0; y < brushSize; y++) {
                var xPosition = (int) imagePosition.X - (x - brushSize / 2);
                var yPosition = (int) imagePosition.Y - (y - brushSize / 2);

                if (xPosition >= 0 && xPosition < terraBrush.TerrainSize && yPosition >= 0 && yPosition < terraBrush.TerrainSize) {
                    var brushPixelValue = brushImage.GetPixel(x, y);
                    var colorValue = brushPixelValue.A;

                    onBrushPixel(colorValue, xPosition, yPosition);
                }
            }
        }
    }
}
