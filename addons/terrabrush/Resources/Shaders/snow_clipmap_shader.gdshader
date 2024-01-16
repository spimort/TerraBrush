shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix, depth_prepass_alpha;

#include "clipmap_shader_include.gdshaderinc"

uniform sampler2D SnowTexture: hint_default_transparent, repeat_disable;
uniform float SnowFactor = 1.0;
uniform float SnowInnerOffset = 0.3;
uniform float HeightMapFactor;
uniform sampler2D SnowColorTexture: source_color, filter_linear_mipmap, repeat_enable;
uniform sampler2D SnowColorNormal: source_color, filter_linear_mipmap, repeat_enable;
uniform sampler2D SnowColorRoughness: source_color, filter_linear_mipmap, repeat_enable;
uniform float SnowColorDetail = 20;
uniform sampler2D Noise: hint_default_transparent, repeat_disable;
uniform float NoiseFactor = 0.1;
uniform float Metallic: hint_range(0.0, 1.0, 0.001); 
uniform sampler2D CompressedSnowTexture: hint_default_black;

varying vec2 _textureUV;
varying vec3 _worldVertex;

float Remap (float value, float from1, float to1, float from2, float to2) {
    return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void fragment() {
	if (COLOR[0] > 0.0f) {
		ALBEDO = texture(SnowColorTexture, _textureUV * SnowColorDetail).rgb;
		NORMAL_MAP = texture(SnowColorNormal, _textureUV * SnowColorDetail).rgb;
		METALLIC = Metallic;
		ROUGHNESS = texture(SnowColorRoughness, _textureUV * SnowColorDetail).r;
		
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
	} else {
		ALPHA = 0.0;
	}
}

void vertex() {
	_textureUV = vec2(0);
	_worldVertex = vec3(0);
	calculateVertex(MODEL_MATRIX, COLOR, VERTEX, _textureUV, _worldVertex);
	
	vec4 currentSnowColor = texture(SnowTexture, _textureUV);
	vec4 currentNoiseColor = texture(Noise, _textureUV);
	vec4 currentCompressed = texture(CompressedSnowTexture, _textureUV);

	if (currentSnowColor.r > 0.0f) {
		float compressedSnowValue = Remap(currentCompressed.r, 0.0, 1.0, 1.0, -1.0);

		VERTEX.y += ((currentSnowColor.r * SnowFactor) * compressedSnowValue) + (currentNoiseColor.r * NoiseFactor) - SnowInnerOffset;

		COLOR = vec4(currentSnowColor.r, currentSnowColor.g, currentSnowColor.b, 1.0);
	} else {
		VERTEX.y -= 1.0;
		COLOR = vec4(0.0f, 0.0f, 0.0f, 0.0);
	}
}