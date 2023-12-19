shader_type particles;

uniform int HeightMapSize;
uniform sampler2D FoliageTexture : hint_default_transparent, repeat_disable;
uniform sampler2D HeightMapTexture : hint_default_transparent, repeat_disable;
uniform sampler2D NoiseTexture : hint_default_transparent, repeat_disable;
uniform sampler2D GroudTexture : hint_default_transparent, repeat_disable;
uniform sampler2D WaterTexture : hint_default_transparent, repeat_disable;
uniform float HeightMapFactor;
uniform float WaterFactor;
uniform vec3 PlayerPosition = vec3(0.0);
uniform int MaximumDistance;
uniform vec3 MeshScale = vec3(1.0);
uniform float WindStrength = 0.1;

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

void start() {
	float xPosition = roundEven(floor(float(int(INDEX) / MaximumDistance)) + (float(HeightMapSize / 2) - (float(MaximumDistance / 2))) + PlayerPosition.x);
	float yPosition = roundEven(float(int(INDEX) % MaximumDistance) + (float(HeightMapSize / 2) - (float(MaximumDistance / 2))) + PlayerPosition.z);
	
	vec3 particlePosition = vec3(float(xPosition), 0, float(yPosition)) - vec3(float(HeightMapSize / 2), 0, float(HeightMapSize / 2));	
	
	vec4 noiseTextureValue = texelFetch(NoiseTexture, ivec2(int(xPosition), int(yPosition)), 0);
	float noiseRandom = hash_noise_range(noiseTextureValue.rgb).x;
	float noiseValue = mix(-1, 1, noiseRandom);
	particlePosition += vec3(noiseValue, 0, noiseValue);
		
	vec3 playerDistance = particlePosition - vec3(PlayerPosition.x, 0, PlayerPosition.z);
	
	if (length(abs(playerDistance)) > float(MaximumDistance)) {
	 	ACTIVE = false;
	} else {
		if (RESTART_POSITION) {
			float halfMap = float(HeightMapSize) / 2.0f;
			
			// Foliage Texture (foliage presence calculation)
			vec4 foliagePosition = texelFetch(FoliageTexture, ivec2(int(xPosition), int(yPosition)), 0);
			bool foliagePresence = foliagePosition.x > 0.0 && (particlePosition.x > -halfMap && particlePosition.x < halfMap) && (particlePosition.z > -halfMap && particlePosition.z < halfMap);
			
			if (foliagePresence) {
				ACTIVE = true;
				
				// HeightMap Texture (height calculation)
				vec4 heightMapPosition = texelFetch(HeightMapTexture, ivec2(int(xPosition + noiseValue), int(yPosition + noiseValue)), 0);
				float height = heightMapPosition.x * HeightMapFactor;
				
				vec4 waterPosition = texelFetch(WaterTexture, ivec2(int(xPosition + noiseValue), int(yPosition + noiseValue)), 0);
				float waterHeight = waterPosition.x * WaterFactor;
				
				height -= waterHeight;
				
				particlePosition.y = height;
						
				// Assign the position to the transform
				TRANSFORM = mat4(vec4(MeshScale.x * foliagePosition.a, 0.0, 0.0, 0.0), vec4(0.0, MeshScale.y * foliagePosition.a, 0.0, 0.0), vec4(0.0, 0.0, MeshScale.z * foliagePosition.a, 0.0), vec4(particlePosition, 1.0));
				
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
			} else {
				ACTIVE = false;
			}
			
			vec4 groundTextureValue = texelFetch(GroudTexture, ivec2(int(xPosition + noiseValue), int(yPosition + noiseValue)), 0);
			COLOR = groundTextureValue;
		}
	}
}

void process() {
}
