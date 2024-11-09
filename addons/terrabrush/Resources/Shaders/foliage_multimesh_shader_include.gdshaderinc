// The position of the camera
uniform vec3 GlobalPosition;
uniform vec3 OffsetPosition;

uniform float InitialCellWidth = 1.0;
uniform int LODRowsPerLevel = 10;
uniform int LODLevels = 5;
uniform float WindStrength = 0.1;
uniform vec4 FoliageAlbedo: source_color = vec4(1.0);
uniform sampler2DArray FoliageAlbedoTextures: hint_default_white, repeat_disable;
uniform int FoliageNumberOfTexture;
uniform bool UseGroundColor = true;
uniform vec3 MeshScale = vec3(1.0);
uniform bool UseBrushScale = true;
uniform sampler2D ScaleNoiseTexture: hint_default_white;
uniform float RandomPlacementRange = 2.0;
uniform int[100] ApplyOnTextureIndexes;
uniform int NumberOfTexturesToApplyOn = 0;

uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;
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

bool calculateTexturePresence(vec3 zoneUV) {
	for (int i = 0; i < NumberOfTexturesToApplyOn; i++) {
		int textureIndex = ApplyOnTextureIndexes[i];
		vec3 splatmapUV = vec3(zoneUV.x, zoneUV.y, floor(float(textureIndex/4)) + (zoneUV.z * ceil(float(NumberOfTextures) / 4.0)));
		
		int channelIndex = textureIndex % 4;
		vec4 currentSplatmap = texture(Splatmaps, splatmapUV);
		float channelValue = currentSplatmap[channelIndex];

		if (channelValue > 0.2) {
			return true;
		}
	}

	return false;
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

void calculateFoliageVertex(int instanceID, inout vec3 vertex, inout vec3 normal, inout vec4 color, out vec3 instancePosition) {
	// Compute the clipmap INDEX position
	int currentIndex = instanceID;

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

			if (isBottomLines) {
				clipMapYPosition *= -1.0; // We flip the points to be at the bottom
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

			if (isRightSide) {
				clipMapXPosition *= -1.0;
			}
		} else {
			// Disable instance
			vertex = vec3(0.0); // Scale down the mesh so it "vanish"
		}

		cellWidth = pow(float(2), float(currentLODLevel - 1)) * InitialCellWidth;
	}

	vec3 particlePosition = vec3(clipMapXPosition * cellWidth, 0, clipMapYPosition * cellWidth);
	particlePosition -= OffsetPosition;

	// Apply the global position on top of the clipmap
	particlePosition.x += roundEven(GlobalPosition.x);
	particlePosition.z += roundEven(GlobalPosition.z);

	instancePosition = particlePosition;

	// Fetch the zoneUV with the "Grid" position of the foliage.
	vec3 worldVertex = vec3(particlePosition.x, 0, particlePosition.z);
	vec3 zoneUV = calculateZoneUV(worldVertex);

	// Calculate the noise/random value with the zoneUV;
	vec4 noiseTextureValue = texture(NoiseTexture, zoneUV.xy);
	vec3 noiseRandom = hash_noise_range(noiseTextureValue.rgb);
	float noiseRandomXValue = mix(-RandomPlacementRange, RandomPlacementRange, noiseRandom.x);
	float noiseRandomZValue = mix(-RandomPlacementRange, RandomPlacementRange, noiseRandom.z);

	particlePosition.x += noiseRandomXValue;
	particlePosition.z += noiseRandomZValue;

	// Update the zoneUV so it fits the new position of the foliage, with the random value
	worldVertex = vec3(particlePosition.x, 0, particlePosition.z);
	zoneUV = calculateZoneUV(worldVertex);

	if (zoneUV.z < 0.0) {
		// Disable instance
		vertex = vec3(0.0); // Scale down the mesh so it "vanish"
	} else {
		vec4 foliagePosition = texture(FoliageTextures, zoneUV);
		bool foliagePresence = false;
		bool presenceByTexture = false;
		if (NumberOfTexturesToApplyOn > 0) {
			foliagePresence = calculateTexturePresence(zoneUV);
			
			if (foliagePresence) {
				presenceByTexture = true;				
			}
		}
		
		if (!foliagePresence) {
			foliagePresence = foliagePosition.x > 0.0;	
		}

		if (foliagePresence) {
			// HeightMap Texture (height calculation)
			vec4 heightMapPosition = texture(HeightmapTextures, zoneUV);
			if (heightMapPosition.g > 0.0) {
				// Disable instance
				vertex = vec3(0.0); // Scale down the mesh so it "vanish"
			} else {
				float height = heightMapPosition.x;

				vec4 waterPosition = texture(WaterTextures, zoneUV);
				float waterHeight = waterPosition.x * WaterFactor;

				height -= waterHeight;

				particlePosition.y = height;

				// Apply the meshscale
				vertex *= MeshScale;

				if (UseBrushScale && !presenceByTexture){
					// Adjust the scale of the grass with the alpha of the texture
					float meshScaleFactor = foliagePosition.a;
					vertex *= meshScaleFactor;
				}

				vertex *= texture(ScaleNoiseTexture, zoneUV.xy).r;

				// Assign the position to the transform
				vertex.x += particlePosition.x;
				vertex.y += particlePosition.y;
				vertex.z += particlePosition.z;

				// Make the foliage wave with wind
				float randomValue = hash_noise_range(instancePosition).x;
				float windDirection = 0.0;
				if (randomValue > 0.5) {
					windDirection = sin(TIME);
				} else {
					windDirection = cos(TIME);
				}
				windDirection *= randomValue * WindStrength;
				vertex = ((vertex - particlePosition) * rotateX(windDirection)) + particlePosition;

				// Apply random rotation to each foliage
				float randomRotation = mix(0, radians(360), randomValue);
				mat3 randomRotationMat = rotateY(randomRotation);
				vertex = ((vertex - particlePosition) * randomRotationMat) + particlePosition;

				normal *= randomRotationMat;

				if (UseGroundColor) {
					color = calculateGroundColor(zoneUV);
				}
			}
		} else {
			// Disable instance
			vertex = vec3(0.0); // Scale down the mesh so it "vanish"
		}
	}
}

void calculateFoliageFragment(vec2 uv, vec4 color, vec3 instancePosition, out vec3 albedo, out float alpha, out float alphaScissorThreshold, out float alphaAntiAliasingEdge) {
	vec4 albedoTexture = vec4(1.0);
	if (FoliageNumberOfTexture > 0) {
		float randomValue = hash_noise_range(instancePosition).x;
		float randomTexture = mix(0.0, float(FoliageNumberOfTexture - 1), randomValue);

		albedoTexture = texture(FoliageAlbedoTextures, vec3(uv.x, uv.y, randomTexture));
	}

	albedo = color.rgb * albedoTexture.rgb * FoliageAlbedo.rgb;
	alpha = FoliageAlbedo.a * albedoTexture.a;
	alphaScissorThreshold = 0.01;
	alphaAntiAliasingEdge = 0.5;
}
