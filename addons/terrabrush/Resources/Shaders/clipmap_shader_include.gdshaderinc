uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float InitialCellWidth = 1.0;

#include "zones_shader_include.gdshaderinc"

float calculateVertexHeight(mat4 modelMatrix, vec4 levelsMeta, vec3 vertex, vec2 offset) {
	vec3 worldVertex = vertex + modelMatrix[3].xyz + vec3(offset.x, 0.0, offset.y);
	
	vec3 zoneUV = calculateZoneUV(worldVertex);
	
	float height = vertex.y;
	
	if (zoneUV.z >= 0.0) {
		// Calculate the average height for mid zone vertices
		if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
			int level = int(levelsMeta.a * 100.0);
			float levelMultiplicator = pow(2.0, float(level));
			vec2 neighbourWorldVertex = (vec2(levelsMeta.g, levelsMeta.b) * levelMultiplicator) * InitialCellWidth;
			vec3 previousZoneUV = calculateZoneUV(worldVertex - vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));
			vec3 nextZoneUV = calculateZoneUV(worldVertex + vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));

			float previousHeight = texture(HeightmapTextures, previousZoneUV).r;
			float nextHeight = texture(HeightmapTextures, nextZoneUV).r;
			
			height = (previousHeight + nextHeight) / 2.0;
		} else {
			height = texture(HeightmapTextures, zoneUV).r;
		}
	}
	
	return height;
}

void calculateVertex(mat4 modelMatrix, vec4 levelsMeta, inout vec3 vertex, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz;
	
	vertex.y = calculateVertexHeight(modelMatrix, levelsMeta, vertex, vec2(0.0));
}
