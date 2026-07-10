#ifndef COMPUTE_SHADER_EXECUTER_H
#define COMPUTE_SHADER_EXECUTER_H

#include <godot_cpp/variant/typed_dictionary.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/rd_uniform.hpp>

using namespace godot;

class ComputeShaderInstance : public RefCounted {
    GDCLASS(ComputeShaderInstance, RefCounted);

private:
    RenderingDevice *_renderingDevice = nullptr;
    RID _computeShader = RID();
    RID _uniformSet = RID();
    RID _pipeline = RID();
    int _xDispatchGroup = 0;
    int _yDispatchGroup = 0;
    TypedDictionary<int, RID> _uniformIDs = TypedDictionary<int, RID>();
    TypedArray<Ref<RDUniform>> _uniforms = TypedArray<Ref<RDUniform>>();

    RenderingDevice::DataFormat getDataFormatForImageFormat(Image::Format format);
    void addImageUniform(int binding, int width, int height, Image::Format format, BitField<RenderingDevice::TextureUsageBits> usage);

protected:
    static void _bind_methods();

public:
    ComputeShaderInstance();
    ~ComputeShaderInstance();

    void init(RenderingDevice *renderingDevice, String shaderPath);
    void freeShaderObjects();
    void addReadWriteImageUniform(int binding, int width, int height, Image::Format format);
    void addReadOnlyImageUniform(int binding, int width, int height, Image::Format format);
    template<typename T>
    void addStructUniform(int binding);
    void createUniformSet();
    void updateImageUniform(int binding, Ref<Image> image);
    template<typename T>
    void updateStructUniform(int binding, T structInstance);
    PackedByteArray getImageResult(int binding);
};

class ComputeShaderExecuter : public RefCounted {
    GDCLASS(ComputeShaderExecuter, RefCounted);

private:
    RenderingDevice *_renderingDevice = nullptr;
    TypedArray<Ref<ComputeShaderInstance>> _instances = TypedArray<Ref<ComputeShaderInstance>>();

protected:
    static void _bind_methods();

public:
    ComputeShaderExecuter();
    ~ComputeShaderExecuter();

    void init();
    void freeShaderObjects();
    Ref<ComputeShaderInstance> addInstance(String shaderPath);
};
#endif
