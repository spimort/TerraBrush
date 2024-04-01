shader_type canvas_item;

uniform float CircleSize : hint_range(0.0, 1.0) = 0.5;

void fragment() {
	float dist = distance(vec2(0.5, 0.5), vec2(mix(0.5, UV.x, 1.0), UV.y));
	vec4 currentColor = texture(TEXTURE, UV);
	COLOR.rgb = currentColor.rgb;
	COLOR.a = (1.0 - step(CircleSize, dist)) * currentColor.a;
}
