shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix;

#include "clipmap_shader_include.gdshaderinc"

uniform sampler2D DEPTH_TEXTURE : hint_depth_texture, filter_linear_mipmap;
uniform sampler2D SCREEN_TEXTURE : hint_screen_texture, filter_linear_mipmap;

uniform sampler2DArray WaterTextures : hint_default_transparent, repeat_disable;
uniform float WaterFactor;
uniform float WaterInnerOffset = 0.3;

// Water uniforms
uniform vec3 WaterColor: source_color;
uniform vec3 FresnelColor: source_color;
uniform float Metallic: hint_range(0.0, 1.0, 0.001); 
uniform float Roughness: hint_range(0.0, 1.0, 0.001); 
uniform sampler2D NormalMap;
uniform sampler2D NormalMap2;
uniform float TimeScale: hint_range(0.0, 0.2, 0.005) = 0.025;
uniform float Strength = 0.4;
uniform sampler2D Wave;
uniform float NoiseScale = 10.0;
uniform float HeightScale = 0.15;
uniform vec4 ColorDeep: source_color;
uniform vec4 ColorShallow: source_color;
uniform float BeersLaw =  2.0;
uniform float DepthOffset = -0.75;
uniform float EdgeScale = 0.1;
uniform float Near = 0.5;
uniform float Far = 100.0;
uniform vec3 EdgeColor: source_color;

varying vec3 _worldVertex;

float fresnel(float amount, vec3 normal, vec3 view) {
	return pow((1.0 - clamp(dot(normalize(normal), normalize(view)), 0.0, 1.0)), amount);
}

float edge(float depth) {
	depth = 2.0 * depth - 1.0;
	return Near * Far / (Far + depth * (Near - Far));
}

void vertex() {
	_worldVertex = vec3(0);
	calculateVertex(MODEL_MATRIX, COLOR, VERTEX, _worldVertex);
	
	vec3 zoneUV = calculateZoneUV(_worldVertex);		
	vec4 currentWaterColor = texture(WaterTextures, zoneUV);

	if (currentWaterColor.r > 0.0f) {
		VERTEX.y -= WaterInnerOffset;
		
		float height = texture(Wave, _worldVertex.xz / NoiseScale + TIME * TimeScale).r;
		VERTEX.y += height * HeightScale;
		
		float rippleValue = currentWaterColor.a;
		VERTEX.y += rippleValue * 0.1;

		COLOR = vec4(currentWaterColor.r, currentWaterColor.g, currentWaterColor.b, 1.0);
	} else {
		VERTEX.y -= 1.0;
		COLOR = vec4(0.0f, 0.0f, 0.0f, 0.0);
	}
}

void fragment() {
	if (COLOR[0] > 0.0f) {
		float depthTexture = texture(DEPTH_TEXTURE, SCREEN_UV).r * 2.0 - 1.0;
		float depth = PROJECTION_MATRIX[3][2] / (depthTexture + PROJECTION_MATRIX[2][2]);
		float depthBlend = exp((depth + VERTEX.z + DepthOffset) * -BeersLaw);
		depthBlend = clamp(1.0 - depthBlend, 0.0, 1.0);
		float depthBlendPower = clamp(pow(depthBlend, 2.5), 0.0, 1.0);
		
		vec3 screenColor = textureLod(SCREEN_TEXTURE, SCREEN_UV, depthBlendPower * 2.5).rgb;
		vec3 depthColor = mix(ColorShallow.rgb, ColorDeep.rgb, depthBlendPower);
		vec3 color = mix(screenColor * depthColor, depthColor * 0.25, depthBlendPower * 0.5);
		
		float zDepth = edge(texture(DEPTH_TEXTURE, SCREEN_UV).x);
		float zPos = edge(FRAGCOORD.z);
		float zDif = zDepth - zPos;
		
		float time = TimeScale * TIME;
		float timer = fract(time);
		float timer2 = fract(time + 0.5);
		float mixTimer = min(fract(time), 1.0 - fract(time)) * 2.0;

		vec3 zoneUV = calculateZoneUV(_worldVertex);		
		vec4 waterTexture = texture(WaterTextures, zoneUV);
		vec2 flows = waterTexture.gb * 2.0 - 1.0;

		vec3 normalMap1 = texture(NormalMap, _worldVertex.xz + flows * timer * Strength).rgb;
		vec3 normalMap2 = texture(NormalMap2, _worldVertex.xz + flows * timer2 * Strength).rgb;
		vec3 normalBlend = mix(normalMap2, normalMap1, mixTimer);
		
		float fresnel = fresnel(5.0, NORMAL, VIEW);
		vec3 surfaceColor = mix(WaterColor.rgb, FresnelColor.rgb, fresnel);

		vec3 depthColorAdj = mix(EdgeColor, color, step(EdgeScale, zDif));
		
		ALBEDO = clamp(surfaceColor + depthColorAdj,vec3(0.0),vec3(1.0));
		METALLIC = Metallic;
		ROUGHNESS = Roughness;
		NORMAL_MAP = normalBlend;	
		
		if (waterTexture.a < 0.5) {
			NORMAL = NORMAL + (waterTexture.a * 10.0);
		}
		
		if (zoneUV.z < 0.0) {
			ALPHA = 0.0;
		}			
	} else {
		ALPHA = 0.0;
	}
}
