#ifndef STRING_NAMES_H
#define STRING_NAMES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class StringNames {
    public:
        const static StringName HeightmapTextures();
        const static StringName ZonesSize();
        const static StringName NumberOfZones();
        const static StringName ZonesMap();
        const static StringName GlobalPosition();
        const static StringName Splatmaps();
        const static StringName Textures();
        const static StringName NumberOfTextures();
        const static StringName TextureDetail();
        const static StringName FoliageTextures();
        const static StringName MeshScale();
        const static StringName WindStrength();
        const static StringName WaterTextures();
        const static StringName WaterFactor();
        const static StringName NoiseTexture();
        const static StringName MaximumDistance();
        const static StringName SnowTextures();
        const static StringName SnowFactor();
        const static StringName SnowInnerOffset();
        const static StringName SnowColorTexture();
        const static StringName SnowColorNormal();
        const static StringName SnowColorRoughness();
        const static StringName SnowColorDetail();
        const static StringName Noise();
        const static StringName NoiseFactor();
        const static StringName Metallic();
        const static StringName NearestFilter();
        const static StringName TexturesDetail();
        const static StringName HasNormalTextures();
        const static StringName HasRoughnessTextures();
        const static StringName HasHeightTextures();
        const static StringName UseAntitile();
        const static StringName BlendFactor();
        const static StringName WaterInnerOffset();
        const static StringName WaterColor();
        const static StringName FresnelColor();
        const static StringName Roughness();
        const static StringName NormalMap();
        const static StringName NormalMap2();
        const static StringName TimeScale();
        const static StringName Strength();
        const static StringName Wave();
        const static StringName NoiseScale();
        const static StringName HeightScale();
        const static StringName ColorDeep();
        const static StringName ColorShallow();
        const static StringName BeersLaw();
        const static StringName DepthOffset();
        const static StringName EdgeScale();
        const static StringName Near();
        const static StringName Far();
        const static StringName EdgeColor();
        const static StringName SetAnglePointContainer();
        const static StringName InitialCellWidth();
        const static StringName LODRowsPerLevel();
        const static StringName LODLevels();
        const static StringName FoliageAlbedo();
        const static StringName FoliageAlbedoTextures();
        const static StringName FoliageAlbedoTexturesNearest();
        const static StringName UseGroundColor();
        const static StringName FoliageNumberOfTexture();
        const static StringName UseBrushScale();
        const static StringName ScaleNoiseTexture();
        const static StringName RandomPlacementRange();
        const static StringName InvertedZ();
        const static StringName TerrainLoaded();
        const static StringName LockTextures();
        const static StringName ApplyLockTextures();
        const static StringName ApplyOnTextureIndexes();
        const static StringName NumberOfTexturesToApplyOn();
        const static StringName AlbedoAlphaChannelUsage();
        const static StringName NormalAlphaChannelUsage();
        const static StringName OffsetPosition();
        const static StringName Resolution();
        const static StringName Triplanar();
        const static StringName TexturesTriplanar();
        const static StringName UseSharpTransitions();
        const static StringName ApplyMetaInfoTextures();
        const static StringName MetaInfoTextures();
        const static StringName MetaInfoColors();
        const static StringName Color();
        const static StringName Compatibility();
        const static StringName ColorTextures();
        const static StringName TexturesMetallic();
        const static StringName TexturesSpecular();
};
#endif