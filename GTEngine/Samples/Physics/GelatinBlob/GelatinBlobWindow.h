// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.1.1 (2016/07/10)

#pragma once

#include <GTEngine.h>
#include "PhysicsModule.h"
using namespace gte;

//#define GELATIN_BLOB_SINGLE_STEP

class GelatinBlobWindow : public Window3
{
public:
    GelatinBlobWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateIcosahedron();
    void CreateSprings();
    void CreateSegments();
    void PhysicsTick();
    void GraphicsTick();

    // The vertex layout for the icosahedron.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // The scene graph.
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthReadNoWriteState;
    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Node> mScene, mSegmentRoot;
    std::shared_ptr<Visual> mIcosahedron;
    std::vector<std::shared_ptr<Visual>> mSegments;

    // The physics system.
    std::unique_ptr<PhysicsModule> mModule;
    Timer mMotionTimer;
};
