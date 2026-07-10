#include "compute_shader_executer.h"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rd_shader_file.hpp>
#include <godot_cpp/classes/rd_shader_spirv.hpp>
#include <godot_cpp/classes/rd_texture_format.hpp>
#include <godot_cpp/classes/rd_texture_view.hpp>

using namespace godot;

void ComputeShaderExecuter::_bind_methods() {}

ComputeShaderExecuter::ComputeShaderExecuter() {}

ComputeShaderExecuter::~ComputeShaderExecuter() {
    freeShaderObjects();
}

void ComputeShaderExecuter::init() {
    if (_renderingDevice != nullptr) {
        return;
    }

    _renderingDevice = RenderingServer::get_singleton()->create_local_rendering_device();
}

void ComputeShaderExecuter::freeShaderObjects() {
    if (_renderingDevice == nullptr) {
        return;
    }

    for (Ref<ComputeShaderInstance> instance : _instances) {
        instance->freeShaderObjects();
    }

    memdelete(_renderingDevice);
    _renderingDevice = nullptr;
}

Ref<ComputeShaderInstance> ComputeShaderExecuter::addInstance(String shaderPath) {
    Ref<ComputeShaderInstance> instance = memnew(ComputeShaderInstance);

    instance->init(_renderingDevice, shaderPath);
    _instances.append(instance);

    return instance;
}

// ---------------------
// ComputeShaderInstance
// ---------------------

void ComputeShaderInstance::_bind_methods() {}

ComputeShaderInstance::ComputeShaderInstance() {}

ComputeShaderInstance::~ComputeShaderInstance() {
    _renderingDevice = nullptr;
}

void ComputeShaderInstance::init(RenderingDevice *renderingDevice, String shaderPath) {
    _renderingDevice = renderingDevice;

    Ref<RDShaderFile> shaderResource = ResourceLoader::get_singleton()->load(shaderPath);
    Ref<RDShaderSPIRV> shaderSpirv = shaderResource->get_spirv();
    _computeShader = renderingDevice->shader_create_from_spirv(shaderSpirv);

    _pipeline = renderingDevice->compute_pipeline_create(_computeShader);
}

void ComputeShaderInstance::freeShaderObjects() {
    _renderingDevice->free_rid(_pipeline);
    _pipeline = RID();

    _renderingDevice->free_rid(_uniformSet);
    _uniformSet = RID();

    for (RID shaderID : _uniformIDs.values()) {
        _renderingDevice->free_rid(shaderID);
    }
    _uniformIDs.clear();

    _renderingDevice->free_rid(_computeShader);
    _computeShader = RID();
}

void ComputeShaderInstance::addImageUniform(int binding, int width, int height, Image::Format format, BitField<RenderingDevice::TextureUsageBits> usage) {
    Ref<RDTextureFormat> textureFormat = memnew(RDTextureFormat);
    textureFormat->set_format(getDataFormatForImageFormat(format));
    textureFormat->set_width(width);
    textureFormat->set_height(height);
    textureFormat->set_usage_bits(usage);

    RID uniformID = _renderingDevice->texture_create(textureFormat, memnew(RDTextureView));
    _uniformIDs.set(binding, uniformID);

    Ref<RDUniform> uniform = memnew(RDUniform);
    uniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_IMAGE);
    uniform->set_binding(binding);
    uniform->add_id(uniformID);
    _uniforms.append(uniform);

    _xDispatchGroup = width / 8;
    _yDispatchGroup = height / 8;
}

void ComputeShaderInstance::addReadWriteImageUniform(int binding, int width, int height, Image::Format format) {
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT);
}

void ComputeShaderInstance::addReadOnlyImageUniform(int binding, int width, int height, Image::Format format) {
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT);
}

template<typename T>
void ComputeShaderInstance::addStructUniform(int binding) {
    int size = (int) Math::ceil(sizeof(T) / 16.0) * 16;

    RID uniformID  = _renderingDevice->uniform_buffer_create(size);
    _uniformIDs.set(binding, uniformID);

    Ref<RDUniform> paintSettingsUniform = memnew(RDUniform);
    paintSettingsUniform->set_uniform_type(RenderingDevice::UNIFORM_TYPE_UNIFORM_BUFFER);
    paintSettingsUniform->set_binding(binding);
    paintSettingsUniform->add_id(uniformID);
}

void ComputeShaderInstance::createUniformSet() {
    _uniformSet = _renderingDevice->uniform_set_create(_uniforms, _computeShader, 0);
    _uniforms.clear();
}

RenderingDevice::DataFormat ComputeShaderInstance::getDataFormatForImageFormat(Image::Format format) {
    switch (format) {
        case Image::Format::FORMAT_RGBA8:
            return RenderingDevice::DATA_FORMAT_R8G8B8A8_UNORM;
        case Image::Format::FORMAT_RF:
            return RenderingDevice::DATA_FORMAT_R32_SFLOAT;
        case Image::Format::FORMAT_RGF:
            return RenderingDevice::DATA_FORMAT_R32G32_SFLOAT;
        default:
            ERR_FAIL_V_MSG(RenderingDevice::DataFormat::DATA_FORMAT_MAX, "Unsupported dataformat for image format " + String::num(format));
    }
}

void ComputeShaderInstance::updateImageUniform(int binding, Ref<Image> image) {
    ERR_FAIL_COND_MSG(!_uniformIDs.has(binding), "Could not find uniform with binding " + String::num_int64(binding));

    RID uniformID = _uniformIDs[binding];
    _renderingDevice->texture_update(uniformID, 0, image->get_data());
}

template<typename T>
void ComputeShaderInstance::updateStructUniform(int binding, T structInstance) {
    ERR_FAIL_COND_MSG(!_uniformIDs.has(binding), "Could not find uniform with binding " + String::num_int64(binding));

    RID uniformID = _uniformIDs[binding];

    int size = (int) Math::ceil(sizeof(T) / 16.0) * 16;

    PackedByteArray bytes;
    bytes.resize(size);
    memcpy(bytes.ptrw(), &structInstance, size);

    _renderingDevice->buffer_update(uniformID, 0, size, bytes);
}

PackedByteArray ComputeShaderInstance::getImageResult(int binding) {
    ERR_FAIL_COND_V_MSG(!_uniformIDs.has(binding), PackedByteArray(), "Could not find uniform with binding " + String::num_int64(binding));

    RID uniformID = _uniformIDs[binding];

    int64_t computeList = _renderingDevice->compute_list_begin();
    _renderingDevice->compute_list_bind_compute_pipeline(computeList, _pipeline);
    _renderingDevice->compute_list_bind_uniform_set(computeList, _uniformSet, 0);
    _renderingDevice->compute_list_dispatch(computeList, _xDispatchGroup, _yDispatchGroup, 1);
    _renderingDevice->compute_list_end();

    _renderingDevice->submit();
    _renderingDevice->sync();

    PackedByteArray result = _renderingDevice->texture_get_data(uniformID, 0);
    return result;
}
