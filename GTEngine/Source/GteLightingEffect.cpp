// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.2.1 (2014/08/31)

#include "GTEnginePCH.h"
#include "GteLightingEffect.h"
#include "GteShaderFactory.h"
using namespace gte;

//----------------------------------------------------------------------------
LightingEffect::~LightingEffect()
{
}
//----------------------------------------------------------------------------
LightingEffect::LightingEffect(LightingConstants const& lighting,
    std::string const& hlslString, bool perVertex)
{
    mPVWMatrixConstant.reset(new ConstantBuffer(sizeof(Matrix4x4<float>),
        true));
    mPVWMatrix = mPVWMatrixConstant->Get<Matrix4x4<float>>();
    *mPVWMatrix = Matrix4x4<float>::Identity();

    mLightingConstant.reset(new ConstantBuffer(sizeof(LightingConstants),
        true));
    mLighting = mLightingConstant->Get<LightingConstants>();
    *mLighting = lighting;

    mVShader.reset(ShaderFactory::CreateVertex("VSMain", hlslString));
    if (!mVShader)
    {
        LogError("Unable to create vertex shader.");
    }

    mPShader.reset(ShaderFactory::CreatePixel("PSMain", hlslString));
    if (!mPShader)
    {
        LogError("Unable to create pixel shader.");
    }

    mVShader->Set("PVWMatrix", mPVWMatrixConstant);

    if (perVertex)
    {
        mVShader->Set("Lighting", mLightingConstant);
    }
    else
    {
        mPShader->Set("Lighting", mLightingConstant);
    }
}
//----------------------------------------------------------------------------
void LightingEffect::SetName(std::string const& name)
{
    VisualEffect::SetName(name);
    mPVWMatrixConstant->SetName(name);
    mLightingConstant->SetName(name);
}
//----------------------------------------------------------------------------
