//uniform sampler2D HeightmapTexture : hint_default_transparent, repeat_disable;
uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float ZonesSize;
uniform sampler2D ZonesMap;
uniform float NumberOfZones;
uniform float InitialCellWidth = 1.0;

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

void calculateVertex(mat4 modelMatrix, vec4 levelsMeta, inout vec3 vertex, out vec2 textureUV, out vec3 worldVertex) {
	worldVertex = vertex + modelMatrix[3].xyz;
	textureUV = (worldVertex.xz + (ZonesSize / 2.0)) / ZonesSize;
	
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