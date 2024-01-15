uniform sampler2D HeightmapTexture : hint_default_transparent, repeat_disable;
uniform float InitialCellWidth = 1.0;

void calculateVertex(mat4 modelMatrix, vec4 zonesMeta, inout vec3 vertex, out vec2 textureUV, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz;
	float heightmapSize = float(textureSize(HeightmapTexture, 0).x);
	textureUV = (worldVertex.xz + (heightmapSize / 2.0)) / heightmapSize;

	vertex.y = texture(HeightmapTexture, textureUV).r;
	
	// Calculate the average height for mid zone vertices
	if (zonesMeta.g > 0.0 || zonesMeta.b > 0.0) {
		int level = int(zonesMeta.a * 100.0);
		float levelMultiplicator = pow(2.0, float(level));
		vec2 prevNextUVVector = ((vec2(zonesMeta.g, zonesMeta.b) / heightmapSize) * levelMultiplicator) * InitialCellWidth;
		
		float previousHeight = texture(HeightmapTexture, textureUV - prevNextUVVector).r;
		float nextHeight = texture(HeightmapTexture, textureUV + prevNextUVVector).r;
		
		vertex.y = (previousHeight + nextHeight) / 2.0;
	}	
}