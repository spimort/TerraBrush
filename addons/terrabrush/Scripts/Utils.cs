using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public static class Utils {
    public static float GetNextFloatWithSeed(int seed, float minValue, float maxValue) {
        var randomGenerator = new Godot.RandomNumberGenerator();
        randomGenerator.Seed = (ulong) seed;

        return randomGenerator.RandfRange(minValue, maxValue);
    }

    public static int GetNextIntWithSeed(int seed, int minValue, int maxValue) {
        var randomGenerator = new Godot.RandomNumberGenerator();
        randomGenerator.Seed = (ulong) seed;

        return randomGenerator.RandiRange(minValue, maxValue);
    }

    public static string PathCombineForwardSlash(string directory, string path) {
        if (!directory.EndsWith("/")) {
            directory += "/";
        }

        if (path.StartsWith("/")) {
            path = path.Substring(0, 1);
        }

        directory += path;

        return directory;
    }

	public static Texture2DArray TexturesToTextureArray(IEnumerable<Texture2D> textures) {
		var textureArray = new Texture2DArray();
		var textureImageArray = new Godot.Collections.Array<Image>();

		int width = 0;
		int height = 0;

		if (textures != null) {
			textures.ToList().ForEach(texture => {
				if (texture != null) {
					var textureImage = texture.GetImage();

					if (width == 0) {
						width = textureImage.GetWidth();
						height = textureImage.GetHeight();
					} else if (textureImage.GetWidth() != width || textureImage.GetHeight() != height) {
						textureImage.Resize(width, height);
					}

					textureImageArray.Add(textureImage);
				}
			});
		}

		textureArray._Images = textureImageArray;

		return textureArray;
	}

	public static ShaderMaterial CreateCustomShaderCopy(ShaderMaterial customShader) {
		var newShader = new ShaderMaterial {
			Shader = customShader.Shader
		};

		foreach (Godot.Collections.Dictionary uniform in customShader.Shader.GetShaderUniformList()) {
			var uniformName = (string) uniform.GetValueOrDefault("name");
			newShader.SetShaderParameter(uniformName, customShader.GetShaderParameter(uniformName));
		}

		return newShader;
    }

    public static Task<Texture2D> WaitForTextureReady(Texture2D texture) {
        var asyncEvent = new TaskCompletionSource<Texture2D>();
        if (texture is NoiseTexture2D noiseTexture && noiseTexture.GetImage() == null) {
            void afterChanged() {
                asyncEvent.SetResult(texture);
                noiseTexture.Changed -= afterChanged;
            }

            noiseTexture.Changed += afterChanged;
        } else {
            asyncEvent.SetResult(texture);
        }

        return asyncEvent.Task;
    }
}
