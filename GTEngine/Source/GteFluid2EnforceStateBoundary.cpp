// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteFluid2EnforceStateBoundary.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
Fluid2EnforceStateBoundary::~Fluid2EnforceStateBoundary()
{
}
//----------------------------------------------------------------------------
Fluid2EnforceStateBoundary::Fluid2EnforceStateBoundary(int xSize, int ySize,
    int numXThreads, int numYThreads)
    :
    mNumXGroups(xSize/numXThreads),
    mNumYGroups(ySize/numYThreads)
{
    mXMin.reset(new Texture1(DF_R32_FLOAT, ySize));
    mXMin->SetUsage(Resource::SHADER_OUTPUT);
    mXMax.reset(new Texture1(DF_R32_FLOAT, ySize));
    mXMax->SetUsage(Resource::SHADER_OUTPUT);
    mYMin.reset(new Texture1(DF_R32_FLOAT, xSize));
    mYMin->SetUsage(Resource::SHADER_OUTPUT);
    mYMax.reset(new Texture1(DF_R32_FLOAT, xSize));
    mYMax->SetUsage(Resource::SHADER_OUTPUT);

    HLSLDefiner definer;
    definer.SetInt("USE_COPY_X_EDGE", 1);
    definer.SetInt("NUM_Y_THREADS", numYThreads);
    mCopyXEdge.reset(ShaderFactory::CreateCompute("CopyXEdge",
        msHLSLEnforceStateBoundaryString, definer, "CopyXEdge"));
    mCopyXEdge->Set("xMin", mXMin);
    mCopyXEdge->Set("xMax", mXMax);

    definer.Clear();
    definer.SetInt("USE_WRITE_X_EDGE", 1);
    definer.SetInt("NUM_Y_THREADS", numYThreads);
    mWriteXEdge.reset(ShaderFactory::CreateCompute("WriteXEdge",
        msHLSLEnforceStateBoundaryString, definer, "WriteXEdge"));
    mWriteXEdge->Set("xMin", mXMin);
    mWriteXEdge->Set("xMax", mXMax);

    definer.Clear();
    definer.SetInt("USE_COPY_Y_EDGE", 1);
    definer.SetInt("NUM_X_THREADS", numXThreads);
    mCopyYEdge.reset(ShaderFactory::CreateCompute("CopyYEdge",
        msHLSLEnforceStateBoundaryString, definer, "CopyYEdge"));
    mCopyYEdge->Set("yMin", mYMin);
    mCopyYEdge->Set("yMax", mYMax);

    definer.Clear();
    definer.SetInt("USE_WRITE_Y_EDGE", 1);
    definer.SetInt("NUM_X_THREADS", numXThreads);
    mWriteYEdge.reset(ShaderFactory::CreateCompute("WriteYEdge",
        msHLSLEnforceStateBoundaryString, definer, "WriteYEdge"));
    mWriteYEdge->Set("yMin", mYMin);
    mWriteYEdge->Set("yMax", mYMax);
}
//----------------------------------------------------------------------------
void Fluid2EnforceStateBoundary::Execute(DX11Engine* engine,
    std::shared_ptr<Texture2> const& state)
{
    // in: state
    // out: mXMin, mXMax
    mCopyXEdge->Set("state", state);
    engine->Execute(mCopyXEdge, 1, mNumYGroups, 1);

    // in: mXMin, mXMax
    // out: state
    mWriteXEdge->Set("state", state);
    engine->Execute(mWriteXEdge, 1, mNumYGroups, 1);

    // in: state
    // out: mYMin, mYMax
    mCopyYEdge->Set("state", state);
    engine->Execute(mCopyYEdge, mNumXGroups, 1, 1);

    // in: mYMin, mYMax
    // out: state
    mWriteYEdge->Set("state", state);
    engine->Execute(mWriteYEdge, mNumXGroups, 1, 1);
}
//----------------------------------------------------------------------------

std::string const
Fluid2EnforceStateBoundary::msHLSLEnforceStateBoundaryString =
"#if USE_COPY_X_EDGE\n"
"Texture2D<float4> state;\n"
"RWTexture1D<float> xMin;\n"
"RWTexture1D<float> xMax;\n"
"\n"
"[numthreads(1, NUM_Y_THREADS, 1)]\n"
"void CopyXEdge(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    state.GetDimensions(dim.x, dim.y);\n"
"    xMin[c.y] = state[uint2(1, c.y)].y;\n"
"    xMax[c.y] = state[uint2(dim.x - 2, c.y)].y;\n"
"}\n"
"#endif\n"
"\n"
"#if USE_WRITE_X_EDGE\n"
"Texture1D<float> xMin;\n"
"Texture1D<float> xMax;\n"
"RWTexture2D<float4> state;\n"
"\n"
"[numthreads(1, NUM_Y_THREADS, 1)]\n"
"void WriteXEdge(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    state.GetDimensions(dim.x, dim.y);\n"
"    state[uint2(0, c.y)] = float4(0.0f, xMin[c.y], 0.0f, 0.0f);\n"
"    state[uint2(dim.x - 1, c.y)] = float4(0.0f, xMax[c.y], 0.0f, 0.0f);\n"
"}\n"
"#endif\n"
"\n"
"#if USE_COPY_Y_EDGE\n"
"Texture2D<float4> state;\n"
"RWTexture1D<float> yMin;\n"
"RWTexture1D<float> yMax;\n"
"\n"
"[numthreads(NUM_X_THREADS, 1, 1)]\n"
"void CopyYEdge(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    state.GetDimensions(dim.x, dim.y);\n"
"    yMin[c.x] = state[uint2(c.x, 1)].x;\n"
"    yMax[c.x] = state[uint2(c.x, dim.y - 2)].x;\n"
"}\n"
"#endif\n"
"\n"
"#if USE_WRITE_Y_EDGE\n"
"Texture1D<float> yMin;\n"
"Texture1D<float> yMax;\n"
"RWTexture2D<float4> state;\n"
"\n"
"[numthreads(NUM_X_THREADS, 1, 1)]\n"
"void WriteYEdge(uint2 c : SV_DispatchThreadID)\n"
"{\n"
"    uint2 dim;\n"
"    state.GetDimensions(dim.x, dim.y);\n"
"    state[uint2(c.x, 0)] = float4(yMin[c.x], 0.0f, 0.0f, 0.0f);\n"
"    state[uint2(c.x, dim.y - 1)] = float4(yMax[c.x], 0.0f, 0.0f, 0.0f);\n"
"}\n"
"#endif\n";
