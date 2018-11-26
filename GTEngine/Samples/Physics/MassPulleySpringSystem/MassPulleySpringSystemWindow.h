// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.3.0 (2016/07/13)

#pragma once

#include <GTEngine.h>
#include "PhysicsModule.h"
using namespace gte;

//#define MASS_PULLEY_SPRING_SYSTEM_SINGLE_STEP

class MassPulleySpringSystemWindow : public Window3
{
public:
    MassPulleySpringSystemWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    bool SetEnvironment();
    void InitializeModule();
    void CreateScene();
    void CreateFloor();
    void CreateCable();
    std::shared_ptr<Visual> CreateMass(float radius);
    void CreatePulley();
    void CreateSpring();
    void CreateHelix();
    void UpdatePulley();
    void UpdateCable();
    void UpdateHelix();
    void PhysicsTick();
    void GraphicsTick();

    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // Root of scene and floor mesh.
    std::shared_ptr<RasterizerState> mWireState;
    std::shared_ptr<Visual> mFloor;
    std::shared_ptr<Node> mScene;
    std::vector<std::shared_ptr<Visual>> mVisuals;

    // Assembly to parent the cable root and pulley root.
    std::shared_ptr<Node> mAssembly;

    // Cable modeled as a tube surface, masses attached to ends.
    std::shared_ptr<Node> mCableRoot;
    std::shared_ptr<BSplineCurve<3, float>> mCableSpline;
    std::unique_ptr<TubeMesh<float>> mCableSurface;
    std::shared_ptr<Visual> mCable;
    std::shared_ptr<Visual> mMass1, mMass2;

    // Node to parent the pulley and spring.
    std::shared_ptr<Node> mPulleyRoot;

    // Pulley modeled as a disk with thickness.
    std::shared_ptr<Node> mPulley;
    std::shared_ptr<Visual> mPlate0, mPlate1, mCylinder;

    // Spring modeled as a tube surface in the shape of a helix, then attached
    // to a U-bracket to hold the pulley disk.
    std::shared_ptr<Node> mSpring;
    std::shared_ptr<Visual> mSide0, mSide1, mTop;
    std::shared_ptr<BSplineCurve<3, float>> mHelixSpline;
    std::unique_ptr<TubeMesh<float>> mHelixSurface;
    std::shared_ptr<Visual> mHelix;

    // The physics system.
    PhysicsModule mModule;

    // Support for clamping the frame rate.
    Timer mMotionTimer;
    double mLastUpdateTime;
};
