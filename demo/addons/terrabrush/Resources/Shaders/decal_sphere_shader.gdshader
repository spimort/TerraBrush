// This shader has been adapted from https://godotshaders.com/shader/shield-shader-with-intersection-highlight/
shader_type spatial;
render_mode unshaded, cull_disabled;

uniform sampler2D depth_texture : source_color, hint_depth_texture;
uniform vec3 Color: source_color = vec3(1,0,0);
uniform float ColorAlpha = 0.01;
uniform float Threshold = 1.0;
uniform float FresnelSharpness = 5.0;
uniform bool Compatibility = true;

void fragment() {
	float depth = texture(depth_texture, SCREEN_UV).x;
	vec3 ndc = vec3(0);
	if (Compatibility) {
		ndc = vec3(SCREEN_UV, depth) * 2.0 - 1.0;		
	} else {
		ndc = vec3(SCREEN_UV * 2.0 - 1.0, depth);		
	}
	vec4 view = INV_PROJECTION_MATRIX * vec4(ndc, 1.0);
	view.xyz /= view.w;
	float linear_depth = -view.z;
	
	float object_depth = FRAGCOORD.z;
	vec3 object_ndc = vec3(0);
	if (Compatibility) {
		object_ndc = vec3(SCREEN_UV, object_depth) * 2.0 - 1.0;		
	} else {
		object_ndc = vec3(SCREEN_UV * 2.0 - 1.0, object_depth);		
	}
	vec4 object_view = INV_PROJECTION_MATRIX * vec4(object_ndc, 1.0);
	object_view.xyz /= object_view.w;
	float linear_object_depth = -object_view.z;
	
	float fresnel = pow(1.0 - dot(VIEW, NORMAL), FresnelSharpness);
	
	if (linear_depth - linear_object_depth > Threshold) {
		ALBEDO = Color.rgb;
		ALPHA = clamp(ColorAlpha+fresnel, 0.0, 1.0);
	} else {
		ALBEDO = Color;
		ALPHA = clamp(0.5+fresnel, 0.0, 1.0);
	}
}