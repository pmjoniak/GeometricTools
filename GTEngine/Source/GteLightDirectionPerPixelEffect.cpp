// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.2 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteLightDirectionPerPixelEffect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
LightDirectionPerPixelEffect::~LightDirectionPerPixelEffect()
{
}
//----------------------------------------------------------------------------
LightDirectionPerPixelEffect::LightDirectionPerPixelEffect(
    LightingConstants const& lighting)
    :
    LightingEffect(lighting, msHLSLString, false)
{
}
//----------------------------------------------------------------------------

std::string const LightDirectionPerPixelEffect::msHLSLString =
"cbuffer PVWMatrix\n"
"{\n"
"    float4x4 pvwMatrix;\n"
"};\n"
"\n"
"struct VS_INPUT\n"
"{\n"
"    float3 modelPosition : POSITION;\n"
"    float3 modelNormal : NORMAL;\n"
"};\n"
"\n"
"struct VS_OUTPUT\n"
"{\n"
"    float3 vertexPosition : TEXCOORD0;\n"
"    float3 vertexNormal : TEXCOORD1;\n"
"    float4 clipPosition : SV_POSITION;\n"
"};\n"
"\n"
"VS_OUTPUT VSMain(VS_INPUT input)\n"
"{\n"
"    VS_OUTPUT output;\n"
"\n"
"    output.vertexPosition = input.modelPosition;\n"
"    output.vertexNormal = input.modelNormal;\n"
"#if GTE_USE_MAT_VEC\n"
"    output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));\n"
"#else\n"
"    output.clipPosition = mul(float4(input.modelPosition, 1.0f), pvwMatrix);\n"
"#endif\n"
"    return output;\n"
"}\n"
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
"struct PS_INPUT\n"
"{\n"
"    float3 vertexPosition : TEXCOORD0;\n"
"    float3 vertexNormal : TEXCOORD1;\n"
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
"\n"
"    float3 normal = normalize(input.vertexNormal);\n"
"    float NDotL = -dot(normal, lightModelDirection.xyz);\n"
"    float3 viewVector = normalize(cameraModelPosition.xyz - input.vertexPosition);\n"
"    float3 halfVector = normalize(viewVector - lightModelDirection.xyz);\n"
"    float NDotH = dot(normal, halfVector);\n"
"    float4 lighting = lit(NDotL, NDotH, materialSpecular.a);\n"
"\n"
"    float3 color = materialAmbient.rgb*lightAmbient.rgb +\n"
"        lighting.y*materialDiffuse.rgb*lightDiffuse.rgb +\n"
"        lighting.z*materialSpecular.rgb*lightSpecular.rgb;\n"
"\n"
"    output.pixelColor0.rgb = materialEmissive.rgb + lightAttenuation.w*color;\n"
"    output.pixelColor0.a = materialDiffuse.a;\n"
"    return output;\n"
"}\n";
