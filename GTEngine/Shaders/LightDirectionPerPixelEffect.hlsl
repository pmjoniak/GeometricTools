// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.7.0 (2014/12/04)

cbuffer PVWMatrix
{
    float4x4 pvwMatrix;
};

struct VS_INPUT
{
    float3 modelPosition : POSITION;
    float3 modelNormal : NORMAL;
};

struct VS_OUTPUT
{
    float3 vertexPosition : TEXCOORD0;
    float3 vertexNormal : TEXCOORD1;
    float4 clipPosition : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    output.vertexPosition = input.modelPosition;
    output.vertexNormal = input.modelNormal;
#if GTE_USE_MAT_VEC
    output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));
#else
    output.clipPosition = mul(float4(input.modelPosition, 1.0f), pvwMatrix);
#endif
    return output;
}

cbuffer Lighting
{
    float4 materialEmissive;
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float4 lightSpotCutoff;
    float4 lightAttenuation;
    float4 cameraModelPosition;
    float4 lightModelPosition;
    float4 lightModelDirection;
    float4x4 wMatrix;
};

struct PS_INPUT
{
    float3 vertexPosition : TEXCOORD0;
    float3 vertexNormal : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 pixelColor0 : SV_TARGET0;
};

PS_OUTPUT PSMain(PS_INPUT input)
{
    PS_OUTPUT output;

    float3 normal = normalize(input.vertexNormal);
    float NDotL = -dot(normal, lightModelDirection.xyz);
    float3 viewVector = normalize(cameraModelPosition.xyz - input.vertexPosition);
    float3 halfVector = normalize(viewVector - lightModelDirection.xyz);
    float NDotH = dot(normal, halfVector);
    float4 lighting = lit(NDotL, NDotH, materialSpecular.a);

    float3 color = materialAmbient.rgb*lightAmbient.rgb +
        lighting.y*materialDiffuse.rgb*lightDiffuse.rgb +
        lighting.z*materialSpecular.rgb*lightSpecular.rgb;

    output.pixelColor0.rgb = materialEmissive.rgb + lightAttenuation.w*color;
    output.pixelColor0.a = materialDiffuse.a;
    return output;
}
