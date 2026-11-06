#ifndef UTILS_H
#define UTILS_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/shader_material.hpp>

using namespace godot;

class Utils {
    public:
        static float InfinityValue;

        static float getNextFloatWithSeed(int seed, float minValue, float maxValue);
        static int getNextIntWithSeed(int seed, int minValue, int maxValue);
        static String pathCombineForwardSlash(String directory, String path);
        static Ref<Texture2DArray> texturesToTextureArray(TypedArray<Ref<Texture2D>> textures);
        static Ref<ShaderMaterial> createCustomShaderCopy(Ref<ShaderMaterial> customShader, const TypedArray<StringName> transparentSampler2DArrayParams = TypedArray<StringName>());
        static bool isPowerOfTwo(int x);
        static Color getPixelLinear(Ref<Image> image, float x, float y);
        static float roundToDecimalPlace(float value, int decimalPlace);
        static Ref<Image> getBrushImageForIndex(const int targetIndex);
        static float degreeAngleToSlopeFactor(const float value);
        static float slopeFactorToDegreeAngle(const float value);
};

class UtilsAsyncTextureLoader : public Object {
    GDCLASS(UtilsAsyncTextureLoader, Object);

private:
    Ref<Texture2DArray> _texture2DArray = nullptr;
    int _textureIndex = -1;
    Ref<Texture2D> _texture = nullptr;

protected:
    static void _bind_methods();

public:
    UtilsAsyncTextureLoader();
    ~UtilsAsyncTextureLoader();

    void setData(Ref<Texture2DArray> &texture2DArray, int textureIndex, Ref<Texture2D> &texture);
    void onTextureUpdated();
};
#endif
