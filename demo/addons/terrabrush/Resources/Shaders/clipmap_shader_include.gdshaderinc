uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float InitialCellWidth = 1.0;
uniform vec3 OffsetPosition;

#include "zones_shader_include.gdshaderinc"

float calculateMidZoneHeightValue(sampler2DArray textures, vec4 levelsMeta, vec3 worldVertex) {
	// Calculate the average height for mid zone vertices
	if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
		int level = int(levelsMeta.a * 100.0);
		float levelMultiplicator = exp2(float(level));
		vec2 neighbourWorldVertex = (vec2(levelsMeta.g, levelsMeta.b) * levelMultiplicator) * InitialCellWidth;
		vec3 previousZoneUV = calculateZoneUV(worldVertex - vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));
		vec3 nextZoneUV = calculateZoneUV(worldVertex + vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));

		float previousHeight = 0.0; 
		float nextHeight = 0.0; 
		if (previousZoneUV.z >= 0.0) {
			previousHeight = texture(textures, previousZoneUV).r;
		} 
		if (nextZoneUV.z >= 0.0) {
			nextHeight = texture(textures, nextZoneUV).r;
		}
		
		return (previousHeight + nextHeight) / 2.0;
	}

	return 0.0;
}

float calculateVertexHeight(vec4 levelsMeta, vec3 vertex, vec3 worldVertex, vec2 offset) {
	vec3 offsetWorldVertex = worldVertex + vec3(offset.x, 0.0, offset.y);
	ivec2 zonesMapPosition = ivec2(0);
	vec3 zoneUV = calculateZoneUVWithZoneInfo(offsetWorldVertex, true, zonesMapPosition);
	float height = vertex.y;

	if (zoneUV.z >= 0.0) {
		// Calculate the average height for mid zone vertices
		if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
			height = calculateMidZoneHeightValue(HeightmapTextures, levelsMeta, offsetWorldVertex);
		} else {
			height = texture(HeightmapTextures, zoneUV).r;
		}
	} else {
		// Try to match the height of neighbors zones for a specific range so the transition to go hidden is easier
		float includingRange = (1.0 / ZonesSize) * 10.0;

		if (zoneUV.x <= includingRange || zoneUV.y <= includingRange || zoneUV.x >= 1.0 - includingRange || zoneUV.y >= 1.0 - includingRange) {
			ivec2 zonesMapSize = textureSize(ZonesMap, 0);

			// Check for the left edge if there is a zone on the right
			ivec2 neighborZonePosition = zonesMapPosition + ivec2(1, 0);
			if (zoneUV.x >= 1.0 - includingRange && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
				float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
				if (neighborZone >= 0.0) {
					vec4 neighborHeight = texture(HeightmapTextures, vec3(0.0, zoneUV.y, neighborZone));
					height = neighborHeight.r;
				}
			}

			// Check for the top edge if there is a zone on the bottom
			neighborZonePosition = zonesMapPosition + ivec2(0, 1);
			if (zoneUV.y >= 1.0 - includingRange && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
				float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
				if (neighborZone >= 0.0) {
					vec4 neighborHeight = texture(HeightmapTextures, vec3(zoneUV.x, 0.0, neighborZone));
					height = neighborHeight.r;
				}
			}

			// Check for the right edge if there is a zone on the left
			neighborZonePosition = zonesMapPosition + ivec2(-1, 0);
			if (zoneUV.x <= includingRange && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
				float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
				if (neighborZone >= 0.0) {
					vec4 neighborHeight = texture(HeightmapTextures, vec3(1.0, zoneUV.y, neighborZone));
					height = neighborHeight.r;
				}
			}

			// Check for the bottom edge if there is a zone on the top
			neighborZonePosition = zonesMapPosition + ivec2(0, -1);
			if (zoneUV.y <= includingRange && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
				float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
				if (neighborZone >= 0.0) {
					vec4 neighborHeight = texture(HeightmapTextures, vec3(zoneUV.x, 1.0, neighborZone));
					height = neighborHeight.r;
				}
			}
		}
	}

	return height;
}

void calculateVertex(mat4 modelMatrix, vec4 custom0, inout vec4 levelsMeta, inout vec3 vertex, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz - OffsetPosition;
	
	int level = int(levelsMeta.a * 100.0);
	float levelMultiplicator = exp2(float(level)) * InitialCellWidth;	
	float nextLevelMultiplicator = exp2(float(level + 1)) * InitialCellWidth;	
	
	float xOffset = levelMultiplicator - mod(worldVertex.x, levelMultiplicator);
	float zOffset = levelMultiplicator - mod(worldVertex.z, levelMultiplicator);
		
	vertex.x += xOffset;
	vertex.z += zOffset;
	
	worldVertex.x += xOffset;
	worldVertex.z += zOffset;
	
	vertex.y = calculateVertexHeight(levelsMeta, vertex, worldVertex, vec2(0.0));
	
	if (custom0.r > 0.0 || custom0.g > 0.0) {
		// Anchor the world vertex to the corners, so we can check if the whole cell need to vanish
		vec2 worldVertexAnchorPosition = vec2(worldVertex.x + (custom0.b * levelMultiplicator), worldVertex.z + (custom0.a * levelMultiplicator));
		if (custom0.r > 0.0 && mod(worldVertexAnchorPosition.y, nextLevelMultiplicator) != 0.0) {
			levelsMeta.r = -1.0;
		}
		
		if (custom0.g > 0.0 && mod(worldVertexAnchorPosition.x, nextLevelMultiplicator) != 0.0) {
			levelsMeta.r = -1.0;
		}		
	}	
	
	if (mod(worldVertex.x, nextLevelMultiplicator) != 0.0) {
		//xOffset *= -1.0;
		//vertex.y += 1.0;
	}
	

 	//if (levelsMeta.r > 0.25 && levelsMeta.r <= 0.5 && mod(worldVertex.x, nextLevelMultiplicator) != 0.0) {
		//vertex.y += 1.0;
		////levelsMeta.r = -1.0;
	//}
	//
 	//if (levelsMeta.r > 0.5 && mod(worldVertex.z, nextLevelMultiplicator) != 0.0) {
		//vertex.y += 1.0;
		////levelsMeta.r = -1.0;
	//}	
	//
 	//if (levelsMeta.r > 0.0 && levelsMeta.r <= 0.25 && (mod(worldVertex.x, nextLevelMultiplicator) != 0.0 || mod(worldVertex.z, nextLevelMultiplicator) != 0.0)) {
		//vertex.y += 1.0;
		////levelsMeta.r = -1.0;
	//}	
	
	//if (custom0.r > 0.0 && mod(worldVertex.x, nextLevelMultiplicator) != 0.0) {
		//vertex.y += 1.0;
	//}
	
	
	
	//if (custom0.r > 0.0) {
		//float checkHideEdgeVertexPosition = worldVertex.x;
		//if (custom0.r > 1.0) {
			//checkHideEdgeVertexPosition += levelMultiplicator;
		//}
		//if (mod(checkHideEdgeVertexPosition, nextLevelMultiplicator) != 0.0) {
			//vertex.y += 1.0;			
		//}
	//}	
	//
	//if (custom0.g > 0.0) {
		//float checkHideEdgeVertexPosition = worldVertex.z;
		//if (custom0.g > 1.0) {
			//checkHideEdgeVertexPosition += levelMultiplicator;
		//}
		//if (mod(checkHideEdgeVertexPosition, nextLevelMultiplicator) != 0.0) {
			//vertex.y += 1.0;			
		//}
	//}		
	

}
