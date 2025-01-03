#if TOOLS
using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

public class HoleTool : ToolBase {
    private HashSet<ZoneResource> _sculptedZones;

    protected override bool ApplyResolution => true;

    public HoleTool(TerraBrush terraBrush) : base(terraBrush) {}

    protected override ImageTexture GetToolCurrentImageTexture(ZoneResource zone) {
        return zone.HeightMapTexture;
    }

    public override void BeginPaint() {
        base.BeginPaint();

        _sculptedZones = new HashSet<ZoneResource>();
    }

    public async override void EndPaint() {
        base.EndPaint();

        _sculptedZones = null;

        _terraBrush.ClearObjects();
        await _terraBrush.CreateObjects();
    }

    public override void Paint(TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        ForEachBrushPixel(brushImage, brushSize, imagePosition, (imageZoneInfo, pixelBrushStrength) => {
            var currentPixel = imageZoneInfo.Image.GetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y);
            var holeValue = toolType == TerrainToolType.HoleAdd ? 1f : 0f;

            var newValue = new Color(
                currentPixel.R,
                Math.Max(0.0f, Math.Min(1.0f, Mathf.Lerp(currentPixel.G, holeValue, (float) Math.Ceiling(pixelBrushStrength) * (brushStrength * 100.0f)))),
                currentPixel.B,
                currentPixel.A
            );

            imageZoneInfo.Image.SetPixel(imageZoneInfo.ZoneInfo.ImagePosition.X, imageZoneInfo.ZoneInfo.ImagePosition.Y, newValue);
            _sculptedZones.Add(imageZoneInfo.Zone);
        });

        _terraBrush.TerrainZones.UpdateHeightmaps();
    }
}
#endif
