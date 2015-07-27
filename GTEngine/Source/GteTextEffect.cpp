// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.3 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteTextEffect.h"
#include "GteShaderFactory.h"
#include "GteVector2.h"
using namespace gte;

//----------------------------------------------------------------------------
TextEffect::TextEffect(std::shared_ptr<Texture2> const& texture)
{
    mTranslate.reset(new ConstantBuffer(sizeof(Vector2<float>), true));
    mColor.reset(new ConstantBuffer(sizeof(Vector4<float>), true));
    mSamplerState.reset(new SamplerState());

    // Create the text shaders.
    mVShader.reset(ShaderFactory::CreateVertex("VSMain", msHLSLString));
    if (mVShader)
    {
        SetTranslate(0.0f, 0.0f);
        mVShader->Set("Translate", mTranslate);
    }
    else
    {
        LogError("Unable to create vertex shader.");
    }

    mPShader.reset(ShaderFactory::CreatePixel("PSMain", msHLSLString));
    if (mPShader)
    {
        SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
        mPShader->Set("TextColor", mColor);
        mPShader->Set("baseTexture", texture);
        mPShader->Set("baseSampler", mSamplerState);
    }
    else
    {
        LogError("Unable to create pixel shader.");
    }
}
//----------------------------------------------------------------------------
std::shared_ptr<ConstantBuffer> const& TextEffect::GetTranslate() const
{
    return mTranslate;
}
//----------------------------------------------------------------------------
std::shared_ptr<ConstantBuffer> const& TextEffect::GetColor() const
{
    return mColor;
}
//----------------------------------------------------------------------------
void TextEffect::SetTranslate(float x, float  y)
{
    float* data = mTranslate->Get<float>();
    data[0] = x;
    data[1] = y;
}
//----------------------------------------------------------------------------
void TextEffect::SetColor(Vector4<float> const& color)
{
    Vector4<float>* data = mColor->Get<Vector4<float>>();
    *data = color;
}
//----------------------------------------------------------------------------
void TextEffect::SetName(std::string const& name)
{
    VisualEffect::SetName(name);
    mTranslate->SetName(mName + ":mTranslate");
    mColor->SetName(mName + ":mColor");
    mSamplerState->SetName(mName + ":mSamplerState");
}
//----------------------------------------------------------------------------

std::string const TextEffect::msHLSLString =
"cbuffer Translate\n"
"{\n"
"    float2 translate;\n"
"};\n"
"struct VS_INPUT\n"
"{\n"
"    float2 modelPosition : POSITION;\n"
"    float2 modelTCoord : TEXCOORD0;\n"
"};\n"
"\n"
"struct VS_OUTPUT\n"
"{\n"
"    float2 vertexTCoord : TEXCOORD0;\n"
"    float4 clipPosition : SV_POSITION;\n"
"};\n"
"\n"
"VS_OUTPUT VSMain (VS_INPUT input)\n"
"{\n"
"    VS_OUTPUT output;\n"
"    output.vertexTCoord = input.modelTCoord;\n"
"    output.clipPosition.x = 2.0f*input.modelPosition.x - 1.0f + 2.0f*translate.x;\n"
"    output.clipPosition.y = 2.0f*input.modelPosition.y - 1.0f + 2.0f*translate.y;\n"
"    output.clipPosition.z = 0.0f;\n"
"    output.clipPosition.w = 1.0f;\n"
"    return output;\n"
"}\n"
"\n"
"cbuffer TextColor\n"
"{\n"
"    float4 textColor;\n"
"};\n"
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
"    float bitmapAlpha = baseTexture.Sample(baseSampler, input.vertexTCoord).r;\n"
"    if (bitmapAlpha > 0.5f)\n"
"    {\n"
"        discard;\n"
"    }\n"
"    output.pixelColor0 = textColor;\n"
"    return output;\n"
"}\n";
