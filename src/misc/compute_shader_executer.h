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
    void addImageUniform(int binding, int width, int height, Image::Format format, BitField<RenderingDevice::TextureUsageBits> usage, RenderingDevice::TextureType textureType);

protected:
    static void _bind_methods();

public:
    ComputeShaderInstance();
    ~ComputeShaderInstance();

    void init(RenderingDevice *renderingDevice, String shaderPath);
    void freeShaderObjects();
    void addReadWriteImageUniform(int binding, int width, int height, Image::Format format);
    void addReadWriteImagesUniform(int binding, int width, int height, Image::Format format);
    void addReadOnlyImageUniform(int binding, int width, int height, Image::Format format);
    void addReadOnlyImagesArrayUniform(int binding, int width, int height, Image::Format format);
    void createUniformSet();
    void updateImageUniform(int binding, Ref<Image> image);
    void updateImagesUniform(int binding, TypedArray<Ref<Image>> images);
    PackedByteArray getImageResult(int binding);
    TypedArray<PackedByteArray> getImagesResult(int binding, int numberOfImages = RenderingDevice::LIMIT_MAX_TEXTURE_ARRAY_LAYERS);

    template<typename T>
    void addStructUniform(int binding) {
        int size = (int) Math::ceil(sizeof(T) / 16.0) * 16;

        RID uniformID  = _renderingDevice->uniform_buffer_create(size);
        _uniformIDs.set(binding, uniformID);

        Ref<RDUniform> uniform = memnew(RDUniform);
        uniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
        uniform->set_binding(binding);
        uniform->add_id(uniformID);
        _uniforms.append(uniform);
    }
    template<typename T>
    void updateStructUniform(int binding, T structInstance) {
        ERR_FAIL_COND_MSG(!_uniformIDs.has(binding), "Could not find uniform with binding " + String::num_int64(binding));

        RID uniformID = _uniformIDs[binding];

        int size = (int) Math::ceil(sizeof(T) / 16.0) * 16;

        PackedByteArray bytes;
        bytes.resize(size);
        memcpy(bytes.ptrw(), &structInstance, size);

        _renderingDevice->buffer_update(uniformID, 0, size, bytes);
    }
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
