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

struct VS_INPUT
{
    float3 modelPosition : POSITION;
    float3 modelNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 vertexColor : COLOR0;
    float4 clipPosition : SV_POSITION;
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 lighting;
    float3 modelLightDiff = input.modelPosition - lightModelPosition.xyz;
    float3 vertexDirection = normalize(modelLightDiff);
    float vertexCosAngle = dot(lightModelDirection.xyz, vertexDirection);
    if (vertexCosAngle >= lightSpotCutoff.y)
    {
        float NDotL = -dot(input.modelNormal, vertexDirection);
        float3 viewVector = normalize(cameraModelPosition.xyz - input.modelPosition);
        float3 halfVector = normalize(viewVector - vertexDirection);
        float NDotH = dot(input.modelNormal, halfVector);
        lighting = lit(NDotL, NDotH, materialSpecular.a);
        lighting.w = pow(abs(vertexCosAngle), lightSpotCutoff.w);
    }
    else
    {
        lighting = float4(1.0f, 0.0f, 0.0f, 0.0f);
    }

#if GTE_USE_MAT_VEC
    float4 worldLightDiff = mul(wMatrix, float4(modelLightDiff, 0.0f));
#else
    float4 worldLightDiff = mul(float4(modelLightDiff, 0.0f), wMatrix);
#endif
    float distance = length(worldLightDiff.xyz);
    float attenuation = lightAttenuation.w / (lightAttenuation.x + distance *
        (lightAttenuation.y + distance*lightAttenuation.z));

    float3 color = materialAmbient.rgb*lightAmbient.rgb + lighting.w*(
        lighting.y*materialDiffuse.rgb*lightDiffuse.rgb +
        lighting.z*materialSpecular.rgb*lightSpecular.rgb);

    output.vertexColor.rgb = materialEmissive.rgb + attenuation*color;
    output.vertexColor.a = materialDiffuse.a;
#if GTE_USE_MAT_VEC
    output.clipPosition = mul(pvwMatrix, float4(input.modelPosition, 1.0f));
#else
    output.clipPosition = mul(float4(input.modelPosition, 1.0f), pvwMatrix);
#endif
    return output;
}

struct PS_INPUT
{
    float4 vertexColor : COLOR0;
};

struct PS_OUTPUT
{
    float4 pixelColor0 : SV_TARGET0;
};

PS_OUTPUT PSMain(PS_INPUT input)
{
    PS_OUTPUT output;
    output.pixelColor0 = input.vertexColor;
    return output;
}
