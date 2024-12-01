#if TOOLS
using System.Linq;
using Godot;

namespace TerraBrush;

public static class CompatibilityScript_0_4_Alpha {
    public static async void Convert(TerraBrush terraBrush) {
        var hasWrongHeightmapFormat = terraBrush.TerrainZones?.Zones?.Any(x => x.HeightMapTexture.GetFormat() == Image.Format.Rf);
		if (!hasWrongHeightmapFormat.GetValueOrDefault()) return;

		var result = await DialogUtils.ShowConfirmDialog(terraBrush, "Convert", "Heightmaps has been created before the hole feature was a thing, which means that they are in the wrong format.\nThey must be converted in order for the hole feature to work.\nDo you want to convert them?");
		if (!result) return;

		GD.Print("Starting heightmaps conversion...");

		if (terraBrush.TerrainZones?.Zones != null) {
			foreach (var zone in terraBrush.TerrainZones.Zones) {
				if (zone.HeightMapTexture.GetFormat() != Image.Format.Rf) continue;

				var image = zone.HeightMapTexture.GetImage();
				var newImage = GodotAgnostic.ImageCreateEmpty(image.GetWidth(), image.GetHeight(), image.HasMipmaps(),
					Image.Format.Rgf);

				for (var x = 0; x < image.GetWidth(); x++) {
					for (var y = 0; y < image.GetWidth(); y++) {
						var pixel = image.GetPixel(x, y);
						newImage.SetPixel(x, y, pixel);
					}
				}

				zone.HeightMapTexture.SetImage(newImage);
				ResourceSaver.Save(zone.HeightMapTexture, zone.HeightMapTexture.ResourcePath);
			}
		}

		GD.Print("Conversion done!");
	}
}
#endif
