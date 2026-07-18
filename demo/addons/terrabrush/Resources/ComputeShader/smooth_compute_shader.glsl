#[compute]
#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(r8, binding = 0) restrict uniform image2D heightmapTexture;
layout(rgba8, binding = 1) restrict readonly uniform image2D brushTexture; ;
layout(set = 0, binding = 2) uniform Params {
    float brushStrength;
} params;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 brushPixel = imageLoad(brushTexture, coords);

    vec4 leftHeightmapPixel = imageLoad(heightmapTexture, coords - ivec2(1, 0));
    vec4 topHeightmapPixel = imageLoad(heightmapTexture, coords - ivec2(0, 1));
    vec4 rightHeightmapPixel = imageLoad(heightmapTexture, coords + ivec2(1, 0));
    vec4 bottomHeightmapPixel = imageLoad(heightmapTexture, coords + ivec2(0, 1));

    vec4 heightmapPixel = imageLoad(heightmapTexture, coords);

    float average = (leftHeightmapPixel.r + topHeightmapPixel.r + rightHeightmapPixel.r + bottomHeightmapPixel.r + heightmapPixel.r) / 5.0;

    heightmapPixel.r = mix(heightmapPixel.r, average, brushPixel.a * params.brushStrength);

    imageStore(heightmapTexture, coords, heightmapPixel);
}
