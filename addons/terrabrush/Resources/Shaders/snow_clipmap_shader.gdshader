shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix, depth_prepass_alpha;

#include "res://addons/terrabrush/Resources/Shaders/snow_clipmap_shader_include.gdshaderinc"

varying vec3 _worldVertex;

void vertex() {
	_worldVertex = vec3(0);
	calculateSnowVertex(
		MODEL_MATRIX, 
		COLOR, 
		VERTEX, 
		NORMAL,
		_worldVertex
	);
}

void fragment() {
	calculateSnowFragment(
		COLOR,
		_worldVertex,
		ALBEDO,
		NORMAL_MAP,
		METALLIC,
		ROUGHNESS,
		ALPHA	
	);
}