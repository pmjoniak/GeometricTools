// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/08/31)

#include "GTEnginePCH.h"
#include "GteFluid2InitializeState.h"
#include "GteShaderFactory.h"
#include <random>
using namespace gte;

//----------------------------------------------------------------------------
Fluid2InitializeState::~Fluid2InitializeState()
{
}
//----------------------------------------------------------------------------
Fluid2InitializeState::Fluid2InitializeState(int xSize, int ySize,
    int numXThreads, int numYThreads)
    :
    mNumXGroups(xSize/numXThreads),
    mNumYGroups(ySize/numYThreads)
{
    // Use a Mersenne twister engine for random numbers.
    std::mt19937 mte;
    std::uniform_real_distribution<float> unirnd(0.0f, 1.0f);

    // Initial density values are randomly generated.
    mDensity.reset(new Texture2(DF_R32_FLOAT, xSize, ySize));
    float* data = mDensity->Get<float>();
    for (unsigned int i = 0; i < mDensity->GetNumElements(); ++i, ++data)
    {
        *data = unirnd(mte);
    }

    // Initial velocity values are zero.
    mVelocity.reset(new Texture2(DF_R32G32_FLOAT, xSize, ySize));
    memset(mVelocity->GetData(), 0, mVelocity->GetNumBytes());

    // The states at time 0 and time -dt are initialized by a compute shader.
    mStateTm1.reset(new Texture2(DF_R32G32B32A32_FLOAT, xSize, ySize));
    mStateTm1->SetUsage(Resource::SHADER_OUTPUT);

    mStateT.reset(new Texture2(DF_R32G32B32A32_FLOAT, xSize, ySize));
    mStateT->SetUsage(Resource::SHADER_OUTPUT);

    // Create the shader for initializing velocity and density.
    HLSLDefiner definer;
    definer.SetInt("NUM_X_THREADS", numXThreads);
    definer.SetInt("NUM_Y_THREADS", numYThreads);
    mInitializeState.reset(ShaderFactory::CreateCompute("InitializeState",
        msHLSLInitializeStateString, definer));
    LogAssert(mInitializeState != nullptr, "Cannot create compute shader.");
    mInitializeState->Set("density", mDensity);
    mInitializeState->Set("velocity", mVelocity);
    mInitializeState->Set("stateTm1", mStateTm1);
    mInitializeState->Set("stateT", mStateT);
}
//----------------------------------------------------------------------------
std::shared_ptr<Texture2> const& Fluid2InitializeState::GetStateTm1() const
{
    return mStateTm1;
}
//----------------------------------------------------------------------------
std::shared_ptr<Texture2> const& Fluid2InitializeState::GetStateT() const
{
    return mStateT;
}
//----------------------------------------------------------------------------
void Fluid2InitializeState::Execute(DX11Engine* engine)
{
    engine->Execute(mInitializeState, mNumXGroups, mNumYGroups, 1);
}
//----------------------------------------------------------------------------

std::string const Fluid2InitializeState::msHLSLInitializeStateString =
"Texture2D<float> density;\n"
"Texture2D<float2> velocity;\n"
"RWTexture2D<float4> stateTm1;\n"
"RWTexture2D<float4> stateT;\n"
"\n"
"[numthreads(NUM_X_THREADS, NUM_Y_THREADS, 1)]\n"
"void CSMain(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    float4 initial = float4(velocity[c], 0.0f, density[c]);\n"
"    stateTm1[c] = initial;\n"
"    stateT[c] = initial;\n"
"}\n";
