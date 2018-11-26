// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <Graphics/GteConstantBuffer.h>
#include <Graphics/GteProgramFactory.h>
#include <Graphics/GteTexture2.h>

namespace gte
{

class GraphicsEngine;

class GTE_IMPEXP Fluid2EnforceStateBoundary
{
public:
    // Construction.  Clamp the velocity vectors at the image boundary to
    // ensure they do not point outside the domain.  For example, let
    // image(x,y) have velocity (vx,vy).  At the boundary pixel (x,0),
    // the velocity is clamped to (vx,0).  At the boundary pixel (0,y), the
    // velocity is clamped to (0,vy).  The density is set to zero on the image
    // boundary.
    Fluid2EnforceStateBoundary(std::shared_ptr<ProgramFactory> const& factory,
        int xSize, int ySize, int numXThreads, int numYThreads);

    // Set the density and velocity values at the image boundary as described
    // in the comments for the constructor.  The state texture has texels
    // (velocity.x, velocity.y, 0, density).
    void Execute(std::shared_ptr<GraphicsEngine> const& engine,
        std::shared_ptr<Texture2> const& state);

private:
    int mNumXGroups, mNumYGroups;
    std::shared_ptr<ComputeProgram> mCopyXEdge;
    std::shared_ptr<ComputeProgram> mWriteXEdge;
    std::shared_ptr<ComputeProgram> mCopyYEdge;
    std::shared_ptr<ComputeProgram> mWriteYEdge;
    std::shared_ptr<Texture1> mXMin;
    std::shared_ptr<Texture1> mXMax;
    std::shared_ptr<Texture1> mYMin;
    std::shared_ptr<Texture1> mYMax;

    // Shader source code as strings.
    static std::string const msGLSLSource;
    static std::string const msHLSLSource;
    static std::string const* msSource[ProgramFactory::PF_NUM_API];
};

}
