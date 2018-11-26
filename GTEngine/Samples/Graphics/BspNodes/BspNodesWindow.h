// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2018/09/08)

#pragma once

#include <GTEngine.h>
using namespace gte;

// In addition to illustrating BSP nodes, this example shows how to use
// PVWUpdater in conjunction with scene graph culling.

class BspNodesWindow : public Window3
{
public:
    BspNodesWindow(Parameters& parameters);

    virtual void OnIdle() override;

    // These functions are overridden because the Window3 versions call
    // mPVWMatriceds.Update().  Because we are dynamically culling, we also
    // need to call mPVWMatrices.Update(mCuller.GetVisibleSet()).
    virtual bool OnResize(int xSize, int ySize) override;
    virtual bool OnMouseMotion(MouseButton button, int x, int y, unsigned int modifiers) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void DoCullSort();

    std::shared_ptr<BspNode> CreateNode(int i, Vector2<float> const& v0,
        Vector2<float> const& v1, Vector4<float> const& color);

    Culler mCuller;
    std::shared_ptr<Node> mScene;
    std::shared_ptr<Visual> mGround;
    std::shared_ptr<RasterizerState> mNoCullWireState;
    std::shared_ptr<BspNode> mBSPNode[4];
    std::shared_ptr<Visual> mRectangle[4];
    std::shared_ptr<VertexColorEffect> mVCEffect[4];
    std::shared_ptr<Visual> mTorus, mSphere, mTetrahedron, mCube, mOctahedron;
    std::set<Visual*> mVisualOpaque;
    std::vector<Visual*> mVisibleOpaque;
    std::vector<Visual*> mVisibleNoCullWire;
};
