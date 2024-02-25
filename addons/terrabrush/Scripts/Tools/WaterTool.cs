#if TOOLS
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class WaterTool : ToolBase {
    private HashSet<ZoneResource> _sculptedZones;

    public WaterTool(TerraBrush terraBrush) : base(terraBrush) {}

    public override void BeginPaint() {
        base.BeginPaint();
        _sculptedZones = new HashSet<ZoneResource>();
    }

    public override void EndPaint() {
        base.EndPaint();

        _terraBrush.UpdateObjectsHeight(_sculptedZones.ToList());

        _sculptedZones = null;
    }

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.WaterTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (_terraBrush.WaterDefinition == null) {
            return;
        }

        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);

            var newColor = toolType == TerrainToolType.WaterAdd ? new Color(1, currentPixel.G, currentPixel.B, 1) : new Color(0, 0.5f, 0.5f, 1);
            var newValue = new Color(
                Mathf.Lerp(currentPixel.R, newColor.R, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.G, newColor.G, pixelBrushStrength * brushStrength),
                Mathf.Lerp(currentPixel.B, newColor.B, pixelBrushStrength * brushStrength),
                1
            );
            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);

            _sculptedZones.Add(imageZoneInfo.Zone);
        });

        _terraBrush.TerrainZones.UpdateWaterTextures();
    }
}
#endif
