#include "clipmap_shader_include.gdshaderinc"

uniform sampler2DArray SnowTextures: hint_default_transparent, repeat_disable;
uniform float SnowFactor = 1.0;
uniform float SnowInnerOffset = 0.3;
uniform sampler2D SnowColorTexture: source_color, filter_linear_mipmap, repeat_enable;
uniform sampler2D SnowColorNormal: filter_linear_mipmap, repeat_enable;
uniform sampler2D SnowColorRoughness: source_color, filter_linear_mipmap, repeat_enable;
uniform float SnowColorDetail = 20;
uniform sampler2D Noise: hint_default_transparent, repeat_disable;
uniform float NoiseFactor = 0.1;
uniform float Metallic: hint_range(0.0, 1.0, 0.001);

float remap(float value, float from1, float to1, float from2, float to2) {
    return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void calculateSnowVertex(
	mat4 modelMatrix, 
	inout vec4 color, 
	inout vec3 vertex, 
	inout vec3 normal, 
	inout vec3 worldVertex	
) {
	calculateVertex(modelMatrix, color, vertex, worldVertex);
	
	vec3 zoneUV = calculateZoneUV(worldVertex);			
	vec4 currentSnowColor = texture(SnowTextures, zoneUV);
	vec4 currentNoiseColor = texture(Noise, zoneUV.xy);

	if (currentSnowColor.r > 0.0f) {
		float compressedSnowValue = currentSnowColor.a;

		vertex.y += ((currentSnowColor.r * SnowFactor) * compressedSnowValue) + (currentNoiseColor.r * NoiseFactor) - SnowInnerOffset;
		
		if (currentSnowColor.a < 1.0) {
			vertex.x += currentSnowColor.g;
			vertex.z += currentSnowColor.b;		
		}

		color = vec4(currentSnowColor.r, currentSnowColor.g, currentSnowColor.b, 1.0);
		
		const vec3 off = vec3(1.0, 1.0, 0.0);
		float hL = calculateVertexHeight(color, vertex, worldVertex, -off.xz);
		float hR = calculateVertexHeight(color, vertex, worldVertex, off.xz);
		float hB = calculateVertexHeight(color, vertex, worldVertex, -off.zy);
		float hF = calculateVertexHeight(color, vertex, worldVertex, off.zy);
		normal = normalize(vec3(hL - hR, 2.0, hB - hF));		
	} else {
		vertex.y -= 1.0;
		
		color = vec4(0.0f, 0.0f, 0.0f, 0.0);
	}
}

void calculateSnowFragment(
	vec4 color, 
	vec3 worldVertex,	
	inout vec3 albedo,
	inout vec3 normalMap,
	inout float metallic,
	inout float roughness,
	inout float alpha		
) {
	if (color[0] > 0.0f) {
		vec3 zoneUV = calculateZoneUV(worldVertex);			
		vec2 worldVertexUV = worldVertex.xz * (float(SnowColorDetail) * (1.0 / ZonesSize));
				
		albedo = texture(SnowColorTexture, worldVertexUV).rgb;
		normalMap = texture(SnowColorNormal, worldVertexUV).rgb;
		metallic = Metallic;
		roughness = texture(SnowColorRoughness, worldVertexUV).r;
		
		if (zoneUV.z < 0.0) {
			alpha = 0.0;
		} else {
			float hole = texture(HeightmapTextures, zoneUV).g;
			if (hole > 0.0) {
				alpha = 0.0;
			}			
		}
	} else {
		alpha = 0.0;
	}
}