#[compute]
#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(r8, binding = 0) restrict uniform image2D heightmapTexture;
layout(rgba8, binding = 1) restrict readonly uniform image2D brushTexture; ;
layout(set = 0, binding = 2) uniform Params {
    float brushStrength;
    bool add;
} params;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 heightmapPixel = imageLoad(heightmapTexture, coords);
    vec4 brushPixel = imageLoad(brushTexture, coords);

    float sculptValue = brushPixel.a * params.brushStrength;
    if (!params.add) {
        sculptValue *= -1.0;
    }

    heightmapPixel.r += sculptValue;

    imageStore(heightmapTexture, coords, heightmapPixel);
}
