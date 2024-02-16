uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float InitialCellWidth = 1.0;

#include "zones_shader_include.gdshaderinc"

void calculateVertex(mat4 modelMatrix, vec4 levelsMeta, inout vec3 vertex, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz;
	
	vec3 zoneUV = calculateZoneUV(worldVertex);		
	if (zoneUV.z >= 0.0) {		
		vertex.y = texture(HeightmapTextures, zoneUV).r;
		
		// Calculate the average height for mid zone vertices
		if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
			int level = int(levelsMeta.a * 100.0);
			float levelMultiplicator = pow(2.0, float(level));
			vec2 neighbourWorldVertex = (vec2(levelsMeta.g, levelsMeta.b) * levelMultiplicator) * InitialCellWidth;
			vec3 previousZoneUV = calculateZoneUV(worldVertex - vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));
			vec3 nextZoneUV = calculateZoneUV(worldVertex + vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));

			float previousHeight = texture(HeightmapTextures, previousZoneUV).r;
			float nextHeight = texture(HeightmapTextures, nextZoneUV).r;
			
			vertex.y = (previousHeight + nextHeight) / 2.0;
		}		
	}				
}