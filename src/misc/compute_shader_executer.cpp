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

void ComputeShaderInstance::addImageUniform(int binding, int width, int height, Image::Format format, BitField<RenderingDevice::TextureUsageBits> usage, RenderingDevice::TextureType textureType) {
    Ref<RDTextureFormat> textureFormat = memnew(RDTextureFormat);
    textureFormat->set_format(getDataFormatForImageFormat(format));
    textureFormat->set_width(width);
    textureFormat->set_height(height);
    textureFormat->set_usage_bits(usage);
    textureFormat->set_texture_type(textureType);

    if (textureType == RenderingDevice::TextureType::TEXTURE_TYPE_2D_ARRAY) {
        textureFormat->set_array_layers(RenderingDevice::LIMIT_MAX_TEXTURE_ARRAY_LAYERS);
    }

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
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT, RenderingDevice::TextureType::TEXTURE_TYPE_2D);
}

void ComputeShaderInstance::addReadWriteImagesUniform(int binding, int width, int height, Image::Format format) {
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_COPY_FROM_BIT, RenderingDevice::TextureType::TEXTURE_TYPE_2D_ARRAY);
}

void ComputeShaderInstance::addReadOnlyImageUniform(int binding, int width, int height, Image::Format format) {
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT, RenderingDevice::TextureType::TEXTURE_TYPE_2D);
}

void ComputeShaderInstance::addReadOnlyImagesArrayUniform(int binding, int width, int height, Image::Format format) {
    addImageUniform(binding, width, height, format, RenderingDevice::TEXTURE_USAGE_STORAGE_BIT + RenderingDevice::TEXTURE_USAGE_CAN_UPDATE_BIT, RenderingDevice::TextureType::TEXTURE_TYPE_2D_ARRAY);
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

void ComputeShaderInstance::updateImagesUniform(int binding, TypedArray<Ref<Image>> images) {
    ERR_FAIL_COND_MSG(!_uniformIDs.has(binding), "Could not find uniform with binding " + String::num_int64(binding));

    RID uniformID = _uniformIDs[binding];
    for (int i = 0; i < images.size(); i++) {
        Ref<Image> image = images[i];
        _renderingDevice->texture_update(uniformID, i, image->get_data());
    }
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

TypedArray<PackedByteArray> ComputeShaderInstance::getImagesResult(int binding, int numberOfImages) {
    ERR_FAIL_COND_V_MSG(!_uniformIDs.has(binding), TypedArray<PackedByteArray>(), "Could not find uniform with binding " + String::num_int64(binding));

    RID uniformID = _uniformIDs[binding];

    int64_t computeList = _renderingDevice->compute_list_begin();
    _renderingDevice->compute_list_bind_compute_pipeline(computeList, _pipeline);
    _renderingDevice->compute_list_bind_uniform_set(computeList, _uniformSet, 0);
    _renderingDevice->compute_list_dispatch(computeList, _xDispatchGroup, _yDispatchGroup, 1);
    _renderingDevice->compute_list_end();

    _renderingDevice->submit();
    _renderingDevice->sync();

    TypedArray<PackedByteArray> results = TypedArray<PackedByteArray>();
    for (int i = 0; i < numberOfImages; i++) {
        PackedByteArray data = _renderingDevice->texture_get_data(uniformID, i);
        if (data.size() > 0) {
            results.append(data);
        }
    }
    return results;
}
