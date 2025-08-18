#ifndef UTILS_H
#define UTILS_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class Utils {
    public:
        static float getNextFloatWithSeed(int seed, float minValue, float maxValue);
        static int getNextIntWithSeed(int seed, int minValue, int maxValue);
        static String pathCombineForwardSlash(String directory, String path);
        static Ref<Texture2DArray> texturesToTextureArray(TypedArray<Ref<Texture2D>> textures);
        static Ref<ShaderMaterial> createCustomShaderCopy(Ref<ShaderMaterial> customShader);
        static bool isPowerOfTwo(int x);
        static Color getPixelLinear(Ref<Image> image, float x, float y);

        // TODO : GDExtension
        // static Task<Texture2D> WaitForTextureReady(Texture2D texture); This one wont be converted for now as async/await is not something that exists in cpp
};
#endif