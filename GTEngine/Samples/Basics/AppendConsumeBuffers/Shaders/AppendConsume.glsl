// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

struct Particle
{
    ivec2 location;
};

// HLSL equivalent:
// ConsumeStructuredBuffer<Particle> currentState;
buffer currentState { Particle data[]; } currentStateAC;
layout(binding = 0, offset = 0) uniform atomic_uint currentStateCounter;

// HLSL equivalent:
// Particle p = currentState.Consume();
Particle currentStateConsume()
{
    uint index = atomicCounterDecrement(currentStateCounter);
    return currentStateAC.data[index];
}

// HLSL equivalent:
// AppendStructuredBuffer<Particle> nextState;
buffer nextState { Particle data[]; } nextStateAC;
layout(binding = 0, offset = 4) uniform atomic_uint nextStateCounter;

// HLSL equivalent:
// Particle p;
// nextState.Append(p);
void nextStateAppend(Particle p)
{
    uint index = atomicCounterIncrement(nextStateCounter);
    nextStateAC.data[index] = p;
}

// The test code uses glDispatchCompute(1,1,1), so 'id' is (x,0,0) with 0 <= x < 32.
layout (local_size_x = 32, local_size_y = 1, local_size_z = 1) in;
void main()
{
    uvec3 id = gl_LocalInvocationID;

    // Append only half the current state (the even-indexed ones.)
    Particle p = currentStateConsume();
    if ((p.location[0] & 1) == 0)
    {
        nextStateAppend(p);
    }
}
