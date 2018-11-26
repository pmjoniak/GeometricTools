// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <Graphics/GteVisual.h>
#include <Graphics/GteTexture2Effect.h>
#include <Mathematics/GteVector2.h>
#include <Mathematics/GteVector3.h>
using namespace gte;

class DeformableBall
{
public:
    // Construction.
    DeformableBall(float duration, float period, std::shared_ptr<Texture2Effect> const& effect);

    // Member access.
    void Set(float duration, float period);
    inline std::shared_ptr<Visual> const& GetMesh() const;
    inline float GetDuration() const;
    inline float GetPeriod() const;
    inline float GetMinActive() const;
    inline float GetMaxActive() const;
    inline float GetAmplitude(float time) const;
    inline bool& DoAffine();

    // Deform the ball.
    bool DoSimulationStep(float realTime);

private:
    // Mesh vertex type.
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> tcoord;
    };

    void CreateBall(std::shared_ptr<Texture2Effect> const& effect);

    // Support for the mesh smoother of mMesh.
    void CreateSmoother();
    void Update(float time);

    // Influence function:  parameters (X,t)
    //   input:  X = point on surface
    //           t = current time
    // The return value is 'true' if and only if the point is within the
    // region of influence of the deformation *and* if the deformation
    // function is active at time t.  This allows the mesh smoother to avoid
    // evolving the surface in regions where no deformation is occuring.
    bool VertexInfluenced(unsigned int i, float time, Vector3<float> const& position) const;

    float GetTangentWeight(unsigned int i, float time, Vector3<float> const& position) const;
    float GetNormalWeight(unsigned int i, float time, Vector3<float> const& position) const;

    // Level surface function:  parameters (X,t,F,Grad(F))
    //   input:  X = point in space
    //           t = time of deformation
    //   output: F(X,t) = scalar function at position and time
    //           Grad(F)(X,t) = gradient at level surface through X at time
    void ComputeFunction(Vector3<float> const& position, float time,
        float& function, Vector3<float>& gradient) const;

    std::shared_ptr<Visual> mMesh;
    float mDuration, mPeriod, mDeformMult;
    float mMinActive, mMaxActive, mInvActiveRange;
    bool mDeforming, mDoAffine;

    // Parameters for Newton's method in ComputeFunction.
    int mMaxIterations;
    float mErrorTolerance;

    // Mesh smoother data.
    std::vector<Vector3<float>> mNormal, mMean;
    std::vector<int> mNeighborCount;
};

inline std::shared_ptr<Visual> const& DeformableBall::GetMesh() const
{
    return mMesh;
}

inline float DeformableBall::GetDuration() const
{
    return mDuration;
}

inline float DeformableBall::GetPeriod() const
{
    return mPeriod;
}

inline float DeformableBall::GetMinActive() const
{
    return mMinActive;
}

inline float DeformableBall::GetMaxActive() const
{
    return mMaxActive;
}

inline float DeformableBall::GetAmplitude(float time) const
{
    return mDeformMult * (time - mMinActive) * (mMaxActive - time);
}

inline bool& DeformableBall::DoAffine()
{
    return mDoAffine;
}
