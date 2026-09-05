shader_type spatial;
render_mode cull_back,blend_mix,depth_draw_opaque,diffuse_burley,specular_schlick_ggx;

#include "res://addons/terrabrush/Resources/Shaders/heightmap_clipmap_shader_include.gdshaderinc"

varying vec3 _worldVertex;
varying vec3 _worldNormal;
varying vec3 _triplanarPos;
varying vec3 _triplanarPowerNormal;

void vertex() {
	_worldVertex = vec3(0);
	_worldNormal = vec3(0);
	_triplanarPos = vec3(0);
	_triplanarPowerNormal = vec3(0);
	calculateHeightmapVertex(
		MODEL_MATRIX,
		CUSTOM0,
		COLOR,
		VERTEX,
		NORMAL,		
		_worldVertex,
		_worldNormal,
		_triplanarPos,
		_triplanarPowerNormal
	);	
}

void fragment() {
	calculateHeightmapFragment(
		_worldVertex,
		_worldNormal,
		_triplanarPos,
		_triplanarPowerNormal,
		COLOR,
		ALBEDO,
		NORMAL_MAP,
		METALLIC,
		SPECULAR,
		ROUGHNESS
	);
	
 	//if (COLOR.r > 0.0 && COLOR.r < 0.5) {
		//ALBEDO = vec3(1, 0, 1);
 	//} else if (COLOR.r > 0.0 && COLOR.r <= 0.5) {
		//ALBEDO = vec3(0, 1, 0);
	//} else 	if (COLOR.r > 0.5) {
		//ALBEDO = vec3(0, 0, 1);
	//}	
	//
	//if (COLOR.g > 0.0 || COLOR.b > 0.0) {
		//ALBEDO = vec3(1, 0, 0);
	//}	
	
	//if (COLOR.g > 0.0) {
		//ALBEDO = vec3(1, 0, 0);
	//}
	//
	//if (COLOR.b > 0.0) {
		//ALBEDO = vec3(0, 1, 0);
	//}	
}