shader_type spatial;
render_mode cull_back,diffuse_burley,specular_schlick_ggx, blend_mix;

uniform sampler2D HeightmapTexture : hint_default_transparent, repeat_disable;
uniform float InitialCellWidth = 1.0;

varying vec2 _textureUV;

void vertex() {
	vec3 worldVertex = VERTEX + MODEL_MATRIX[3].xyz;
	float heightmapSize = float(textureSize(HeightmapTexture, 0).x);
	_textureUV = (worldVertex.xz + (heightmapSize / 2.0)) / heightmapSize;

	VERTEX.y = texture(HeightmapTexture, _textureUV).r;
	
	// Calculate the average height for mid zone vertices
	if (COLOR.g > 0.0 || COLOR.b > 0.0) {
		int level = int(COLOR.a * 100.0);
		float levelMultiplicator = pow(2.0, float(level));
		vec2 prevNextUVVector = ((vec2(COLOR.g, COLOR.b) / heightmapSize) * levelMultiplicator) * InitialCellWidth;
		
		float previousHeight = texture(HeightmapTexture, _textureUV - prevNextUVVector).r;
		float nextHeight = texture(HeightmapTexture, _textureUV + prevNextUVVector).r;
		
		VERTEX.y = (previousHeight + nextHeight) / 2.0;
	}
	
	// "Hide" vertices that are outside the map
	float halfHeightmapSize = float(textureSize(HeightmapTexture, 0).x) / 2.0;	
	if (
		worldVertex.x > halfHeightmapSize || 
		worldVertex.x < -halfHeightmapSize || 
		worldVertex.z > halfHeightmapSize ||
		worldVertex.z < -halfHeightmapSize
	) {
		VERTEX.y = -10000000.0;
	}	
}
