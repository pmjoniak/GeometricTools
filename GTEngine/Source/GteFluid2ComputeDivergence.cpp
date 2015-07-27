// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteFluid2ComputeDivergence.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
Fluid2ComputeDivergence::~Fluid2ComputeDivergence()
{
}
//----------------------------------------------------------------------------
Fluid2ComputeDivergence::Fluid2ComputeDivergence(int xSize, int ySize,
    int numXThreads, int numYThreads,
    std::shared_ptr<ConstantBuffer> const& parameters)
    :
    mNumXGroups(xSize/numXThreads),
    mNumYGroups(ySize/numYThreads)
{
    HLSLDefiner definer;
    definer.SetInt("NUM_X_THREADS", numXThreads);
    definer.SetInt("NUM_Y_THREADS", numYThreads);

    // Create the shader for computing the divergence of the velocity.
    mComputeDivergence.reset(ShaderFactory::CreateCompute("ComputeDivergence",
        msHLSLComputeDivergenceString, definer));
    LogAssert(mComputeDivergence != nullptr, "Cannot create compute shader.");

    mDivergence.reset(new Texture2(DF_R32_FLOAT, xSize, ySize));
    mDivergence->SetUsage(Resource::SHADER_OUTPUT);

    mComputeDivergence->Set("Parameters", parameters);
    mComputeDivergence->Set("divergence", mDivergence);
}
//----------------------------------------------------------------------------
std::shared_ptr<Texture2> const& Fluid2ComputeDivergence::GetDivergence()
    const
{
    return mDivergence;
}
//----------------------------------------------------------------------------
void Fluid2ComputeDivergence::Execute(DX11Engine* engine,
    std::shared_ptr<Texture2> const& state)
{
    mComputeDivergence->Set("state", state);
    engine->Execute(mComputeDivergence, mNumXGroups, mNumYGroups, 1);
}
//----------------------------------------------------------------------------

std::string const Fluid2ComputeDivergence::msHLSLComputeDivergenceString =
"cbuffer Parameters\n"
"{\n"
"    float4 spaceDelta;    // (dx, dy, 0, 0)\n"
"    float4 halfDivDelta;  // (0.5/dx, 0.5/dy, 0, 0)\n"
"    float4 timeDelta;     // (dt, dt/dx, dt/dy, 0)\n"
"    float4 viscosityX;    // (velVX, velVX, 0, denVX)\n"
"    float4 viscosityY;    // (velVX, velVY, 0, denVY)\n"
"    float4 epsilon;       // (epsilonX, epsilonY, 0, epsilon0)\n"
"};\n"
"\n"
"Texture2D<float4> state;\n"
"RWTexture2D<float> divergence;\n"
"\n"
"[numthreads(NUM_X_THREADS, NUM_Y_THREADS, 1)]\n"
"void CSMain(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    state.GetDimensions(dim.x, dim.y);\n"
"\n"
"    int x = int(c.x);\n"
"    int y = int(c.y);\n"
"    int xm = max(x - 1, 0);\n"
"    int xp = min(x + 1, dim.x - 1);\n"
"    int ym = max(y - 1, 0);\n"
"    int yp = min(y + 1, dim.y - 1);\n"
"\n"
"    float2 velocityGradient = float2(\n"
"        state[int2(xp, y)].x - state[int2(xm, y)].x,\n"
"        state[int2(x, yp)].y - state[int2(x, ym)].y\n"
"    );\n"
"\n"
"    divergence[c] = dot(halfDivDelta.xy, velocityGradient);\n"
"}\n";
