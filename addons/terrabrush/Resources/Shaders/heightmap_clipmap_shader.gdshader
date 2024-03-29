shader_type spatial;
render_mode cull_back,blend_mix,depth_draw_opaque,diffuse_burley,specular_schlick_ggx;

#include "clipmap_shader_include.gdshaderinc"

uniform sampler2DArray WaterTextures : hint_default_transparent, repeat_disable;
uniform float WaterFactor = 1;
uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps : repeat_disable;
uniform int TextureDetail;
uniform int NumberOfTextures;
uniform sampler2DArray Normals;
uniform bool HasNormalTextures;
uniform sampler2DArray RoughnessTextures;
uniform bool HasRoughnessTextures;
uniform sampler2DArray HeightTextures;
uniform bool HasHeightTextures;
uniform bool UseAntitile = true;
uniform float BlendFactor;

varying vec3 _worldVertex;

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
vec4 texture_array_antitile(sampler2DArray tex, vec3 uv) {
	float frequency = 2.0;
	float scale = 1.3;
	float sharpness = 0.7;

	// Rotate and scale UV
	float rot = 3.14 * 0.6;
	float cosa = cos(rot);
	float sina = sin(rot);
	vec3 uv2 = vec3(rotate(uv.xy, cosa, sina) * scale, uv.z);

	vec4 col0 = texture(tex, uv);
	vec4 col1 = texture(tex, uv2);

	// Periodically alternate between the two versions using a warped checker pattern
	float t = 1.1 + 0.5
		* sin(uv2.x * frequency + sin(uv.x) * 2.0)
		* cos(uv2.y * frequency + sin(uv.y) * 2.0);
	t = smoothstep(sharpness, 2.0 - sharpness, t);

	return depth_blend2(col0, col0.a, col1, col1.a, t);
}

void vertex() {
	_worldVertex = vec3(0);
	calculateVertex(MODEL_MATRIX, COLOR, VERTEX, _worldVertex);
	
	vec3 zoneUV = calculateZoneUV(_worldVertex);
	
	const vec3 off = vec3(1.0, 1.0, 0.0);
	float hL = calculateVertexHeight(COLOR, VERTEX, _worldVertex, -off.xz);
	float hR = calculateVertexHeight(COLOR, VERTEX, _worldVertex, off.xz);
	float hB = calculateVertexHeight(COLOR, VERTEX, _worldVertex, -off.zy);
	float hF = calculateVertexHeight(COLOR, VERTEX, _worldVertex, off.zy);
	NORMAL = normalize(vec3(hL - hR, 2.0, hB - hF));
	
	vec4 waterTexture = texture(WaterTextures, zoneUV);
	VERTEX.y -= waterTexture.r * WaterFactor;
}

float getChannelValue(int currentChannel, vec4 currentSplatmap) {
	return currentSplatmap[currentChannel];
}

float contrast(float factor, float v) {
	return max(0.0, min(1.0, factor * (v - 0.5) + 0.5));
}

void fragment() {
	vec4 resultTexture = vec4(0.0);
	vec4 resultNormal = vec4(0.0);
	vec4 resultRoughness = vec4(0.0);	
	
	vec3 zoneUV = calculateZoneUV(_worldVertex);		
	
	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		vec3 detailUV = vec3(zoneUV.x, zoneUV.y, zoneUV.z);
		vec3 textureUV = vec3(detailUV.x * float(TextureDetail), detailUV.y * float(TextureDetail), float(i));
		vec3 splatmapUV = vec3(detailUV.x, detailUV.y, floor(float(i/4)) + (detailUV.z * ceil(float(NumberOfTextures) / 4.0)));
	
		vec4 currentTexture = UseAntitile ? texture_array_antitile(Textures, textureUV) : texture(Textures, textureUV);
		vec4 currentNormal = HasNormalTextures ? (UseAntitile ? texture_array_antitile(Normals, textureUV) : texture(Normals, textureUV)) : vec4(0);
		vec4 currentRoughness = HasRoughnessTextures ? (UseAntitile ? texture_array_antitile(RoughnessTextures, textureUV) : texture(RoughnessTextures, textureUV)) : vec4(0);
		vec4 currentHeight = HasHeightTextures ? (UseAntitile ? texture_array_antitile(HeightTextures, textureUV) : texture(HeightTextures, textureUV)) : vec4(0);
		vec4 currentSplatmap = texture(Splatmaps, splatmapUV);
	
		float channelValue = getChannelValue(currentChannel, currentSplatmap);
	
		float heightBlendedChannelValue = channelValue;
		if (HasHeightTextures) {
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
	
	ALBEDO = resultTexture.xyz;
	
	if (HasRoughnessTextures) {
		ROUGHNESS = resultRoughness.x;		
	}
	
	if (HasNormalTextures) {
		NORMAL_MAP = resultNormal.xyz;	
	}
	
	ALPHA = 1.0;
	ALPHA_SCISSOR_THRESHOLD = 1.0;
	
	if (zoneUV.z < 0.0) {
		ALPHA = 0.0;
	}
}