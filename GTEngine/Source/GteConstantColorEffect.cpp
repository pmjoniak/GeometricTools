// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteConstantColorEffect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
ConstantColorEffect::ConstantColorEffect(Vector4<float> const& color)
{
    // Create the shader constants.
    mPVWMatrixConstant.reset(new ConstantBuffer(sizeof(Matrix4x4<float>),
        true));
    mPVWMatrix = mPVWMatrixConstant->Get<Matrix4x4<float>>();
    *mPVWMatrix = Matrix4x4<float>::Identity();

    mColorConstant.reset(new ConstantBuffer(sizeof(Vector4<float>), true));
    mColor = mColorConstant->Get<Vector4<float>>();
    *mColor = color;

    // Create the vertex color effect shaders.
    mVShader.reset(ShaderFactory::CreateVertex("VSMain", msHLSLString));
    if (mVShader)
    {
        mVShader->Set("PVWMatrix", mPVWMatrixConstant);
        mVShader->Set("ConstantColor", mColorConstant);
    }
    else
    {
        LogError("Unable to create vertex shader.");
    }

    mPShader.reset(ShaderFactory::CreatePixel("PSMain", msHLSLString));
    LogAssert(mPShader, "Unable to create pixel shader.");
}
//----------------------------------------------------------------------------
void ConstantColorEffect::SetName(std::string const& name)
{
    VisualEffect::SetName(name);
    mPVWMatrixConstant->SetName(name);
}
//----------------------------------------------------------------------------

std::string const ConstantColorEffect::msHLSLString =
"cbuffer PVWMatrix\n"
"{\n"
"    float4x4 pvwMatrix;\n"
"};\n"
"\n"
"cbuffer ConstantColor\n"
"{\n"
"    float4 constantColor;\n"
"};\n"
"\n"
"struct VS_INPUT\n"
"{\n"
"    float3 modelPosition : POSITION;\n"
"};\n"
"\n"
"struct VS_OUTPUT\n"
"{\n"
"    float4 vertexColor : COLOR0;\n"
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
"    output.vertexColor = constantColor;\n"
"    return output;\n"
"}\n"
"\n"
"struct PS_INPUT\n"
"{\n"
"    float4 vertexColor : COLOR0;\n"
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
"    output.pixelColor0 = input.vertexColor;\n"
"    return output;\n"
"}\n";
