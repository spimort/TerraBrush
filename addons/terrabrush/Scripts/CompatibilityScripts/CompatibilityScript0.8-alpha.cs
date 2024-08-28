using System.Linq;
using Godot;

namespace TerraBrush;

public static class CompatibilityScript_0_8_Alpha {
    public static void Convert(TerraBrush terraBrush) {
        if (terraBrush.Foliages != null) {
            foreach (var foliage in terraBrush.Foliages) {
                if (foliage.Definition.Strategy == 0) {
                    foliage.Definition.Strategy = FoliageStrategy.GPUParticle;
                }
            }
        }
    }
}
