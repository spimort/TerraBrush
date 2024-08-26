shader_type spatial;
render_mode cull_back,blend_mix,depth_draw_opaque,diffuse_burley,specular_schlick_ggx;

#include "res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader_include.gdshaderinc"

varying vec3 _worldVertex;

void vertex() {
	_worldVertex = vec3(0);
	calculateHeightmapVertex(MODEL_MATRIX, COLOR, VERTEX, NORMAL, _worldVertex);
}

void fragment() {
	calculateHeightmapFragment(
		_worldVertex,
		ALBEDO,
		NORMAL_MAP,
		ROUGHNESS,
		ALPHA,
		ALPHA_SCISSOR_THRESHOLD
	);
}