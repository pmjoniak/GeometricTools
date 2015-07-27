// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/02)

#include "GTEnginePCH.h"
#include "GteFluid3.h"
using namespace gte;

//----------------------------------------------------------------------------
Fluid3::~Fluid3()
{
}
//----------------------------------------------------------------------------
Fluid3::Fluid3(DX11Engine* engine, int xSize, int ySize, int zSize,
    float dt)
    :
    mEngine(engine),
    mXSize(xSize),
    mYSize(ySize),
    mZSize(zSize),
    mDt(dt),
    mTime(0.0f)
{
    // Create the shared parameters for many of the simulation shaders.
    float dx = 1.0f/static_cast<float>(mXSize);
    float dy = 1.0f/static_cast<float>(mYSize);
    float dz = 1.0f/static_cast<float>(mZSize);
    float dtDivDxDx = (dt/dx)/dx;
    float dtDivDyDy = (dt/dy)/dy;
    float dtDivDzDz = (dt/dz)/dz;
    float ratio0 = dx/dy;
    float ratio1 = dx/dz;
    float ratio0Sqr = ratio0*ratio0;
    float ratio1Sqr = ratio1*ratio1;
    float factor = 0.5f/(1.0f + ratio0Sqr + ratio1Sqr);
    float epsilonX = factor;
    float epsilonY = ratio0Sqr*factor;
    float epsilonZ = ratio1Sqr*factor;
    float epsilon0 = dx*dx*factor;
    float const denViscosity = 0.0001f;
    float const velViscosity = 0.0001f;
    float denVX = denViscosity*dtDivDxDx;
    float denVY = denViscosity*dtDivDyDy;
    float denVZ = denViscosity*dtDivDzDz;
    float velVX = velViscosity*dtDivDxDx;
    float velVY = velViscosity*dtDivDyDy;
    float velVZ = velViscosity*dtDivDzDz;

    mParameters.reset(new ConstantBuffer(sizeof(Fluid3Parameters), false));
    Fluid3Parameters& p = *mParameters->Get<Fluid3Parameters>();
    p.spaceDelta = { dx, dy, dz, 0.0f };
    p.halfDivDelta = { 0.5f / dx, 0.5f / dy, 0.5f / dz, 0.0f };
    p.timeDelta = { dt / dx, dt / dy, dt / dz, dt };
    p.viscosityX = { velVX, velVX, velVX, denVX };
    p.viscosityY = { velVY, velVY, velVY, denVY };
    p.viscosityZ = { velVZ, velVZ, velVZ, denVZ };
    p.epsilon = { epsilonX, epsilonY, epsilonZ, epsilon0 };

    // Create the compute shaders and textures for the simulation.
    mInitializeSource.reset(new Fluid3InitializeSource(mXSize, mYSize,
        mZSize, 8, 8, 8, mParameters));
    mSourceTexture = mInitializeSource->GetSource();

    mInitializeState.reset(new Fluid3InitializeState(mXSize, mYSize, mZSize,
        8, 8, 8));
    mStateTm1Texture = mInitializeState->GetStateTm1();
    mStateTTexture = mInitializeState->GetStateT();

    mEnforceStateBoundary.reset(new Fluid3EnforceStateBoundary(mXSize,
        mYSize, mZSize, 8, 8, 8));

    mUpdateState.reset(new Fluid3UpdateState(mXSize, mYSize, mZSize, 8, 8, 8,
        mParameters));
    mStateTp1Texture = mUpdateState->GetUpdateState();

    mComputeDivergence.reset(new Fluid3ComputeDivergence(mXSize, mYSize,
        mZSize, 8, 8, 8, mParameters));
    mDivergenceTexture = mComputeDivergence->GetDivergence();

    mSolvePoisson.reset(new Fluid3SolvePoisson(mXSize, mYSize, mZSize,
        8, 8, 8, mParameters, 32));
    mPoissonTexture = mSolvePoisson->GetPoisson();

    mAdjustVelocity.reset(new Fluid3AdjustVelocity(mXSize, mYSize, mZSize,
        8, 8, 8, mParameters));
}
//----------------------------------------------------------------------------
void Fluid3::Initialize()
{
    mInitializeSource->Execute(mEngine);
    mInitializeState->Execute(mEngine);
    mEnforceStateBoundary->Execute(mEngine, mStateTm1Texture);
    mEnforceStateBoundary->Execute(mEngine, mStateTTexture);
}
//----------------------------------------------------------------------------
void Fluid3::DoSimulationStep()
{
    mUpdateState->Execute(mEngine, mSourceTexture, mStateTm1Texture,
        mStateTTexture);
    mEnforceStateBoundary->Execute(mEngine, mStateTp1Texture);
    mComputeDivergence->Execute(mEngine, mStateTp1Texture);
    mSolvePoisson->Execute(mEngine, mDivergenceTexture);
    mAdjustVelocity->Execute(mEngine, mStateTp1Texture, mPoissonTexture,
        mStateTm1Texture);
    mEnforceStateBoundary->Execute(mEngine, mStateTm1Texture);
    std::swap(mStateTm1Texture, mStateTTexture);

    mTime += mDt;
}
//----------------------------------------------------------------------------
std::shared_ptr<Texture3> const& Fluid3::GetState() const
{
    return mStateTTexture;
}
//----------------------------------------------------------------------------
