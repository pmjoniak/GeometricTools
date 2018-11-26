// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2018/10/05)

#include "PhysicsModule.h"

PhysicsModule::PhysicsModule ()
    :
    springConstant(0.0f),
    mass(0.0f),
    mTime(0.0f),
    mDeltaTime(0.0f),
    mPosition({ 0.0f, 0.0f }),
    mVelocity({ 0.0f, 0.0f }),
    mInitialPosition({ 0.0f, 0.0f }),
    mFrequency(0),
    mVelDivFreq({ 0.0f, 0.0f })
{
}

void PhysicsModule::Evaluate()
{
    float angle = mFrequency*mTime;
    float sn = std::sin(angle);
    float cs = std::cos(angle);
    mPosition = cs * mInitialPosition + sn * mVelDivFreq;
    mVelocity = (mVelDivFreq * cs - mInitialPosition * sn) * mFrequency;
}

void PhysicsModule::Initialize(float time, float deltaTime,
    Vector2<float> const& initialPosition, Vector2<float> const& initialVelocity)
{
    mTime = time;
    mDeltaTime = deltaTime;
    mInitialPosition = initialPosition;
    mFrequency = std::sqrt(springConstant / mass);
    mVelDivFreq = initialVelocity / mFrequency;
    Evaluate();
}

void PhysicsModule::Update()
{
    mTime += mDeltaTime;
    Evaluate();
}
