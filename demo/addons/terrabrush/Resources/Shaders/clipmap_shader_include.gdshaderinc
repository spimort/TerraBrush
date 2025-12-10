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

		if (previousZoneUV.z >= 0.0 && nextZoneUV.z >= 0.0) {
			float previousHeight = texture(textures, previousZoneUV).r;
			float nextHeight = texture(textures, nextZoneUV).r;

			return (previousHeight + nextHeight) / 2.0;
		}
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

void calculateVertex(mat4 modelMatrix, vec4 levelsMeta, inout vec3 vertex, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz - OffsetPosition;

	vertex.y = calculateVertexHeight(levelsMeta, vertex, worldVertex, vec2(0.0));
}
