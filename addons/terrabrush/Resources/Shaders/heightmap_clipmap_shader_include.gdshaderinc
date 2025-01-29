#include "clipmap_shader_include.gdshaderinc"

uniform bool ApplyLockTextures = false;
uniform sampler2DArray LockTextures : hint_default_black;
uniform sampler2DArray WaterTextures : hint_default_transparent, repeat_disable;
uniform float WaterFactor = 1;
uniform sampler2DArray Textures : source_color, filter_linear_mipmap;
uniform sampler2DArray TexturesNearest : source_color, filter_nearest_mipmap;
uniform sampler2DArray Splatmaps : repeat_disable;
uniform int[100] TexturesDetail;
uniform int NumberOfTextures;
uniform sampler2DArray Normals : filter_linear_mipmap;
uniform sampler2DArray NormalsNearest : filter_nearest_mipmap;
uniform bool HasNormalTextures;
uniform sampler2DArray RoughnessTextures : filter_linear_mipmap;
uniform sampler2DArray RoughnessTexturesNearest : filter_nearest_mipmap;
uniform bool HasRoughnessTextures;
uniform sampler2DArray HeightTextures : filter_linear_mipmap;
uniform sampler2DArray HeightTexturesNearest : filter_nearest_mipmap;
uniform bool HasHeightTextures;
uniform bool UseAntitile = true;
uniform float BlendFactor;
uniform bool NearestFilter = false;
uniform int AlbedoAlphaChannelUsage = 0; // None = 0; Roughness = 1; Height = 2
uniform int NormalAlphaChannelUsage = 0; // None = 0; Roughness = 1; Height = 2

const int ALPHA_CHANNEL_USAGE_NONE = 0;
const int ALPHA_CHANNEL_USAGE_ROUGHNESS = 1;
const int ALPHA_CHANNEL_USAGE_HEIGHT = 2;

vec2 rotate(vec2 v, float cosa, float sina) {
	return vec2(cosa * v.x - sina * v.y, sina * v.x + cosa * v.y);
}

vec4 depth_blend2(vec4 a_value, float a_bump, vec4 b_value, float b_bump, float t) {
	// https://www.gamasutra.com
	// /blogs/AndreyMishkinis/20130716/196339/Advanced_Terrain_Texture_Splatting.php
	float d = 0.1;
	float ma = max(a_bump + (1.0 - t), b_bump + t) - d;
	float ba = max(a_bump + (1.0 - t) - ma, 0.0);
	float bb = max(b_bump + t - ma, 0.0);
	return (a_value * ba + b_value * bb) / (ba + bb);
}

// Antitile from zylann's shader
vec4 texture_array_antitile(sampler2DArray texLinear, sampler2DArray texNearest, vec3 uv) {
	float frequency = 2.0;
	float scale = 1.3;
	float sharpness = 0.7;

	// Rotate and scale UV
	float rot = 3.14 * 0.6;
	float cosa = cos(rot);
	float sina = sin(rot);
	vec3 uv2 = vec3(rotate(uv.xy, cosa, sina) * scale, uv.z);

	vec4 col0 = vec4(0);
	vec4 col1 = vec4(0);

	if (NearestFilter) {
		col0 = texture(texNearest, uv);
		col1 = texture(texNearest, uv2);
	} else {
		col0 = texture(texLinear, uv);
		col1 = texture(texLinear, uv2);
	}

	// Periodically alternate between the two versions using a warped checker pattern
	float t = 1.1 + 0.5
		* sin(uv2.x * frequency + sin(uv.x) * 2.0)
		* cos(uv2.y * frequency + sin(uv.y) * 2.0);
	t = smoothstep(sharpness, 2.0 - sharpness, t);

	return depth_blend2(col0, col0.a, col1, col1.a, t);
}

void calculateHeightmapVertex(
	mat4 modelMatrix,
	inout vec4 color,
	inout vec3 vertex,
	inout vec3 normal,
	inout vec3 worldVertex
) {
	calculateVertex(modelMatrix, color, vertex, worldVertex);

	vec3 zoneUV = calculateZoneUV(worldVertex);

	const vec3 off = vec3(1.0, 1.0, 0.0);
	float hL = calculateVertexHeight(color, vertex, worldVertex, -off.xz);
	float hR = calculateVertexHeight(color, vertex, worldVertex, off.xz);
	float hB = calculateVertexHeight(color, vertex, worldVertex, -off.zy);
	float hF = calculateVertexHeight(color, vertex, worldVertex, off.zy);
	normal = normalize(vec3(hL - hR, 2.0, hB - hF));

	vec4 waterTexture = texture(WaterTextures, zoneUV);
	vertex.y -= waterTexture.r * WaterFactor;
}

float getChannelValue(int currentChannel, vec4 currentSplatmap) {
	return currentSplatmap[currentChannel];
}

float contrast(float factor, float v) {
	return max(0.0, min(1.0, factor * (v - 0.5) + 0.5));
}

void fillVec4WithFilterTexture(vec3 textureUV, sampler2DArray linearTextures, sampler2DArray nearestTextures, out vec4 resultTexture) {
	if (NearestFilter) {
		resultTexture = UseAntitile ? texture_array_antitile(linearTextures, nearestTextures, textureUV) : texture(nearestTextures, textureUV);
	} else {
		resultTexture = UseAntitile ? texture_array_antitile(linearTextures, nearestTextures, textureUV) : texture(linearTextures, textureUV);
	}
}

void calculateHeightmapFragment(
	vec3 worldVertex,
	inout vec3 albedo,
	inout vec3 normalMap,
	inout float roughness,
	inout float alpha,
	inout float alphaScissorThreshold
) {
	vec4 resultTexture = vec4(0.0);
	vec4 resultNormal = vec4(0.0);
	vec4 resultRoughness = vec4(0.0);

	vec3 zoneUV = calculateZoneUV(worldVertex);

	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		int textureDetail = TexturesDetail[i];
		vec2 worldVertexUV = worldVertex.xz * (float(textureDetail) * (1.0 / ZonesSize));
		vec3 textureUV = vec3(worldVertexUV.x, worldVertexUV.y, float(i));
		vec3 textureZoneUV = zoneUV;
		if (Resolution > 1.0) {
			textureZoneUV = calculateZoneUVResolution(worldVertex, false);
		}
		vec3 splatmapUV = vec3(textureZoneUV.x, textureZoneUV.y, floor(float(i/4)) + (textureZoneUV.z * ceil(float(NumberOfTextures) / 4.0)));

		vec4 currentTexture = vec4(0);
		vec4 currentNormal = vec4(0);
		vec4 currentRoughness = vec4(0);
		vec4 currentHeight = vec4(0);

		fillVec4WithFilterTexture(textureUV, Textures, TexturesNearest, currentTexture);
		currentRoughness = vec4(currentTexture.a * float(AlbedoAlphaChannelUsage == ALPHA_CHANNEL_USAGE_ROUGHNESS));
		currentHeight = vec4(currentTexture.a * float(AlbedoAlphaChannelUsage == ALPHA_CHANNEL_USAGE_HEIGHT));
		if (HasNormalTextures) {
			fillVec4WithFilterTexture(textureUV, Normals, NormalsNearest, currentNormal);
			currentRoughness = max(currentRoughness, vec4(currentNormal.a * float(AlbedoAlphaChannelUsage != ALPHA_CHANNEL_USAGE_ROUGHNESS && NormalAlphaChannelUsage == ALPHA_CHANNEL_USAGE_ROUGHNESS)));
			currentHeight = max(currentHeight, vec4(currentNormal.a * float(AlbedoAlphaChannelUsage != ALPHA_CHANNEL_USAGE_HEIGHT && NormalAlphaChannelUsage == ALPHA_CHANNEL_USAGE_HEIGHT)));
		}
		if (HasRoughnessTextures) {
			fillVec4WithFilterTexture(textureUV, RoughnessTextures, RoughnessTexturesNearest, currentRoughness);
		}
		if (HasHeightTextures) {
			fillVec4WithFilterTexture(textureUV, HeightTextures, HeightTexturesNearest, currentHeight);
		}

		vec4 currentSplatmap = texture(Splatmaps, splatmapUV);
		float channelValue = getChannelValue(currentChannel, currentSplatmap);

		float heightBlendedChannelValue = channelValue;
		if (HasHeightTextures || AlbedoAlphaChannelUsage == ALPHA_CHANNEL_USAGE_HEIGHT || NormalAlphaChannelUsage == ALPHA_CHANNEL_USAGE_HEIGHT) {
			heightBlendedChannelValue = min(contrast(BlendFactor, currentHeight.x) + channelValue, 1.0) * sqrt(channelValue);
		}

		resultTexture += currentTexture * heightBlendedChannelValue;
		resultNormal += currentNormal * channelValue;
		resultRoughness += currentRoughness * heightBlendedChannelValue;

		if (currentChannel == 3) {
			currentChannel = 0;
		} else {
			currentChannel++;
		}
	}

	albedo = resultTexture.xyz;

	if (ApplyLockTextures) {
		vec4 lockTexture = texture(LockTextures, zoneUV);
		float lockValue = (1.0 - lockTexture.r);
		albedo *= vec3(max(lockValue, 0.5), lockValue, lockValue);
	}

	if (HasRoughnessTextures || AlbedoAlphaChannelUsage == ALPHA_CHANNEL_USAGE_ROUGHNESS || NormalAlphaChannelUsage == ALPHA_CHANNEL_USAGE_ROUGHNESS) {
		roughness = resultRoughness.x;
	}

	if (HasNormalTextures) {
		normalMap = resultNormal.xyz;
	}

	alpha = 1.0;
	alphaScissorThreshold = 1.0;

	if (zoneUV.z < 0.0) {
		alpha = 0.0;
	} else {
		float hole = texture(HeightmapTextures, zoneUV).g;
		if (hole > 0.0) {
			alpha = 0.0;
		}
	}
}