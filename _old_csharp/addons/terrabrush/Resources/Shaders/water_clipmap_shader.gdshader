shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix;

#include "res://addons/terrabrush/Resources/Shaders/water_clipmap_shader_include.gdshaderinc"

uniform sampler2D DEPTH_TEXTURE : hint_depth_texture, filter_linear_mipmap;
uniform sampler2D SCREEN_TEXTURE : hint_screen_texture, filter_linear_mipmap;

varying vec3 _worldVertex;

void vertex() {
	_worldVertex = vec3(0);
	calculateWaterVertex(MODEL_MATRIX, COLOR, VERTEX, _worldVertex);
}

void fragment() {
	calculateWaterFragment(
		DEPTH_TEXTURE,
		SCREEN_TEXTURE,
		SCREEN_UV,
		PROJECTION_MATRIX,
		COLOR,
		VERTEX,
		FRAGCOORD,
		VIEW,
		_worldVertex, 
		NORMAL,
		ALBEDO,
		NORMAL_MAP,
		METALLIC,
		ROUGHNESS,
		ALPHA
	);
}
