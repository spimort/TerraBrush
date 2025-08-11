shader_type particles;

uniform sampler2DArray HeightmapTextures: hint_default_black, repeat_disable;

uniform vec3 GlobalPosition;
uniform vec3 OffsetPosition;
uniform sampler2DArray FoliageTextures : hint_default_black, repeat_disable;
uniform sampler2D NoiseTexture : hint_default_transparent, repeat_disable;

// Textures to calculate ground color
uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps : repeat_disable;
uniform int TextureDetail;
uniform int NumberOfTextures;

uniform sampler2DArray WaterTextures : hint_default_transparent, repeat_disable;
uniform float WaterFactor;

uniform int MaximumDistance;
uniform vec3 MeshScale = vec3(1.0);
uniform float WindStrength = 0.1;

#include "zones_shader_include.gdshaderinc"

// 3D Noise with friendly permission by Inigo Quilez
vec3 hash_noise_range(vec3 p) {
	p *= mat3(vec3(127.1, 311.7, -53.7), vec3(269.5, 183.3, 77.1), vec3(-301.7, 27.3, 215.3));
	return 2.0 * fract(fract(p)*4375.55) -1.;
}

mat4 __build_rotation_mat4(vec3 axis, float angle) {
	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;
	return mat4(vec4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0), vec4(oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0), vec4(oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0), vec4(0, 0, 0, 1));
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

float remap(float value, float from1, float to1, float from2, float to2) {
    return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void start() {
	vec3 globalPosition = GlobalPosition - OffsetPosition;
	
	float xPosition = roundEven(floor(float(int(INDEX) / MaximumDistance)) - (float(MaximumDistance / 2)));
	float yPosition = roundEven(float(int(INDEX) % MaximumDistance) - (float(MaximumDistance / 2)));

	xPosition += roundEven(globalPosition.x);
	yPosition += roundEven(globalPosition.z);

	// Fetch the zoneUV with the "Grid" position of the foliage.
	vec3 worldVertex = vec3(xPosition, 0, yPosition) - OffsetPosition;
	vec3 zoneUV = calculateZoneUV(worldVertex);

	// Calculate the noise/random value with the zoneUV;
	vec4 noiseTextureValue = texture(NoiseTexture, zoneUV.xy);
	float noiseRandom = hash_noise_range(noiseTextureValue.rgb).x;
	float noiseValue = mix(-1, 1, noiseRandom);

	xPosition += noiseValue;
	yPosition += noiseValue;

	// Update the zoneUV so it fits the new position of the foliage, with the random value
	worldVertex = vec3(xPosition, 0, yPosition);
	zoneUV = calculateZoneUV(worldVertex);

	if (zoneUV.z < 0.0) {
	 	ACTIVE = false;
	} else {
		if (RESTART_POSITION) {
			vec4 foliagePosition = texture(FoliageTextures, zoneUV);
			bool foliagePresence = foliagePosition.x > 0.0;

			float particleDistance = length(vec2(xPosition, yPosition) - globalPosition.xz);
			// From center means that we need to divide the max distance by 2.0
			float maxDistanceFromCenter = float(MaximumDistance) / 2.0;
			if (foliagePresence && particleDistance <= maxDistanceFromCenter) {
				// HeightMap Texture (height calculation)
				vec4 heightMapPosition = texture(HeightmapTextures, zoneUV);
				if (heightMapPosition.g > 0.0) {
					ACTIVE = false;
				} else {
					ACTIVE = true;

					vec3 particlePosition = vec3(xPosition, 0, yPosition);
					float height = heightMapPosition.x;

					vec4 waterPosition = texture(WaterTextures, zoneUV);
					float waterHeight = waterPosition.x * WaterFactor;

					height -= waterHeight;

					particlePosition.y = height;

					float meshScaleFactor = foliagePosition.a;

					// Calculate the distance so we can make the foliage fade when far distance
					float scaleFalloffDistance = maxDistanceFromCenter / 10.0;
					if (particleDistance >= maxDistanceFromCenter - scaleFalloffDistance) {
						meshScaleFactor = remap(particleDistance, maxDistanceFromCenter - scaleFalloffDistance, maxDistanceFromCenter, meshScaleFactor, 0.0);
					}

					// Assign the position to the transform
					TRANSFORM = mat4(vec4(MeshScale.x * meshScaleFactor, 0.0, 0.0, 0.0), vec4(0.0, MeshScale.y * meshScaleFactor, 0.0, 0.0), vec4(0.0, 0.0, MeshScale.z * meshScaleFactor, 0.0), vec4(particlePosition, 1.0));

					// Apply random rotation to each foliage
					float randomValue = hash_noise_range(particlePosition).x;
					float randomRotation = mix(0, radians(360), randomValue);
					TRANSFORM *= __build_rotation_mat4(vec3(0, 1, 0), randomRotation);

					// Apply the transform for each blades
					TRANSFORM = EMISSION_TRANSFORM * TRANSFORM;

					// Make the foliage wave with wind
					float windDirection = 0.0;
					if (randomValue > 0.5) {
						windDirection = sin(TIME);
					} else {
						windDirection = cos(TIME);
					}
					windDirection *= randomValue * WindStrength;

					TRANSFORM[1] += vec4(windDirection, 0, windDirection, 1.0);

					COLOR = calculateGroundColor(zoneUV);
				}
			} else {
				ACTIVE = false;
			}
		}
	}
}

void process() {
}