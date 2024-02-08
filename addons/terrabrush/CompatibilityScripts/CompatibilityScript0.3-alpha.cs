using System.Linq;
using Godot;

namespace TerraBrush;

public static class CompatibilityScript_0_3_Alpha {
    public static void Convert(TerraBrush terraBrush) {
        #pragma warning disable 0618
        if (terraBrush.HeightMap != null && (terraBrush.TerrainZones == null || terraBrush.TerrainZones.Zones.Length == 0)) {
            GD.Print("The current uses the old system without the zones. Trying to convert the map to use the new zones system.");
            terraBrush.ZonesSize = terraBrush.TerrainSize == 0 ? 256 : terraBrush.TerrainSize;
            terraBrush.TerrainZones = new ZonesResource() {
                Zones = new[] {
                    new ZoneResource() {
                        HeightMapTexture = terraBrush.HeightMap,
                        SplatmapsTexture = terraBrush.Splatmaps,
                        WaterTexture = terraBrush.WaterTexture,
                        SnowTexture = terraBrush.SnowTexture,
                        FoliagesTexture = terraBrush.Foliages?.Select(x => x.Texture).ToArray(),
                        ObjectsTexture = terraBrush.Objects?.Select(x => x.Texture).ToArray()
                    }
                }
            };

            terraBrush.TerrainSize = 0;
            terraBrush.HeightMap = null;
            terraBrush.Splatmaps = null;
            terraBrush.WaterTexture = null;
            terraBrush.SnowTexture = null;
            terraBrush.Foliages?.ToList().ForEach(x => {
                x.Texture = null;
            });
            terraBrush.Objects?.ToList().ForEach(x => {
                x.Texture = null;
            });

            GD.Print("Done converting.");
        }
        #pragma warning restore 0618
    }
}
