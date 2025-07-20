using System;
using System.Linq;
using System.Reflection;
using Godot;

namespace TerraBrush;

public static class GodotAgnostic {
	// Still displays if a method is obsolete in case a newer method needs to be used again
	private static void PrintObsoleteWarnings(MethodInfo method) {
		foreach (var attribute in method.GetCustomAttributes().Where(a => a is ObsoleteAttribute)) {
			GD.PushWarning($"{method.DeclaringType?.FullName ?? ""}.{method.Name} is obsolete: {attribute}");
		}
	}
	
	// Finds Image.Create or Image.CreateEmpty depending on the version of Godot and what's available
	private static MethodInfo FindImageCreateEmpty() {
		var imageType = typeof(Image);
		
		// Try to find the CreateEmpty method introduced in 4.3
		var method = imageType.GetMethod("CreateEmpty");
		
		// If it can't be found, we are on Godot 4.2 and below
		if (method == null) {
			// Look for the Create method
			method = imageType.GetMethod("Create");
		}
		
		// Hopefully this is never the case...
		if (method == null) {
			GD.PrintErr("Couldn't find Godot.Image.CreateEmpty or Godot.Image.Create");
		}
		else {
			PrintObsoleteWarnings(method);
		}

		return method;
	}
	public delegate Image ImageCreateEmptyMethod(int width, int height, bool useMipmaps, Image.Format format);

	public static readonly ImageCreateEmptyMethod ImageCreateEmpty = FindImageCreateEmpty()?.CreateDelegate<ImageCreateEmptyMethod>();
}