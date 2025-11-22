#ifndef FOLIAGE_DEFINITION_RESOURCE_H
#define FOLIAGE_DEFINITION_RESOURCE_H

#include "../misc/enums.h"

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;

class FoliageDefinitionResource : public Resource {
    GDCLASS(FoliageDefinitionResource, Resource);

private:
    FoliageStrategy _strategy = FoliageStrategy::FOLIAGESTRATEGY_MULTIMESH;
    Ref<Mesh> _mesh = nullptr;
    Vector3 _meshScale = Vector3();
    float _windStrength = 0;
    Ref<Texture2D> _noiseTexture = nullptr;
    int _visualInstanceLayers = 0;

    // _multimesh settings
    int _lodLevels = 0;
    int _lodRowsPerLevel = 0;
    float _lodInitialCellWidth = 0;
    Color _albedo = Color();
    TypedArray<Ref<Texture2D>> _albedoTextures = TypedArray<Ref<Texture2D>>();
    bool _nearestTextureFilter = false;
    bool _useGroundColor = false;
    bool _castShadow = false;
    bool _useBrushScale = false;
    Ref<Texture2D> _scaleNoiseTexture = nullptr;
    float _randomPlacementRange = 0;
    TypedArray<int> _applyOnTextureIndexes = TypedArray<int>();
    Ref<ShaderMaterial> _customShader = nullptr;

    // _particles settings
    Ref<Material> _meshMaterial = nullptr;
    int _maximumRenderDistance = 0;
    int _editorMaximumRenderDistance = 0;

protected:
    static void _bind_methods();
    void _validate_property(PropertyInfo &property) const;

public:
    FoliageDefinitionResource();
    ~FoliageDefinitionResource();

    FoliageStrategy get_strategy() const;
    void set_strategy(const FoliageStrategy value);

    Ref<Mesh> get_mesh() const;
    void set_mesh(const Ref<Mesh> &value);

    Vector3 get_meshScale() const;
    void set_meshScale(const Vector3 value);

    float get_windStrength() const;
    void set_windStrength(const float value);

    Ref<Texture2D> get_noiseTexture() const;
    void set_noiseTexture(const Ref<Texture2D> &value);

    int get_visualInstanceLayers() const;
    void set_visualInstanceLayers(const int value);

    // _multimesh settings
    int get_lodLevels() const;
    void set_lodLevels(const int value);

    int get_lodRowsPerLevel() const;
    void set_lodRowsPerLevel(const int value);

    float get_lodInitialCellWidth() const;
    void set_lodInitialCellWidth(const float value);

    Color get_albedo() const;
    void set_albedo(const Color value);

    TypedArray<Ref<Texture2D>> get_albedoTextures() const;
    void set_albedoTextures(const TypedArray<Ref<Texture2D>>);

    bool get_nearestTextureFilter() const;
    void set_nearestTextureFilter(const bool value);

    bool get_useGroundColor() const;
    void set_useGroundColor(const bool value);

    bool get_castShadow() const;
    void set_castShadow(const bool value);

    bool get_useBrushScale() const;
    void set_useBrushScale(const bool value);

    Ref<Texture2D> get_scaleNoiseTexture() const;
    void set_scaleNoiseTexture(const Ref<Texture2D> &value);

    float get_randomPlacementRange() const;
    void set_randomPlacementRange(const float value);

    TypedArray<int> get_applyOnTextureIndexes() const;
    void set_applyOnTextureIndexes(const TypedArray<int>);

    Ref<ShaderMaterial> get_customShader() const;
    void set_customShader(const Ref<ShaderMaterial> &value);

    // _particles settings
    Ref<Material> get_meshMaterial() const;
    void set_meshMaterial(const Ref<Material> &value);

    int get_maximumRenderDistance() const;
    void set_maximumRenderDistance(const int value);

    int get_editorMaximumRenderDistance() const;
    void set_editorMaximumRenderDistance(const int value);
};
#endif