// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteTexture2Effect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
Texture2Effect::Texture2Effect(std::shared_ptr<Texture2> const& texture,
    SamplerState::Filter filter, SamplerState::Mode mode0,
    SamplerState::Mode mode1)
    :
    mTexture(texture)
{
    // Create the shader constant.
    mPVWMatrixConstant.reset(new ConstantBuffer(sizeof(Matrix4x4<float>),
        true));
    mPVWMatrix = mPVWMatrixConstant->Get<Matrix4x4<float>>();
    *mPVWMatrix = Matrix4x4<float>::Identity();

    // Create the texture sampler.
    mSampler.reset(new SamplerState());
    mSampler->filter = filter;
    mSampler->mode[0] = mode0;
    mSampler->mode[1] = mode1;

    // Create the texture effect shaders.
    mVShader.reset(ShaderFactory::CreateVertex("VSMain", msHLSLString));
    if (mVShader)
    {
        mVShader->Set("PVWMatrix", mPVWMatrixConstant);
    }
    else
    {
        LogError("Unable to create vertex shader.");
    }

    mPShader.reset(ShaderFactory::CreatePixel("PSMain", msHLSLString));
    if (mPShader)
    {
        mPShader->Set("baseTexture", mTexture);
        mPShader->Set("baseSampler", mSampler);
    }
    else
    {
        LogError("Unable to create pixel shader.");
    }
}
//----------------------------------------------------------------------------
void Texture2Effect::SetName(std::string const& name)
{
    VisualEffect::SetName(name);
    mPVWMatrixConstant->SetName(name);
    mSampler->SetName(name);
}
//----------------------------------------------------------------------------

std::string const Texture2Effect::msHLSLString =
"cbuffer PVWMatrix\n"
"{\n"
"    float4x4 pvwMatrix;\n"
"};\n"
"\n"
"struct VS_INPUT\n"
"{\n"
"    float3 modelPosition : POSITION;\n"
"    float2 modelTCoord : TEXCOORD0;\n"
"};\n"
"\n"
"struct VS_OUTPUT\n"
"{\n"
"    float2 vertexTCoord : TEXCOORD0;\n"
"    float4 clipPosition : SV_POSITION;\n"
"};\n"
"\n"
"VS_OUTPUT VSMain(VS_INPUT input)\n"
"{\n"
"    VS_OUTPUT output;\n"
"#if GTE_USE_MAT_VEC\n"
"    output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));\n"
"#else\n"
"    output.clipPosition = mul(float4(input.modelPosition, 1.0f), pvwMatrix);\n"
"#endif\n"
"    output.vertexTCoord = input.modelTCoord;\n"
"    return output;\n"
"}\n"
"\n"
"Texture2D baseTexture;\n"
"SamplerState baseSampler;\n"
"\n"
"struct PS_INPUT\n"
"{\n"
"    float2 vertexTCoord : TEXCOORD0;\n"
"};\n"
"\n"
"struct PS_OUTPUT\n"
"{\n"
"    float4 pixelColor0 : SV_TARGET0;\n"
"};\n"
"\n"
"PS_OUTPUT PSMain(PS_INPUT input)\n"
"{\n"
"    PS_OUTPUT output;\n"
"    output.pixelColor0 = baseTexture.Sample(baseSampler, input.vertexTCoord);\n"
"    return output;\n"
"}\n";
