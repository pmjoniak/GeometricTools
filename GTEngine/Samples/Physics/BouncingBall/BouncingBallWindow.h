// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <GTEngine.h>
#include "DeformableBall.h"
using namespace gte;

//#define BOUNCING_BALL_SINGLE_STEP

class BouncingBallWindow : public Window3
{
public:
    BouncingBallWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateBall();
    void CreateFloor();
    void CreateWall();
    void PhysicsTick();
    void GraphicsTick();

    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // Representation of body
    std::unique_ptr<DeformableBall> mBall;

    // Simulated clock.
    float mSimTime, mSimDelta;

    // The scene graph.
    std::shared_ptr<Node> mScene, mBallNode;
    std::shared_ptr<Visual> mFloor, mWall;
    std::shared_ptr<RasterizerState> mWireState;
    std::unique_ptr<PlanarReflectionEffect> mPlanarReflectionEffect;
    VisibleSet mSceneVisibleSet, mBallNodeVisibleSet;
};
