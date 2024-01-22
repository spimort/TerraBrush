//uniform sampler2D HeightmapTexture : hint_default_transparent, repeat_disable;
uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
uniform float ZonesSize;
uniform sampler2D ZonesMap;
uniform float NumberOfZones;
uniform float InitialCellWidth = 1.0;

void calculateVertex(mat4 modelMatrix, vec4 levelsMeta, inout vec3 vertex, out vec2 textureUV, out vec3 worldVertex) {
	float fixedZonesSize = ZonesSize - 1.0; 
	worldVertex = vertex + modelMatrix[3].xyz;
	
	textureUV = (worldVertex.xz + (fixedZonesSize / 2.0)) / fixedZonesSize;
	
	ivec2 zonesMapSize = textureSize(ZonesMap, 0);

	ivec2 zonesStep = ivec2(int(floor(textureUV.x)), int(floor(textureUV.y)));
	int xZonesMap = zonesStep.x + int(zonesMapSize.x) / 2;
	int yZonesMap = zonesStep.y + int(zonesMapSize.y) / 2;
	
	vec4 zoneInfoPosition = vec4(-1);	
	if (xZonesMap >= 0 && xZonesMap < zonesMapSize.x && yZonesMap >= 0 && yZonesMap < zonesMapSize.y) {
		zoneInfoPosition = texelFetch(ZonesMap, ivec2(xZonesMap, yZonesMap), 0);
	}
	
	if (zoneInfoPosition.r >= 0.0) {
		ivec3 pixelCoords = ivec3(
			int(floor(worldVertex.x) + (fixedZonesSize / 2.0) - (float(zonesStep.x) * fixedZonesSize)), 
			int(floor(worldVertex.z) + (fixedZonesSize / 2.0) - (float(zonesStep.y) * fixedZonesSize)), 
			int(floor(zoneInfoPosition.r))
		);
		vertex.y = texelFetch(HeightmapTextures, pixelCoords, 0).r;				
		
		// Calculate the average height for mid zone vertices
		//if (levelsMeta.g > 0.0 || levelsMeta.b > 0.0) {
			//int level = int(levelsMeta.a * 100.0);
			//float levelMultiplicator = pow(2.0, float(level));
			//vec2 prevNextUVVector = ((vec2(levelsMeta.g, levelsMeta.b) / ZonesSize) * levelMultiplicator) * InitialCellWidth;
			//vec3 zonePrevNextUVVector = vec3(prevNextUVVector.x, prevNextUVVector.y, zoneUV.z);
			//
			//float previousHeight = texture(HeightmapTextures, zoneUV - zonePrevNextUVVector).r;
			//float nextHeight = texture(HeightmapTextures, zoneUV + zonePrevNextUVVector).r;
			//
			//vertex.y = (previousHeight + nextHeight) / 2.0;
		//}		
	}				
}