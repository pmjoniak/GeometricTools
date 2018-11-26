// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

uniform Weights
{
    float weight[2 * RADIUS + 1];
};

layout(rgba32f) uniform readonly image2D inImage;
layout(rgba32f) uniform writeonly image2D outImage;

#define NUM_Y_THREADS 768
shared vec4 samples[NUM_Y_THREADS + 2*RADIUS];

layout (local_size_x = 1, local_size_y = NUM_Y_THREADS, local_size_z = 1) in;
void main()
{
    ivec2 dt = ivec2(gl_GlobalInvocationID.xy);
    ivec2 gt = ivec2(gl_LocalInvocationID.xy);

    // Load the texels from the input texture, store them in group-shared
    // memory, and have all threads in the group wait until all texels
    // are loaded.
    samples[gt.y + RADIUS] = imageLoad(inImage, dt);
    if (gt.y < RADIUS)
    {
        samples[gt.y] = imageLoad(inImage, dt - ivec2(0, RADIUS));
    }
    else if (gt.y >= NUM_Y_THREADS - RADIUS)
    {
        samples[gt.y + 2 * RADIUS] = imageLoad(inImage, dt + ivec2(0, RADIUS));
    }
    groupMemoryBarrier();

    vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int y = 0; y <= 2*RADIUS; ++y)
    {
        result += weight[y] * samples[gt.y + y];
    }
    imageStore(outImage, dt, result);
}
