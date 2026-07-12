#[compute]
#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(r8, binding = 0) restrict uniform image2DArray heightmapTextures;
layout(rgba8, binding = 1) restrict readonly uniform image2D brushTexture; ;
layout(set = 0, binding = 2) uniform Params {
    int numberOfHeightmaps;
    float brushStrength;
} params;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 brushPixel = imageLoad(brushTexture, coords);

    for (int i = 0; i < params.numberOfHeightmaps; i++) {
        ivec3 heightmapCoords = ivec3(coords, i);

        vec4 leftHeightmapPixel = imageLoad(heightmapTextures, heightmapCoords - ivec3(1, 0, i));
        vec4 topHeightmapPixel = imageLoad(heightmapTextures, heightmapCoords - ivec3(0, 1, i));
        vec4 rightHeightmapPixel = imageLoad(heightmapTextures, heightmapCoords + ivec3(1, 0, i));
        vec4 bottomHeightmapPixel = imageLoad(heightmapTextures, heightmapCoords + ivec3(0, 1, i));

        vec4 heightmapPixel = imageLoad(heightmapTextures, heightmapCoords);

        float average = (leftHeightmapPixel.r + topHeightmapPixel.r + rightHeightmapPixel.r + bottomHeightmapPixel.r + heightmapPixel.r) / 5.0;

        heightmapPixel.r = mix(heightmapPixel.r, average, brushPixel.a * params.brushStrength);

        imageStore(heightmapTextures, heightmapCoords, heightmapPixel);
    }
}
