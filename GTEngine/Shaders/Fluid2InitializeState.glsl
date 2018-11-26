// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

layout(r32f) uniform readonly image2D density;
layout(rg32f) uniform readonly image2D velocity;
layout(rgba32f) uniform writeonly image2D stateTm1;
layout(rgba32f) uniform writeonly image2D stateT;

layout (local_size_x = NUM_X_THREADS, local_size_y = NUM_Y_THREADS, local_size_z = 1) in;
void main()
{
    ivec2 c = ivec2(gl_GlobalInvocationID.xy);
    vec4 initial = vec4(imageLoad(velocity, c).xy, 0.0f, imageLoad(density, c).x);
    imageStore(stateTm1, c, initial);
    imageStore(stateT, c, initial);
};
