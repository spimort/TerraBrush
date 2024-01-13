shader_type spatial;
render_mode blend_mix,depth_draw_opaque,cull_disabled,diffuse_burley,specular_schlick_ggx;

#include "clipmap_shader_include.gdshaderinc"

uniform sampler2D WaterTexture : repeat_disable;
uniform float WaterFactor = 1;
uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps : repeat_disable;
uniform int TextureDetail;
uniform int NumberOfTextures;
uniform sampler2DArray Normals;
uniform sampler2DArray RoughnessTexutres;
uniform bool UseAntitile = true;

varying vec2 _textureUV;
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
	_textureUV = vec2(0);
	_worldVertex = vec3(0);
	calculateVertex(MODEL_MATRIX, COLOR, VERTEX, _textureUV, _worldVertex);
	
	vec4 waterTexture = texture(WaterTexture, _textureUV);
	VERTEX.y -= waterTexture.r * WaterFactor;
}

void fragment() {
	vec4 resultTexture = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 resultNormal = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 resultRoughness = vec4(0.0, 0.0, 0.0, 0.0);	
	
	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		vec2 detailUV = _textureUV * float(TextureDetail);
		vec3 textureUV = vec3(detailUV.x, detailUV.y, float(i));
		vec3 splatmapUV = vec3(_textureUV.x, _textureUV.y, floor(float(i/4)));
	
		vec4 currentTexture = UseAntitile ? texture_array_antitile(Textures, textureUV) : texture(Textures, textureUV) ;
		vec4 currentNormal = UseAntitile ? texture_array_antitile(Normals, textureUV) : texture(Normals, textureUV);
		vec4 currentRoughness = UseAntitile ? texture_array_antitile(RoughnessTexutres, textureUV) : texture(RoughnessTexutres, textureUV);
		vec4 currentSplatmap = texture(Splatmaps, splatmapUV);
	
		float channelValue = 0.0;
		if (currentChannel == 0) {
			channelValue = currentSplatmap.x;
		} else if (currentChannel == 1) {
			channelValue = currentSplatmap.y;
		} else if (currentChannel == 2) {
			channelValue = currentSplatmap.z;
		} else if (currentChannel == 3) {
			channelValue = currentSplatmap.w;
		}
	
		resultTexture += currentTexture * channelValue;
		resultNormal += currentNormal * channelValue;
		resultRoughness += currentRoughness * channelValue;
	
		if (currentChannel == 3) {
			currentChannel = 0;
		} else {
			currentChannel++;
		}
	}
	
	ALBEDO = resultTexture.xyz;
	ROUGHNESS = resultRoughness.x;
	NORMAL_MAP = resultNormal.xyz;	
	ALPHA = 1.0;
	ALPHA_SCISSOR_THRESHOLD = 1.0;
	
	// "Hide" vertices that are outside the map
	float heightmapSize = float(textureSize(HeightmapTexture, 0).x) - 1.0;
	float halfHeightmapSize = heightmapSize / 2.0;	
	if (
		_worldVertex.x > halfHeightmapSize || 
		_worldVertex.x < -halfHeightmapSize || 
		_worldVertex.z > halfHeightmapSize ||
		_worldVertex.z < -halfHeightmapSize
	) {
		ALPHA = 0.0;
	}		
}