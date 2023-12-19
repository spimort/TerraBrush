using Godot;

namespace TerraBrush;

public class WaterFlowTool : ToolBase {
    private Vector2 _previousWaterMousePosition = Vector2.Zero;
    private Vector2 _previousWaterMouseDirection = Vector2.Zero;

    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.WaterTexture == null) {
            return;
        }

        var waterImage = terraBrush.WaterTexture.GetImage();

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var currentPixel = waterImage.GetPixel(xPosition, yPosition);
            if (currentPixel.R > 0.0) {
                var direction = _previousWaterMousePosition.DirectionTo(imagePosition) * -1;

                if (direction == Vector2.Zero) {
                    direction = _previousWaterMouseDirection;
                } else {
                    direction = new Vector2(
                        Mathf.Lerp(_previousWaterMouseDirection.X, direction.X, 0.1f),
                        Mathf.Lerp(_previousWaterMouseDirection.Y, direction.Y, 0.1f)
                    );
                    _previousWaterMouseDirection = direction;
                }

                var newColor = toolType == TerrainToolType.WaterFlowAdd ? new Color(currentPixel.R, Mathf.Remap(direction.X, -1, 1, 0, 1), Mathf.Remap(direction.Y, -1, 1, 0, 1), 1) : new Color(currentPixel.R, 0.5f, 0.5f, 1);
                var newValue = new Color(
                    Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                    Mathf.Lerp(currentPixel.A, newColor.A, pixelBrushStrength * brushStrength)
                );
                waterImage.SetPixel(xPosition, yPosition, newValue);
            }
        });

        terraBrush.WaterTexture.Update(waterImage);
        _previousWaterMousePosition = imagePosition;
    }
}
