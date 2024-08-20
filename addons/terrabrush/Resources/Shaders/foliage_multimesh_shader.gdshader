shader_type spatial;
render_mode cull_disabled, diffuse_burley;

uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;

uniform vec3 GlobalPosition;
uniform sampler2DArray FoliageTextures : hint_default_black, repeat_disable;
uniform sampler2D NoiseTexture : hint_default_transparent, repeat_disable;

// Textures to calculate ground color
uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps : repeat_disable;
uniform int TextureDetail;
uniform int NumberOfTextures;

// Water textures to adjust the height
uniform sampler2DArray WaterTextures : hint_default_transparent, repeat_disable;
uniform float WaterFactor;

uniform float InitialCellWidth = 1.0;
uniform int LODRowsPerLevel = 10;
uniform int LODLevels = 5;

uniform float WindStrength = 0.1;

#include "zones_shader_include.gdshaderinc"

// 3D Noise with friendly permission by Inigo Quilez
vec3 hash_noise_range(vec3 p) {
	p *= mat3(vec3(127.1, 311.7, -53.7), vec3(269.5, 183.3, 77.1), vec3(-301.7, 27.3, 215.3));
	return 2.0 * fract(fract(p)*4375.55) -1.;
}

// This code has been taken from https://godotshaders.com/shader/vertex-mesh-rotator/
mat3 rotateZ(float theta) {	
	float cosa = cos(theta);
	float sina = sin(theta);
 
    //Y rotation	
	mat3 rotate_z  = mat3(
	   vec3(cosa,-sina, 0.0),
	   vec3(sina, cosa, 0.0),
	   vec3(0.0, 0.0, 1.0)
	);
	
	return rotate_z;
}

mat3 rotateY(float theta) {	
	float cosa = cos(theta);
	float sina = sin(theta);
 
    //Y rotation	
	mat3 rotate_y  = mat3(
	   vec3(cosa, 0.0, sina),
	   vec3(0.0, 1.0, 0.0),
	   vec3(-sina, 0.0, cosa)
	);
	
	return rotate_y;
}

mat3 rotateX(float theta) {	
	float cosa = cos(theta);
	float sina = sin(theta);
 
    //Y rotation	
	mat3 rotate_x  = mat3(
	   vec3(1.0, 0.0, 0.0),
	   vec3(0.0, cosa, -sina),
	   vec3(0.0, sina, cosa)
	);
	
	return rotate_x;
}

vec4 calculateGroundColor(vec3 zoneUV) {
	vec4 resultTexture = vec4(0.0);
	
	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		vec3 detailUV = vec3(zoneUV.x, zoneUV.y, zoneUV.z);
		vec3 textureUV = vec3(detailUV.x * float(TextureDetail), detailUV.y * float(TextureDetail), float(i));
		vec3 splatmapUV = vec3(detailUV.x, detailUV.y, floor(float(i/4)) + (detailUV.z * ceil(float(NumberOfTextures) / 4.0)));
	
		vec4 currentTexture = texture(Textures, textureUV);
		vec4 currentSplatmap = texture(Splatmaps, splatmapUV);
	
		float channelValue = 0.0;
		if (currentChannel == 0) {
			channelValue = currentSplatmap.x;
		} else if (currentChannel == 1) {
			channelValue = currentSplatmap.y;
		} else if (currentChannel == 2) {
			channelValue = currentSplatmap.z;
		} else if (currentChannel == 3) {
			channelValue = currentSplatmap.w;
		}
	
		resultTexture += currentTexture * channelValue;
	
		if (currentChannel == 3) {
			currentChannel = 0;
		} else {
			currentChannel++;
		}
	}
	
	return resultTexture;	
}

void vertex() {
	// Compute the clipmap INDEX position
	int currentIndex = int(INSTANCE_ID);
	
	int lodRowsPerLevel = LODRowsPerLevel;
	if (lodRowsPerLevel % 2 == 0) {
		// The number of rows per level cannot be even
		lodRowsPerLevel += 1;
	}	
	
	int lodLevelNumberOfPoints = (lodRowsPerLevel - (-1 - lodRowsPerLevel)) + 2;	
	
	float clipMapXPosition = 0.0f;
	float clipMapYPosition = 0.0f;	
	
	float cellWidth = InitialCellWidth;
	int currentLODLevel = 0;
		
	if (currentIndex < (lodLevelNumberOfPoints * lodLevelNumberOfPoints)) {
		COLOR = vec4(1, 0, 0, 1);
		currentLODLevel = 1;
		
		clipMapXPosition = roundEven(float(currentIndex % lodLevelNumberOfPoints) - (float(lodLevelNumberOfPoints / 2)));
		clipMapYPosition = roundEven(floor(float(currentIndex / lodLevelNumberOfPoints)) - (float(lodLevelNumberOfPoints / 2)));		
	} else {		
		int remainingLODLevels = LODLevels - 1;
		
		int topLines = (lodRowsPerLevel + 1) / 2;
		int numberOfPointsTopLines = lodLevelNumberOfPoints * topLines * remainingLODLevels;
		
		int numberOfPointsPerSide = lodLevelNumberOfPoints - (topLines * 2);
		int numberOfSides = topLines;
		int numberOfPointsLeftSide = numberOfSides * numberOfPointsPerSide * remainingLODLevels;
			
		currentIndex -= (lodLevelNumberOfPoints * lodLevelNumberOfPoints);	
		
		if (currentIndex < numberOfPointsTopLines * 2) { // We are on the top lines
			bool isBottomLines = currentIndex >= numberOfPointsTopLines;
			if (isBottomLines) {
				currentIndex -= numberOfPointsTopLines; // We remove the top lines index so we restart at 0 for the bottom
			}
			
			currentLODLevel = LODLevels - int(floor(float(currentIndex / (lodLevelNumberOfPoints * topLines))));
			currentIndex -= (LODLevels - currentLODLevel) * (lodLevelNumberOfPoints * topLines); // We reset the index on each LODLevel
			
			clipMapXPosition = roundEven(float(currentIndex % lodLevelNumberOfPoints) - (float(lodLevelNumberOfPoints / 2)));
			clipMapYPosition = roundEven(floor(float(currentIndex / lodLevelNumberOfPoints)) - (float(lodLevelNumberOfPoints / 2)));
			
			COLOR = vec4(1, 0.5, 1, 1);
			
			if (isBottomLines) {
				clipMapYPosition *= -1.0; // We flip the points to be at the bottom
				COLOR = vec4(0, 1, 0, 1);					
			}			
		} else if (currentIndex < (numberOfPointsTopLines * 2) + (numberOfPointsLeftSide * 2)) { // We are at the left side
			bool isRightSide = currentIndex >= (numberOfPointsTopLines * 2) + numberOfPointsLeftSide;
			if (isRightSide) {
				currentIndex -= (numberOfPointsTopLines * 2) + numberOfPointsLeftSide;
			} else {
				currentIndex -= numberOfPointsTopLines * 2;				
			}
			
			currentLODLevel = LODLevels - int(floor(float(currentIndex / (numberOfPointsPerSide * numberOfSides))));
			currentIndex -= (LODLevels - currentLODLevel) * (numberOfPointsPerSide * numberOfSides);// We reset the index on each LODLevel	
			
			clipMapXPosition = roundEven(float(currentIndex % numberOfSides) - (float(lodLevelNumberOfPoints / 2)));
			clipMapYPosition = roundEven(floor(float(currentIndex / numberOfSides)) - (float(lodLevelNumberOfPoints / 2)));
					
			clipMapYPosition += float(topLines) ;	
			
			COLOR = vec4(1);
			
			if (isRightSide) {
				clipMapXPosition *= -1.0;
				COLOR = vec4(1, 1, 0, 1);
			}			
		} else {
			// Disable instance			
			VERTEX.y = 0.0; // Scale down the mesh so it "vanish"
		}
		
		cellWidth = pow(float(2), float(currentLODLevel - 1)) * InitialCellWidth;	
	}		
	
	vec3 particlePosition = vec3(clipMapXPosition * cellWidth, 0, clipMapYPosition * cellWidth);			
	
	// Apply the global position on top of the clipmap
	particlePosition.x += roundEven(GlobalPosition.x);
	particlePosition.z += roundEven(GlobalPosition.z);
	
	// Fetch the zoneUV with the "Grid" position of the foliage.
	vec3 worldVertex = vec3(particlePosition.x, 0, particlePosition.z);
	vec3 zoneUV = calculateZoneUV(worldVertex);	
	
	// Calculate the noise/random value with the zoneUV;
	vec4 noiseTextureValue = texture(NoiseTexture, zoneUV.xy);
	float noiseRandom = hash_noise_range(noiseTextureValue.rgb).x;
	float noiseValue = mix(-1, 1, noiseRandom);
	
	particlePosition.x += noiseValue;			
	particlePosition.z += noiseValue;	
	
	// Update the zoneUV so it fits the new position of the foliage, with the random value
	worldVertex = vec3(particlePosition.x, 0, particlePosition.z);
	zoneUV = calculateZoneUV(worldVertex);					
	
	if (zoneUV.z < 0.0) {
		// Disable instance			
		VERTEX.y = 0.0; // Scale down the mesh so it "vanish"
	} else {
		vec4 foliagePosition = texture(FoliageTextures, zoneUV);						
		bool foliagePresence = foliagePosition.x > 0.0;

		if (foliagePresence) {			
			// HeightMap Texture (height calculation)
			vec4 heightMapPosition = texture(HeightmapTextures, zoneUV);
			if (heightMapPosition.g > 0.0) {
				// Disable instance			
				VERTEX.y = 0.0; // Scale down the mesh so it "vanish"
			} else {				
				float height = heightMapPosition.x;
				
				vec4 waterPosition = texture(WaterTextures, zoneUV);
				float waterHeight = waterPosition.x * WaterFactor;
				
				height -= waterHeight;
				
				particlePosition.y = height;				
						
				// Assign the position to the transform
				VERTEX.x += particlePosition.x;
				VERTEX.y += particlePosition.y;
				VERTEX.z += particlePosition.z;
				
				// Make the foliage wave with wind
				float randomValue = hash_noise_range(particlePosition).x;
				float windDirection = 0.0;				
				if (randomValue > 0.5) {
					windDirection = sin(TIME);
				} else {
					windDirection = cos(TIME);
				}
				windDirection *= randomValue * WindStrength;				
				VERTEX = ((VERTEX - particlePosition) * rotateX(windDirection)) + particlePosition;				

				// Apply random rotation to each foliage				
				float randomRotation = mix(0, radians(360), randomValue);				
				mat3 randomRotationMat = rotateY(randomRotation);				
				VERTEX = ((VERTEX - particlePosition) * randomRotationMat) + particlePosition;						
				
				COLOR = calculateGroundColor(zoneUV);
			}
		} else {
			// Disable instance			
			VERTEX.y = 0.0; // Scale down the mesh so it "vanish"
		}
	}
}

void fragment() {
	ALBEDO = COLOR.rgb;
}
