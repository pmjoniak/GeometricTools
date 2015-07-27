// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteFluid2UpdateState.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
Fluid2UpdateState::~Fluid2UpdateState()
{
}
//----------------------------------------------------------------------------
Fluid2UpdateState::Fluid2UpdateState(int xSize, int ySize, int numXThreads,
    int numYThreads, std::shared_ptr<ConstantBuffer> const& parameters)
    :
    mNumXGroups(xSize/numXThreads),
    mNumYGroups(ySize/numYThreads)
{
    HLSLDefiner definer;
    definer.SetInt("NUM_X_THREADS", numXThreads);
    definer.SetInt("NUM_Y_THREADS", numYThreads);

    // Create the shader for generating velocity from vortices.
    mComputeUpdateState.reset(ShaderFactory::CreateCompute("UpdateState",
        msHLSLUpdateStateString, definer));
    LogAssert(mComputeUpdateState != nullptr, "Cannot create compute shader.");

    mUpdateState.reset(new Texture2(DF_R32G32B32A32_FLOAT, xSize, ySize));
    mUpdateState->SetUsage(Resource::SHADER_OUTPUT);

    mAdvectionSampler.reset(new SamplerState());
    mAdvectionSampler->filter = SamplerState::MIN_L_MAG_L_MIP_P;
    mAdvectionSampler->mode[0] = SamplerState::CLAMP;
    mAdvectionSampler->mode[1] = SamplerState::CLAMP;

    mComputeUpdateState->Set("Parameters", parameters);
    mComputeUpdateState->Set("advectionSampler", mAdvectionSampler);
    mComputeUpdateState->Set("updateState", mUpdateState);
}
//----------------------------------------------------------------------------
std::shared_ptr<Texture2> const& Fluid2UpdateState::GetUpdateState() const
{
    return mUpdateState;
}
//----------------------------------------------------------------------------
void Fluid2UpdateState::Execute(DX11Engine* engine,
    std::shared_ptr<Texture2> const& source,
    std::shared_ptr<Texture2> const& stateTm1,
    std::shared_ptr<Texture2> const& stateT)
{
    mComputeUpdateState->Set("source", source);
    mComputeUpdateState->Set("stateTm1", stateTm1);
    mComputeUpdateState->Set("stateT", stateT);
    engine->Execute(mComputeUpdateState, mNumXGroups, mNumYGroups, 1);
}
//----------------------------------------------------------------------------

std::string const Fluid2UpdateState::msHLSLUpdateStateString =
"cbuffer Parameters\n"
"{\n"
"    float4 spaceDelta;    // (dx, dy, 0, 0)\n"
"    float4 halfDivDelta;  // (0.5/dx, 0.5/dy, 0, 0)\n"
"    float4 timeDelta;     // (dt/dx, dt/dy, 0, dt)\n"
"    float4 viscosityX;    // (velVX, velVX, 0, denVX)\n"
"    float4 viscosityY;    // (velVX, velVY, 0, denVY)\n"
"    float4 epsilon;       // (epsilonX, epsilonY, 0, epsilon0)\n"
"};\n"
"\n"
"Texture2D<float4> source;\n"
"Texture2D<float4> stateTm1;\n"
"Texture2D<float4> stateT;\n"
"SamplerState advectionSampler;  // bilinear, clamp\n"
"RWTexture2D<float4> updateState;\n"
"\n"
"[numthreads(NUM_X_THREADS, NUM_Y_THREADS, 1)]\n"
"void CSMain(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    stateT.GetDimensions(dim.x, dim.y);\n"
"\n"
"    int x = int(c.x);\n"
"    int y = int(c.y);\n"
"    int xm = max(x - 1, 0);\n"
"    int xp = min(x + 1, dim.x - 1);\n"
"    int ym = max(y - 1, 0);\n"
"    int yp = min(y + 1, dim.y - 1);\n"
"\n"
"    // Sample states at (x,y), (x+dx,y), (x-dx,y), (x,y+dy), (x,y-dy).\n"
"    float4 stateZZ = stateT[int2(x, y)];\n"
"    float4 statePZ = stateT[int2(xp, y)];\n"
"    float4 stateMZ = stateT[int2(xm, y)];\n"
"    float4 stateZP = stateT[int2(x, yp)];\n"
"    float4 stateZM = stateT[int2(x, ym)];\n"
"\n"
"    // Sample the source state at (x,y).\n"
"    float4 src = source[int2(x, y)];\n"
"\n"
"    // Estimate second-order derivatives of state at (x,y).\n"
"    float4 stateDXX = statePZ - 2.0f*stateZZ + stateMZ;\n"
"    float4 stateDYY = stateZP - 2.0f*stateZZ + stateZM;\n"
"\n"
"    // Compute advection.\n"
"    float2 tcd = spaceDelta.xy*(c - timeDelta.xy*stateZZ.xy + 0.5f);\n"
"    float4 advection = stateTm1.SampleLevel(advectionSampler, tcd, 0.0f);\n"
"\n"
"    // Update the state.\n"
"    updateState[c] = advection +\n"
"        (viscosityX*stateDXX + viscosityY*stateDYY + timeDelta.w*src);\n"
"}\n";
