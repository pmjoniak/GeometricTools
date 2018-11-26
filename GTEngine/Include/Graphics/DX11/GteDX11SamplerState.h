// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <Graphics/GteSamplerState.h>
#include <Graphics/DX11/GteDX11DrawingState.h>

namespace gte
{

class GTE_IMPEXP DX11SamplerState : public DX11DrawingState
{
public:
    // Construction.
    DX11SamplerState(ID3D11Device* device, SamplerState const* samplerState);
    static std::shared_ptr<GEObject> Create(void* device, GraphicsObject const* object);

    // Member access.
    inline SamplerState* GetSamplerState();
    inline ID3D11SamplerState* GetDXSamplerState();

private:
    // Conversions from GTEngine values to DX11 values.
    static D3D11_FILTER const msFilter[];
    static D3D11_TEXTURE_ADDRESS_MODE const msMode[];
    static D3D11_COMPARISON_FUNC const msComparison[];
};

inline SamplerState* DX11SamplerState::GetSamplerState()
{
    return static_cast<SamplerState*>(mGTObject);
}

inline ID3D11SamplerState* DX11SamplerState::GetDXSamplerState()
{
    return static_cast<ID3D11SamplerState*>(mDXObject);
}

}
