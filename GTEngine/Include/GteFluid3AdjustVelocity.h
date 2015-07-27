// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#pragma once

#include "GTEngineDEF.h"
#include "GteFluid3Parameters.h"
#include "GteComputeShader.h"
#include "GteDX11Engine.h"
#include "GteTexture3.h"

namespace gte
{

class GTE_IMPEXP Fluid3AdjustVelocity
{
public:
    // Construction and destruction.  Adjust the velocities using the
    // solution to the Poisson equation.
    ~Fluid3AdjustVelocity();
    Fluid3AdjustVelocity(int xSize, int ySize, int zSize, int numXThreads,
        int numYThreads, int numZThreads,
        std::shared_ptr<gte::ConstantBuffer> const& parameters);

    // Update the state for the fluid simulation.
    void Execute(gte::DX11Engine* engine,
        std::shared_ptr<gte::Texture3> const& inState,
        std::shared_ptr<gte::Texture3> const& poisson,
        std::shared_ptr<gte::Texture3> const& outState);

private:
    int mNumXGroups, mNumYGroups, mNumZGroups;
    std::shared_ptr<gte::ComputeShader> mAdjustVelocity;

    static std::string const msHLSLAdjustVelocityString;
};

}
