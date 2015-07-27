// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/13)

#pragma once

#include "GTEngineDEF.h"
#include "GteVisualEffect.h"
#include "GteMatrix4x4.h"
#include "GteTexture3.h"

namespace gte
{

class GTE_IMPEXP Texture3Effect : public VisualEffect
{
public:
    // Construction.
    Texture3Effect(std::shared_ptr<Texture3> const& texture,
        SamplerState::Filter filter, SamplerState::Mode mode0,
        SamplerState::Mode mode1, SamplerState::Mode mode2);

    // Member access.
    inline void SetPVWMatrix(Matrix4x4<float> const& pvwMatrix);
    inline Matrix4x4<float> const& GetPVWMatrix() const;

    // Required to bind and update resources.
    inline std::shared_ptr<ConstantBuffer> const& GetPVWMatrixConstant() const;
    inline std::shared_ptr<Texture3> const& GetTexture() const;
    inline std::shared_ptr<SamplerState> const& GetSampler() const;

    // Naming support, used in the DX11 debug layer.  The default name is "".
    // If you want the name to show up in the DX11 destruction messages when
    // the associated DX11GraphicsObject is destroyed, set the name to
    // something other than "".
    virtual void SetName(std::string const& name);

private:
    // Vertex shader parameters.
    std::shared_ptr<ConstantBuffer> mPVWMatrixConstant;

    // Pixel shader parameters.
    std::shared_ptr<Texture3> mTexture;
    std::shared_ptr<SamplerState> mSampler;

    // Convenience pointers.
    Matrix4x4<float>* mPVWMatrix;

    static std::string const msHLSLString;
};

//----------------------------------------------------------------------------
inline void Texture3Effect::SetPVWMatrix(Matrix4x4<float> const& pvwMatrix)
{
    *mPVWMatrix = pvwMatrix;
}
//----------------------------------------------------------------------------
inline Matrix4x4<float> const& Texture3Effect::GetPVWMatrix() const
{
    return *mPVWMatrix;
}
//----------------------------------------------------------------------------
inline std::shared_ptr<ConstantBuffer> const&
Texture3Effect::GetPVWMatrixConstant() const
{
    return mPVWMatrixConstant;
}
//----------------------------------------------------------------------------
inline std::shared_ptr<Texture3> const& Texture3Effect::GetTexture() const
{
    return mTexture;
}
//----------------------------------------------------------------------------
inline std::shared_ptr<SamplerState> const& Texture3Effect::GetSampler() const
{
    return mSampler;
}
//----------------------------------------------------------------------------

}
