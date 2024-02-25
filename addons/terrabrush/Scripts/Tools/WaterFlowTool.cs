#if TOOLS
using Godot;

namespace TerraBrush;

public class WaterFlowTool : ToolBase {
    private Vector2 _previousWaterMousePosition = Vector2.Zero;
    private Vector2 _previousWaterMouseDirection = Vector2.Zero;

    public WaterFlowTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.WaterTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.WaterDefinition == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

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
                imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            }
        });

        _terraBrush.TerrainZones.UpdateWaterTextures();
        _previousWaterMousePosition = imagePosition;
    }
}
#endif
