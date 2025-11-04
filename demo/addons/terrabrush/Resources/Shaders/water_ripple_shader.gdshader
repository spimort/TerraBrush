shader_type canvas_item;

uniform sampler2D DoubleBufferTexture;
uniform sampler2D CollisionTexture: hint_default_black;

uniform float Phase = 0.18;
uniform float Attenuation = 0.995;
uniform float DeltaUV = 2.3;
uniform float WaterRippleTextureSize = 512;

void fragment() {
	float dv = 1.0 / WaterRippleTextureSize;
	float du = 1.0 / WaterRippleTextureSize;
	vec3 duv = vec3(du, dv, 0) * DeltaUV;

	vec3 doubleBuffer = texture(DoubleBufferTexture, UV).rgb;

	float previous = (2.0 * doubleBuffer.r - doubleBuffer.g + Phase * (
		texture(DoubleBufferTexture, UV - duv.zy).r +
		texture(DoubleBufferTexture, UV + duv.zy).r +
		texture(DoubleBufferTexture, UV - duv.xz).r +
		texture(DoubleBufferTexture, UV + duv.xz).r - 4.0 * doubleBuffer.r)) * Attenuation;
		
	vec2 colisionUV = UV;
	float collision = texture(CollisionTexture, colisionUV).r;
	float prevCollision = texture(DoubleBufferTexture, UV).b;
	
	if (collision > 0.0 && prevCollision == 0.0) {
		previous += collision * 0.5;
	}
	
	if (prevCollision > 0.0 && collision == 0.0) {
		previous -= prevCollision * 0.5;
	}
		
	COLOR = vec4(previous, doubleBuffer.r, collision, 1);
}