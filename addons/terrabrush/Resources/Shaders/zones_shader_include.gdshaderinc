uniform float ZonesSize;
uniform sampler2D ZonesMap;
uniform float NumberOfZones;

vec3 calculateZoneUV(vec3 worldVertex) {
	float fixedZonesSize = ZonesSize - 1.0; 	
	vec2 zoneArea = (worldVertex.xz + (fixedZonesSize / 2.0)) / fixedZonesSize;
	
	ivec2 zonesMapSize = textureSize(ZonesMap, 0);

	ivec2 zonesStep = ivec2(int(floor(zoneArea.x)), int(floor(zoneArea.y)));
	int xZonesMap = zonesStep.x + int(zonesMapSize.x) / 2;
	int yZonesMap = zonesStep.y + int(zonesMapSize.y) / 2;
	
	vec4 zoneInfoPosition = vec4(-1);	
	if (xZonesMap >= 0 && xZonesMap < zonesMapSize.x && yZonesMap >= 0 && yZonesMap < zonesMapSize.y) {
		zoneInfoPosition = texelFetch(ZonesMap, ivec2(xZonesMap, yZonesMap), 0);
	}	
	
	if (zoneInfoPosition.r >= 0.0) {
		float sizeUnit = (1.0 / ZonesSize);
		float halfUnit = sizeUnit / 2.0;
		float fixedSizePercent = 1.0 - sizeUnit;
		vec3 zoneUV = vec3(
			(((zoneArea.x - (float(zonesStep.x))) * fixedSizePercent) + halfUnit), 
			(((zoneArea.y - (float(zonesStep.y))) * fixedSizePercent) + halfUnit),
			floor(zoneInfoPosition.r)
		);	
		
		return zoneUV;		
	}	
	
	return vec3(-1);
}