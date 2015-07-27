// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteVisualEffect.h"
using namespace gte;

//----------------------------------------------------------------------------
VisualEffect::~VisualEffect()
{

}
//----------------------------------------------------------------------------
VisualEffect::VisualEffect(
    std::shared_ptr<VertexShader> const& vshader,
    std::shared_ptr<PixelShader> const& pshader,
    std::shared_ptr<GeometryShader> const& gshader)
    :
    mVShader(vshader),
    mPShader(pshader),
    mGShader(gshader)
{
}
//----------------------------------------------------------------------------
void VisualEffect::SetName(std::string const& name)
{
    mName = name;

    if (mVShader)
    {
        mVShader->SetName(mName + ":mVShader");
    }

    if (mPShader)
    {
        mPShader->SetName(mName + ":mPShader");
    }

    if (mGShader)
    {
        mGShader->SetName(mName + ":mGShader");
    }
}
//----------------------------------------------------------------------------
