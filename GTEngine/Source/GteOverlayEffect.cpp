// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.3 (2014/12/04)

#include "GTEnginePCH.h"
#include "GteOverlayEffect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
OverlayEffect::OverlayEffect(int windowWidth, int windowHeight,
    int textureWidth, int textureHeight, SamplerState::Filter filter,
    SamplerState::Mode mode0, SamplerState::Mode mode1, bool useColorPShader)
    :
    mWindowWidth(static_cast<float>(windowWidth)),
    mWindowHeight(static_cast<float>(windowHeight))
{
    Initialize(windowWidth, windowHeight, textureWidth, textureHeight);

    if (useColorPShader)
    {
        mPShader.reset(ShaderFactory::CreatePixel("PSMain",
            msColorPShaderString));
    }
    else
    {
        mPShader.reset(ShaderFactory::CreatePixel("PSMain",
            msGrayPShaderString));
    }

    if (mPShader)
    {
        std::shared_ptr<SamplerState> sampler(new SamplerState());
        sampler->filter = filter;
        sampler->mode[0] = mode0;
        sampler->mode[1] = mode1;
        mPShader->Set("imageSampler", sampler);
    }
    else
    {
        LogError("Unable to create pixel shader.");
    }

    mEffect.reset(new VisualEffect(mVShader, mPShader));
}
//----------------------------------------------------------------------------
OverlayEffect::OverlayEffect(int windowWidth, int windowHeight,
    int textureWidth, int textureHeight,
    std::shared_ptr<PixelShader> const& pshader)
    :
    mWindowWidth(static_cast<float>(windowWidth)),
    mWindowHeight(static_cast<float>(windowHeight)),
    mPShader(pshader)
{
    LogAssert(mPShader, "Pixel shader is null.");
    Initialize(windowWidth, windowHeight, textureWidth, textureHeight);
    mEffect.reset(new VisualEffect(mVShader, mPShader));
}
//----------------------------------------------------------------------------
void OverlayEffect::SetTextureRectangle(std::array<int, 4> const& rectangle)
{
    mTextureRectangle = rectangle;
    UpdateVertexBuffer();
}
//----------------------------------------------------------------------------
void OverlayEffect::SetRectangles(std::array<int, 4> const& overlayRectangle,
    std::array<int, 4> const& textureRectangle)
{
    mOverlayRectangle = overlayRectangle;
    mTextureRectangle = textureRectangle;
    UpdateVertexBuffer();
}
//----------------------------------------------------------------------------
bool OverlayEffect::Contains(int x, int y) const
{
    return mOverlayRectangle[0] <= x
        && x < mOverlayRectangle[0] + mOverlayRectangle[2]
        && mOverlayRectangle[1] <= y
        && y < mOverlayRectangle[1] + mOverlayRectangle[3];
}
//----------------------------------------------------------------------------
void OverlayEffect::SetTexture(std::shared_ptr<Texture2> const& texture)
{
    if (texture)
    {
        mEffect->GetPixelShader()->Set("imageTexture", texture);
    }
}
//----------------------------------------------------------------------------
void OverlayEffect::SetTexture(std::string const& textureName,
    std::shared_ptr<Texture2> const& texture)
{
    if (texture)
    {
        mEffect->GetPixelShader()->Set(textureName, texture);
    }
}
//----------------------------------------------------------------------------
void OverlayEffect::SetName(std::string const& name)
{
    mName = name;
    mVBuffer->SetName(mName + ":mVBuffer");
    mIBuffer->SetName(mName + ":mIBuffer");
    mVShader->SetName(mName + ":mVShader");
    mPShader->SetName(mName + ":mPShader");
    mPShader->SetName(mName + ":mEffect");
}
//----------------------------------------------------------------------------
void OverlayEffect::Initialize(int windowWidth, int windowHeight,
    int textureWidth, int textureHeight)
{
    if (windowWidth <= 0 || windowHeight <= 0
    ||  textureWidth <= 0 || textureHeight <= 0)
    {
        LogError("Invalid input rectangle.");

        // Use dummy sizes.
        windowWidth = 1;
        windowHeight = 1;
        textureWidth = 1;
        textureHeight = 1;
    }

    mInvTextureWidth = 1.0f/static_cast<float>(textureWidth);
    mInvTextureHeight = 1.0f/static_cast<float>(textureHeight);

    mOverlayRectangle[0] = 0;
    mOverlayRectangle[1] = 0;
    mOverlayRectangle[2] = windowWidth;
    mOverlayRectangle[3] = windowHeight;

    mTextureRectangle[0] = 0;
    mTextureRectangle[1] = 0;
    mTextureRectangle[2] = textureWidth;
    mTextureRectangle[3] = textureHeight;

    // Create the vertex buffer.
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);
    mVBuffer.reset(new VertexBuffer(vformat, 4));
    mVBuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    UpdateVertexBuffer();

    // Create the index buffer.
    mIBuffer.reset(new IndexBuffer(IP_TRIMESH, 2, sizeof(unsigned int)));
    unsigned int* indices = mIBuffer->Get<unsigned int>();
    indices[0] = 0;  indices[1] = 2;  indices[2] = 3;
    indices[3] = 0;  indices[4] = 3;  indices[5] = 1;

    // Create the vertex shader.
    mVShader.reset(ShaderFactory::CreateVertex("VSMain", msVShaderString));
    LogAssert(mVShader, "Unable to create vertex shader.");
}
//----------------------------------------------------------------------------
void OverlayEffect::UpdateVertexBuffer()
{
    // Convert to normalized coordinates.
    float invWindowWidth = 1.0f/mWindowWidth;
    float invWindowHeight = 1.0f/mWindowHeight;
    float px = static_cast<float>(mOverlayRectangle[0])*invWindowWidth;
    float py = static_cast<float>(mOverlayRectangle[1])*invWindowHeight;
    float pw = static_cast<float>(mOverlayRectangle[2])*invWindowWidth;
    float ph = static_cast<float>(mOverlayRectangle[3])*invWindowHeight;

    float tx = static_cast<float>(mTextureRectangle[0])*mInvTextureWidth;
    float ty = static_cast<float>(mTextureRectangle[1])*mInvTextureHeight;
    float tw = static_cast<float>(mTextureRectangle[2])*mInvTextureWidth;
    float th = static_cast<float>(mTextureRectangle[3])*mInvTextureHeight;

    Vertex* vertex = mVBuffer->Get<Vertex>();
    vertex[0].position = { px, py };
    vertex[0].tcoord = { tx, ty };
    vertex[1].position = { px + pw, py };
    vertex[1].tcoord = { tx + tw, ty };
    vertex[2].position = { px, py + ph };
    vertex[2].tcoord = { tx, ty + th };
    vertex[3].position = { px + pw, py + ph };
    vertex[3].tcoord = { tx + tw, ty + th };
}
//----------------------------------------------------------------------------

std::string const OverlayEffect::msVShaderString =
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
"    output.clipPosition.x = 2.0f*input.modelPosition.x - 1.0f;\n"
"    output.clipPosition.y = -2.0f*input.modelPosition.y + 1.0f;\n"
"    output.clipPosition.z = 0.0f;\n"
"    output.clipPosition.w = 1.0f;\n"
"    output.vertexTCoord = input.modelTCoord;\n"
"    return output;\n"
"}\n";

std::string const OverlayEffect::msColorPShaderString =
"Texture2D<float4> imageTexture;\n"
"SamplerState imageSampler;\n"
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
"    output.pixelColor0 = imageTexture.Sample(imageSampler, input.vertexTCoord);\n"
"    return output;\n"
"}\n";

std::string const OverlayEffect::msGrayPShaderString =
"Texture2D<float> imageTexture;\n"
"SamplerState imageSampler;\n"
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
"    float gray = imageTexture.Sample(imageSampler, input.vertexTCoord);\n"
"    output.pixelColor0 = float4(gray, gray, gray, 1.0f);\n"
"    return output;\n"
"}\n";
