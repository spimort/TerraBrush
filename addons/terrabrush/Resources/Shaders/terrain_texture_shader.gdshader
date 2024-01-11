shader_type canvas_item;

uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps;
uniform int NumberOfTextures;
uniform sampler2DArray Normals;

vec2 rotate(vec2 v, float cosa, float sina) {
	return vec2(cosa * v.x - sina * v.y, sina * v.x + cosa * v.y);
}

vec4 depth_blend2(vec4 a_value, float a_bump, vec4 b_value, float b_bump, float t) {
	// https://www.gamasutra.com
	// /blogs/AndreyMishkinis/20130716/196339/Advanced_Terrain_Texture_Splatting.php
	float d = 0.1;
	float ma = max(a_bump + (1.0 - t), b_bump + t) - d;
	float ba = max(a_bump + (1.0 - t) - ma, 0.0);
	float bb = max(b_bump + t - ma, 0.0);
	return (a_value * ba + b_value * bb) / (ba + bb);
}

// Antitile from zylann's shader
vec4 texture_array_antitile(sampler2DArray tex, vec3 uv) {
	float frequency = 2.0;
	float scale = 1.3;
	float sharpness = 0.7;

	// Rotate and scale UV
	float rot = 3.14 * 0.6;
	float cosa = cos(rot);
	float sina = sin(rot);
	vec3 uv2 = vec3(rotate(uv.xy, cosa, sina) * scale, uv.z);

	vec4 col0 = texture(tex, uv);
	vec4 col1 = texture(tex, uv2);

	// Periodically alternate between the two versions using a warped checker pattern
	float t = 1.1 + 0.5
		* sin(uv2.x * frequency + sin(uv.x) * 2.0)
		* cos(uv2.y * frequency + sin(uv.y) * 2.0);
	t = smoothstep(sharpness, 2.0 - sharpness, t);

	return depth_blend2(col0, col0.a, col1, col1.a, t);
}

void fragment() {
	vec4 resultTexture = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 resultNormal = vec4(0.0, 0.0, 0.0, 0.0);
	
	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		vec2 detailUV = UV;
		vec3 textureUV = vec3(detailUV.x, detailUV.y, float(i));
		vec3 splatmapUV = vec3(UV.x, UV.y, floor(float(i/4)));
	
		vec4 currentTexture = texture(Textures, textureUV) ;
		vec4 currentNormal = texture(Normals, textureUV);
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
		resultNormal += currentNormal * channelValue;
	
		if (currentChannel == 3) {
			currentChannel = 0;
		} else {
			currentChannel++;
		}
	}
	
	COLOR = vec4(resultTexture.r, resultTexture.g, resultTexture.b, 1.0);
	
	//vec2 uv = UV * 1.1;
	//COLOR = texture(Textures, vec3(uv.x, uv.y, 0.0));
}
