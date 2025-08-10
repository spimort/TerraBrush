shader_type spatial;
render_mode blend_mix, depth_draw_opaque, cull_disabled, diffuse_lambert, specular_schlick_ggx;

#include "clipmap_shader_include.gdshaderinc"

void vertex() {
	vec3 worldVertex;
	calculateVertex(MODEL_MATRIX, COLOR, VERTEX, worldVertex);
}