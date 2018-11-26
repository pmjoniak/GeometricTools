// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.3.0 (2016/07/10)

#pragma once

#include <GTEngine.h>
#include "PhysicsModule.h"
using namespace gte;

//#define GELATIN_CUBE_SINGLE_STEP

class GelatinCubeWindow : public Window3
{
public:
    GelatinCubeWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateCube();
    void CreateSprings();
    void PhysicsTick();
    void GraphicsTick();

    // The vertex layout for the cube.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    // Support for creating the cube faces.
    void CreateFaceVertices(unsigned int numRows, unsigned int numCols,
        float faceValue, unsigned int const permute[3], Vertex* vertices,
        unsigned int& index);

    void CreateFaceIndices(unsigned int numRows, unsigned int numCols,
        bool ccw, unsigned int& vBase, unsigned int*& indices);

    void UpdateFaces();

    // The scene graph.
    std::shared_ptr<BlendState> mBlendState;
    std::shared_ptr<DepthStencilState> mDepthReadNoWriteState;
    std::shared_ptr<RasterizerState> mNoCullSolidState;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<Node> mScene;
    std::shared_ptr<Visual> mCube;

    // The physics system.
    std::unique_ptr<PhysicsModule> mModule;
    Timer mMotionTimer;

    // The masses are located at the control points of a spline surface.
    // The control points are connected in a mass-spring system.
    std::shared_ptr<BSplineVolume<3, float>> mVolume;
    unsigned int mNumUSamples, mNumVSamples, mNumWSamples;
};
