shader_type spatial;
render_mode cull_back,blend_mix,depth_draw_opaque,diffuse_burley,specular_schlick_ggx;

#include "res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader_include.gdshaderinc"

varying vec3 _worldVertex;
varying vec3 _triplanarPos;
varying vec3 _triplanarPowerNormal;

void vertex() {
	_worldVertex = vec3(0);
	_triplanarPos = vec3(0);
	_triplanarPowerNormal = vec3(0);
	calculateHeightmapVertex(
		MODEL_MATRIX, 
		COLOR, 
		VERTEX, 
		NORMAL, 
		_worldVertex, 
		_triplanarPos, 
		_triplanarPowerNormal
	);
}

void fragment() {
	calculateHeightmapFragment(
		_worldVertex,
		_triplanarPos,
		_triplanarPowerNormal,
		ALBEDO,
		NORMAL_MAP,
		ROUGHNESS,
		ALPHA,
		ALPHA_SCISSOR_THRESHOLD
	);
}