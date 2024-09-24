#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class LockTool : ToolBase {
    public LockTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        if (zone.LockTexture == null) {
            zone.LockTexture = ZoneUtils.CreateLockImage(_terraBrush.ZonesSize, zone.ZonePosition);
        }

        return zone.LockTexture;
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            if (pixelBrushStrength > 0.0) {
                var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
                var lockValue = toolType == TerrainToolType.LockAdd ? 1f : 0f;

                var newValue = new Color(
                    Mathf.Lerp(currentPixel.R, lockValue, pixelBrushStrength * brushStrength),
                    currentPixel.G,
                    currentPixel.B,
                    currentPixel.A
                );

                imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            }
        }, true);

        _terraBrush.TerrainZones.UpdateLockTexture();
    }
}
#endif
