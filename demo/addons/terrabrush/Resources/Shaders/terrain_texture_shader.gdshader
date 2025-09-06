shader_type canvas_item;

uniform sampler2DArray Textures : source_color;
uniform sampler2DArray Splatmaps;
uniform int NumberOfTextures;

void fragment() {
	vec4 resultTexture = vec4(0.0, 0.0, 0.0, 0.0);
	
	int currentChannel = 0;
	for (int i = 0; i < NumberOfTextures; i++) {
		vec2 detailUV = UV;
		vec3 textureUV = vec3(detailUV.x, detailUV.y, float(i));
		vec3 splatmapUV = vec3(UV.x, UV.y, floor(float(i/4)));
	
		vec4 currentTexture = texture(Textures, textureUV) ;
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
	
	COLOR = vec4(resultTexture.r, resultTexture.g, resultTexture.b, 1.0);
}
