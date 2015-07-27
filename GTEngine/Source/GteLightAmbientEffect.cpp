// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.2 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteLightAmbientEffect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
LightAmbientEffect::~LightAmbientEffect()
{
}
//----------------------------------------------------------------------------
LightAmbientEffect::LightAmbientEffect(LightingConstants const& lighting)
    :
    LightingEffect(lighting, msHLSLString, true)
{
}
//----------------------------------------------------------------------------

std::string const LightAmbientEffect::msHLSLString =
"cbuffer PVWMatrix\n"
"{\n"
"    float4x4 pvwMatrix;\n"
"};\n"
"\n"
"cbuffer Lighting\n"
"{\n"
"    float4 materialEmissive;\n"
"    float4 materialAmbient;\n"
"    float4 materialDiffuse;\n"
"    float4 materialSpecular;\n"
"    float4 lightAmbient;\n"
"    float4 lightDiffuse;\n"
"    float4 lightSpecular;\n"
"    float4 lightSpotCutoff;\n"
"    float4 lightAttenuation;\n"
"    float4 cameraModelPosition;\n"
"    float4 lightModelPosition;\n"
"    float4 lightModelDirection;\n"
"    float4x4 wMatrix;\n"
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
"\n"
"    float3 ambient = lightAttenuation.w*lightAmbient.rgb;\n"
"    output.vertexColor.rgb = materialEmissive.rgb + materialAmbient.rgb*ambient;\n"
"    output.vertexColor.a = 1.0f;\n"
"#if GTE_USE_MAT_VEC\n"
"    output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));\n"
"#else\n"
"    output.clipPosition = mul(float4(input.modelPosition, 1.0f), pvwMatrix);\n"
"#endif\n"
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
