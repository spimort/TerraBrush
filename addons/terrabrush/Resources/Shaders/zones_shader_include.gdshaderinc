uniform float ZonesSize;
uniform sampler2D ZonesMap;
uniform float NumberOfZones;

bool isZoneInZonesMap(ivec2 zonePosition, ivec2 zonesMapSize) {
	return zonePosition.x >= 0 && zonePosition.x < zonesMapSize.x && zonePosition.y >= 0 && zonePosition.y < zonesMapSize.y;
}

vec3 calculateZoneUVWithZoneInfo(vec3 worldVertex, out ivec2 zonesMapPosition) {
	float fixedZonesSize = (ZonesSize - 1.0); 	
	vec2 zoneArea = (worldVertex.xz + (fixedZonesSize / 2.0)) / fixedZonesSize;
	
	ivec2 zonesMapSize = textureSize(ZonesMap, 0);

	ivec2 zonesStep = ivec2(int(floor(zoneArea.x)), int(floor(zoneArea.y)));
	int xZonesMap = zonesStep.x + int(zonesMapSize.x) / 2;
	int yZonesMap = zonesStep.y + int(zonesMapSize.y) / 2;
	
	vec4 zoneInfoPosition = vec4(-1);
	zonesMapPosition = ivec2(xZonesMap, yZonesMap);
	if (xZonesMap >= 0 && xZonesMap < zonesMapSize.x && yZonesMap >= 0 && yZonesMap < zonesMapSize.y) {
		zoneInfoPosition = texelFetch(ZonesMap, zonesMapPosition, 0);
	}	

	float sizeUnit = (1.0 / ZonesSize);
	float halfUnit = sizeUnit / 2.0;
	float fixedSizePercent = 1.0 - sizeUnit;
	
	float zoneX = (((zoneArea.x - (float(zonesStep.x))) * fixedSizePercent) + halfUnit);
	float zoneY = (((zoneArea.y - (float(zonesStep.y))) * fixedSizePercent) + halfUnit);
	
	// If the zoneInformation.r >= 0, it means that a zone has been created for this position.
	if (zoneInfoPosition.r >= 0.0) {
		return vec3(zoneX, zoneY, floor(zoneInfoPosition.r));	
	}		
	
	// There is a edge case when there is no zone for a position, the neighbor zone does not
	// cover the first pixel row/column (otherwise both images would overlap).
	// To handle that, we take the previous zone max position and we use it for our row/column.
	if (zoneX <= halfUnit || zoneY <= halfUnit) {
		ivec2 neighborZonePosition = zonesMapPosition - ivec2(1, 0);
		if (zoneX <= halfUnit && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
			float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
			if (neighborZone >= 0.0) {
				return vec3(1.0, zoneY, neighborZone);				
			}
		}	
		
		neighborZonePosition = zonesMapPosition - ivec2(0, 1);
		if (zoneY <= halfUnit && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
			float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
			if (neighborZone >= 0.0) {
				return vec3(zoneX, 1.0, neighborZone);				
			}
		}		
		
		neighborZonePosition = zonesMapPosition - ivec2(1, 1);
		if (zoneX <= halfUnit && zoneY <= halfUnit && isZoneInZonesMap(neighborZonePosition, zonesMapSize)) {
			float neighborZone = texelFetch(ZonesMap, neighborZonePosition, 0).r;
			if (neighborZone >= 0.0) {
				return vec3(1.0, 1.0, neighborZone);				
			}
		}
	}	
	
	return vec3(zoneX, zoneY, -1);	
}

vec3 calculateZoneUV(vec3 worldVertex) {
	ivec2 zonesMapPosition = ivec2(0);
	return calculateZoneUVWithZoneInfo(worldVertex, zonesMapPosition);
}