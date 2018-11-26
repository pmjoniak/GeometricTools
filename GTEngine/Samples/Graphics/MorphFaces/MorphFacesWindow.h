// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <GTEngine.h>
#include "CubicInterpolator.h"
using namespace gte;

class MorphFacesWindow : public Window3
{
public:
    MorphFacesWindow(Parameters& parameters);

    virtual void OnIdle() override;
    virtual bool OnCharPress(unsigned char key, int x, int y) override;

private:
    bool SetEnvironment();
    void CreateScene();
    void CreateMorphResult();
    void LoadTarget(int i, std::string const& targetName);
    void UpdateMorph(float time);

    std::shared_ptr<Node> mScene, mMorphResult;
    std::shared_ptr<RasterizerState> mWireState;
    std::shared_ptr<Lighting> mLighting;
    std::shared_ptr<LightCameraGeometry> mLightGeometry;
    std::array<std::shared_ptr<PointLightEffect>, 4> mPLEffects;
    std::vector<std::shared_ptr<Visual>> mVisuals;

    enum { NUM_TARGETS = 12 };

    struct InVertex
    {
        Vector3<float> position, normal;
    };

    struct OutVertex
    {
        Vector3<float> position, normal;
        Vector2<float> tcoord;
    };

    int mNumVertices;
    std::array<std::vector<InVertex>, NUM_TARGETS> mVertices;
    std::array<std::shared_ptr<CubicInterpolator<1, float>>, NUM_TARGETS> mWeightInterpolator;
    std::shared_ptr<CubicInterpolator<3, float>> mColorInterpolator;
    Vector4<float> mLightWorldPosition;
    Timer mAnimTimer;
    double mAnimStartTime;
};
