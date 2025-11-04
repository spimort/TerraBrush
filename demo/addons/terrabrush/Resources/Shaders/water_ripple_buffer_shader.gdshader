shader_type canvas_item;

uniform sampler2D RippleTexture;

void fragment() {
	COLOR.rgb = texture(RippleTexture, UV).rgb;
}