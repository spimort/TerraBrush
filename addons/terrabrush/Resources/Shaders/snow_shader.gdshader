shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix, depth_prepass_alpha;

uniform sampler2D SnowTexture: hint_default_transparent, repeat_disable;
uniform sampler2D HeightMapTexture: hint_default_transparent, repeat_disable;
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

float Remap (float value, float from1, float to1, float from2, float to2) {
    return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void fragment() {
	if (COLOR[0] > 0.0f) {
		ALBEDO = texture(SnowColorTexture, UV * SnowColorDetail).rgb;
		NORMAL_MAP = texture(SnowColorNormal, UV * SnowColorDetail).rgb;
		METALLIC = Metallic;
		ROUGHNESS = texture(SnowColorRoughness, UV * SnowColorDetail).r;
	} else {
		ALPHA = 0.0;
	}
}

void vertex() {
	vec4 currentSnowColor = texture(SnowTexture, UV);
	vec4 currentHeightmapColor = texture(HeightMapTexture, UV);
	vec4 currentNoiseColor = texture(Noise, UV);
	vec4 currentCompressed = texture(CompressedSnowTexture, UV);

	if (currentSnowColor.r > 0.0f) {
		float compressedSnowValue = Remap(currentCompressed.r, 0.0, 1.0, 1.0, 0.0);

		VERTEX.y = ((currentHeightmapColor.r * HeightMapFactor) + (currentSnowColor.r * SnowFactor) * compressedSnowValue) + (currentNoiseColor.r * NoiseFactor) - SnowInnerOffset;

		COLOR = vec4(currentSnowColor.r, currentSnowColor.g, currentSnowColor.b, 1.0);
	} else {
		VERTEX.y = (currentHeightmapColor.r * HeightMapFactor) - 1.0;
		COLOR = vec4(0.0f, 0.0f, 0.0f, 0.0);
	}
}