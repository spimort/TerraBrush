uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float InitialCellWidth = 1.0;
uniform vec3 OffsetPosition;

#include "zones_shader_include.gdshaderinc"

float calculateMidZoneHeightValue(sampler2DArray textures, vec4 levelsMeta, vec3 worldVertex, vec3 zoneUV) {
	// Calculate the average height for mid zone vertices
	if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
		// Since we offset when we are on even zone size, we need to "deoffset" it to calculate the midzone
		float evenOffset = 0.0;
		if (mod(ZonesSize, 2.0) == 0.0) {
			evenOffset = InitialCellWidth / 2.0;
		}			
		
		int level = int(levelsMeta.a * 100.0);
		float levelMultiplicator = exp2(float(level));

		// We also grab the next level cell width to check if we overlap that level, to add some rows/columns
		vec2 neighbourMeta = vec2(0, 0);
		float nextLevelCellWidth = exp2(float(level + 1)) * InitialCellWidth;	
		
		// Check if we are between two of the next level vertices, so we can make the average of both (-1, 1) vertices
		// Also check that on the other axis, we are on the row/column that is visible
		// Same thing for the other axis
		if (levelsMeta.g > 0.0 && mod(worldVertex.x - evenOffset, nextLevelCellWidth) != 0.0 && mod(worldVertex.z - evenOffset, nextLevelCellWidth) == 0.0) {
			neighbourMeta.x = 1.0;
		}
		
		if (levelsMeta.b > 0.0 && mod(worldVertex.z - evenOffset, nextLevelCellWidth) != 0.0 && mod(worldVertex.x - evenOffset, nextLevelCellWidth) == 0.0) {
			neighbourMeta.y = 1.0;
		}		
		
		// If wee need to calculate next level average zone height (-1, 1), then we calcaulate it
		if (neighbourMeta.x > 0.0 || neighbourMeta.y > 0.0) {
			vec2 neighbourWorldVertex = (neighbourMeta * levelMultiplicator) * InitialCellWidth;
			vec3 previousZoneUV = calculateZoneUV(worldVertex - vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));
			vec3 nextZoneUV = calculateZoneUV(worldVertex + vec3(neighbourWorldVertex.x, 0.0, neighbourWorldVertex.y));
			
			if (previousZoneUV.z >= 0.0 && nextZoneUV.z >= 0.0) {
				float previousHeight = texture(textures, previousZoneUV).r; 
				float nextHeight = texture(textures, nextZoneUV).r; 
				
				return (previousHeight + nextHeight) / 2.0;							
			}
		}
	}

	return texture(textures, zoneUV).r;
}

float calculateVertexHeight(vec4 levelsMeta, vec3 vertex, vec3 worldVertex, vec2 offset) {
	vec3 offsetWorldVertex = worldVertex + vec3(offset.x, 0.0, offset.y);
	ivec2 zonesMapPosition = ivec2(0);
	vec3 zoneUV = calculateZoneUVWithZoneInfo(offsetWorldVertex, true, zonesMapPosition);
	float height = vertex.y;

	if (zoneUV.z >= 0.0) {
		// Calculate the average height for mid zone vertices
		if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
			height = calculateMidZoneHeightValue(HeightmapTextures, levelsMeta, offsetWorldVertex, zoneUV);
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
	
	// We get the level here to get the cell size we are at right now
	// The level is computed from the Color channel in the vertice, fromt he alpha channel	
	int level = int(levelsMeta.a * 100.0);
	float levelCellWidth = exp2(float(level)) * InitialCellWidth;	
	
	// We also grab the next level cell width to check if we overlap that level, to add some rows/columns
	float nextLevelCellWidth = exp2(float(level + 1)) * InitialCellWidth;	
		
	// We snap the vertices to the cell width (at this point the vertices does not need to be anchored)
	float xOffset = levelCellWidth - mod(worldVertex.x, levelCellWidth);
	float zOffset = levelCellWidth - mod(worldVertex.z, levelCellWidth);	
	
	vertex.x += xOffset;
	vertex.z += zOffset;
	
	// We also move the worldVertex to make sure the calculateVertexHeight gets the height of the new position
	worldVertex.x += xOffset;
	worldVertex.z += zOffset;
	
	// Here is the part where we check if we need to hide the extra rows/columns
	// If the horizontal or the vertical flag (1.0) is on, then we are on a edge
	if (custom0.r > 0.0 || custom0.g > 0.0) {
		// Anchor the world vertex to the corners, so we can check if the whole cell need to vanish
		vec2 worldVertexAnchorPosition = vec2(worldVertex.x + (custom0.b * levelCellWidth), worldVertex.z + (custom0.a * levelCellWidth));
		
		// Check against the next levelCellWidth to check if we need to hide something, according to the anchor of the cell
		if (custom0.r > 0.0 && mod(worldVertexAnchorPosition.y, nextLevelCellWidth) != 0.0) {
			levelsMeta.r = -1.0;
		}
		
		if (custom0.g > 0.0 && mod(worldVertexAnchorPosition.x, nextLevelCellWidth) != 0.0) {
			levelsMeta.r = -1.0;
		}
	}

	// If we are on an even zoneSize, we need to offset each cells
	if (mod(ZonesSize, 2.0) == 0.0) {
		float evenOffset = InitialCellWidth / 2.0;
		vertex.x += evenOffset; 
		vertex.z += evenOffset;
		
		worldVertex.x += evenOffset;
		worldVertex.z += evenOffset;		
	}

	vertex.y = calculateVertexHeight(levelsMeta, vertex, worldVertex, vec2(0.0));			
}
