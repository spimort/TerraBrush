#ifndef STRING_NAMES_H
#define STRING_NAMES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class StringNames {
    public:
        static StringName HeightmapTextures();
        static StringName ZonesSize();
        static StringName NumberOfZones();
        static StringName ZonesMap();
        static StringName GlobalPosition();
        static StringName Splatmaps();
        static StringName Textures();
        static StringName NumberOfTextures();
        static StringName TextureDetail();
        static StringName FoliageTextures();
        static StringName MeshScale();
        static StringName WindStrength();
        static StringName WaterTextures();
        static StringName WaterFactor();
        static StringName NoiseTexture();
        static StringName MaximumDistance();
        static StringName SnowTextures();
        static StringName SnowFactor();
        static StringName SnowInnerOffset();
        static StringName SnowColorTexture();
        static StringName SnowColorNormal();
        static StringName SnowColorRoughness();
        static StringName SnowColorDetail();
        static StringName Noise();
        static StringName NoiseFactor();
        static StringName Metallic();
        static StringName NearestFilter();
        static StringName TexturesDetail();
        static StringName HasNormalTextures();
        static StringName HasRoughnessTextures();
        static StringName HasHeightTextures();
        static StringName UseAntitile();
        static StringName BlendFactor();
        static StringName WaterInnerOffset();
        static StringName WaterColor();
        static StringName FresnelColor();
        static StringName Roughness();
        static StringName NormalMap();
        static StringName NormalMap2();
        static StringName TimeScale();
        static StringName Strength();
        static StringName Wave();
        static StringName NoiseScale();
        static StringName HeightScale();
        static StringName ColorDeep();
        static StringName ColorShallow();
        static StringName BeersLaw();
        static StringName DepthOffset();
        static StringName EdgeScale();
        static StringName Near();
        static StringName Far();
        static StringName EdgeColor();
        static StringName SetAnglePointContainer();
        static StringName InitialCellWidth();
        static StringName LODRowsPerLevel();
        static StringName LODLevels();
        static StringName FoliageAlbedo();
        static StringName FoliageAlbedoTextures();
        static StringName FoliageAlbedoTexturesNearest();
        static StringName UseGroundColor();
        static StringName FoliageNumberOfTexture();
        static StringName UseBrushScale();
        static StringName ScaleNoiseTexture();
        static StringName RandomPlacementRange();
        static StringName InvertedZ();
        static StringName TerrainLoaded();
        static StringName LockTextures();
        static StringName ApplyLockTextures();
        static StringName ApplyOnTextureIndexes();
        static StringName NumberOfTexturesToApplyOn();
        static StringName AlbedoAlphaChannelUsage();
        static StringName NormalAlphaChannelUsage();
        static StringName OffsetPosition();
        static StringName Resolution();
        static StringName Triplanar();
        static StringName TexturesTriplanar();
        static StringName UseSharpTransitions();
        static StringName ApplyMetaInfoTextures();
        static StringName MetaInfoTextures();
        static StringName MetaInfoColors();
};
#endif