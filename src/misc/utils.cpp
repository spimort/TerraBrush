#include "utils.h"
#include "setting_contants.h"

#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/os.hpp>

using namespace godot;

float Utils::InfinityValue = std::numeric_limits<float>::infinity();

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
    TypedArray<Ref<Image>> textureImageArray = TypedArray<Ref<Image>>();

    int width = 0;
    int height = 0;

    if (!textures.is_empty()) {
        Ref<Texture2D> firstTexture = Ref<Texture2D>(textures[0]);
        if (firstTexture.is_null()) {
            return nullptr;
        }

        Ref<Image> firstImage = firstTexture->get_image();
        if (firstImage.is_null()) {
            return nullptr;
        }

        Image::Format expectedFormat = firstImage->get_format();
        bool expectedMipmaps = firstImage->has_mipmaps();
        int expectedWidth = firstImage->get_width();
        int expectedHeight = firstImage->get_height();

        for (Ref<Texture2D> texture : textures) {
            if (!texture.is_null()) {
                Ref<Image> textureImage = texture->get_image();

                ERR_FAIL_COND_V_MSG(textureImage->get_format() != expectedFormat, nullptr, "The image '" + texture->get_path() + "' does not have the expected format : '" + String::num_int64(expectedFormat) + "'. Actual format : '" + String::num_int64(textureImage->get_format()) + "'");
                ERR_FAIL_COND_V_MSG(textureImage->has_mipmaps() != expectedMipmaps, nullptr, "The image '" + texture->get_path() + "' does not have the expected mipmaps : '" + expectedMipmaps + "'. Actual mipmaps : '" + textureImage->has_mipmaps() + "'");
                ERR_FAIL_COND_V_MSG(textureImage->get_width() != expectedWidth, nullptr, "The image '" + texture->get_path() + "' does not have the expected width : '" + String::num_int64(expectedWidth) + "'. Actual width : '" + String::num_int64(textureImage->get_width()) + "'");
                ERR_FAIL_COND_V_MSG(textureImage->get_height() != expectedHeight, nullptr, "The image '" + texture->get_path() + "' does not have the expected height : '" + String::num_int64(expectedHeight) + "'. Actual height : '" + String::num_int64(textureImage->get_height()) + "'");

                if (!textureImage.is_null()) {
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
    }

    if (textureImageArray.size() == 0) {
        return nullptr;
    }

    Ref<Texture2DArray> textureArray = memnew(Texture2DArray);
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
    // Align to pixel centers (GPU-equivalent)
    x -= 0.5f;
    y -= 0.5f;

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

float Utils::roundToDecimalPlace(float value, int decimalPlace) {
    const double multiplier = std::pow(10.0, decimalPlace);
    return std::ceil(value * multiplier) / multiplier;
}

Ref<Image> Utils::getBrushImageForIndex(const int targetIndex) {
    TypedArray<Ref<DirAccess>> directories = TypedArray<Ref<DirAccess>>();
    directories.append(DirAccess::open("res://addons/terrabrush/Assets/Brushes/"));

    String customBrushesDirectory = ProjectSettings::get_singleton()->get_setting(SettingContants::CustomBrushesFolder(), SettingContants::CustomBrushesFolderDefaultValue());
    if (!customBrushesDirectory.is_empty() && DirAccess::dir_exists_absolute(customBrushesDirectory)) {
        directories.append(DirAccess::open(customBrushesDirectory));
    }

    int index = 0;
    for (int directoryIndex = 0; directoryIndex < directories.size(); directoryIndex++) {
        Ref<DirAccess> directory = directories[directoryIndex];

        PackedStringArray files = directory->get_files();
        for (int fileIndex = 0; fileIndex < files.size(); fileIndex++) {
            String file = files[fileIndex];

            // Images are not listed on the web export, only the .import files.
            // The .import file name will be used to find the .png files.
            if (OS::get_singleton()->get_name() == "Web") {
                file = file.replace(".import", "");
            }

            if (file.ends_with(".png") || file.ends_with(".PNG")) {
                if (index == targetIndex) {
                    Ref<Texture2D> brushImage = ResourceLoader::get_singleton()->load(directory->get_current_dir() + "/" + file);
                    if (brushImage.is_null()) {
                        return nullptr;
                    }

                    return brushImage->get_image();
                }

                index++;
            }
        }
    }

    return nullptr;
}