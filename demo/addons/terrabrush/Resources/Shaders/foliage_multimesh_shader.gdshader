shader_type spatial;
render_mode blend_mix, depth_draw_opaque, cull_disabled, diffuse_burley, specular_schlick_ggx, alpha_to_coverage;

#include "res://addons/terrabrush/Resources/Shaders/foliage_multimesh_shader_include.gdshaderinc"

varying flat vec3 _instancePosition;

void vertex() {
	_instancePosition = vec3(0);
	calculateFoliageVertex(
		INSTANCE_ID,
		VERTEX,
		NORMAL,
		COLOR,
		_instancePosition
	);
}

void fragment() {
	calculateFoliageFragment(
		UV,
		COLOR,
		_instancePosition,
		ALBEDO,
		ALPHA, 
		ALPHA_SCISSOR_THRESHOLD,
		ALPHA_ANTIALIASING_EDGE
	);
}
