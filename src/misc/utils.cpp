#include "utils.h"

#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

float Utils::getNextFloatWithSeed(int seed, float minValue, float maxValue) {
    Ref<RandomNumberGenerator> randomGenerator = memnew(RandomNumberGenerator);
    randomGenerator->set_seed(seed);

    return randomGenerator->randf_range(minValue, maxValue);
}

int Utils::getNextIntWithSeed(int seed, int minValue, int maxValue) {
    Ref<RandomNumberGenerator> randomGenerator = memnew(RandomNumberGenerator);
    randomGenerator->set_seed(seed);

    return randomGenerator->randi_range(minValue, maxValue);
}

String Utils::pathCombineForwardSlash(String directory, String path) {
    if (!directory.ends_with("/")) {
        directory += "/";
    }

    if (path.begins_with("/")) {
        path = path.substr(0, 1);
    }

    directory += path;

    return directory;
}

Ref<Texture2DArray> Utils::texturesToTextureArray(TypedArray<Ref<Texture2D>> textures) {
    Ref<Texture2DArray> textureArray = memnew(Texture2DArray);
    TypedArray<Ref<Image>> textureImageArray = TypedArray<Ref<Image>>();

    int width = 0;
    int height = 0;

    if (!textures.is_empty()) {
        for (Ref<Texture2D> texture : textures) {
            if (!texture.is_null()) {
                Ref<Image> textureImage = texture->get_image();

                if (width == 0) {
                    width = textureImage->get_width();
                    height = textureImage->get_height();
                } else if (textureImage->get_width() != width || textureImage->get_height() != height) {
                    textureImage->resize(width, height);
                }

                textureImageArray.append(textureImage);
            }
        }
    }

    // TODO
    // textureArray._Images = textureImageArray;
    textureArray->create_from_images(textureImageArray);

    return textureArray;
}

Ref<ShaderMaterial> Utils::createCustomShaderCopy(Ref<ShaderMaterial> customShader) {
    Ref<ShaderMaterial> newShader = memnew(ShaderMaterial);
    newShader->set_shader(customShader->get_shader());

    for (Dictionary uniform : customShader->get_shader()->get_shader_uniform_list()) {
        String uniformName = uniform["name"];
        newShader->set_shader_parameter((StringName)uniformName, customShader->get_shader_parameter((StringName)uniformName));
    }

    return newShader;
}

bool Utils::isPowerOfTwo(int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

Color Utils::getPixelLinear(Ref<Image> image, float x, float y) {
    int x0 = Math::max(0, (int)Math::floor(x));
    int x1 = Math::min(x0 + 1, image->get_width() - 1);
    int y0 = Math::max(0, (int) Math::floor(y));
    int y1 = Math::min(y0 + 1, image->get_height() - 1);

    float uRatio = x - x0;
    float vRatio = y - y0;
    float uOpposite = 1 - uRatio;
    float vOpposite = 1 - vRatio;

    Color result =
        image->get_pixel(x0, y0) * uOpposite * vOpposite +
        image->get_pixel(x1, y0) * uRatio * vOpposite +
        image->get_pixel(x0, y1) * uOpposite * vRatio +
        image->get_pixel(x1, y1) * uRatio * vRatio;

    return result;
}
