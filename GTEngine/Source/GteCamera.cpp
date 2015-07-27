// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.2 (2014/12/02)

#include "GTEnginePCH.h"
#include "GteCamera.h"
#include "GteVector3.h"
#include "GteConstants.h"
using namespace gte;

//----------------------------------------------------------------------------
Camera::Camera(bool isPerspective)
    :
    mPosition({ 0.0f, 0.0f, 0.0f, 0.0f }),
    mDVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
    mUVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
    mRVector({ 0.0f, 0.0f, 0.0f, 0.0f }),
    mViewMatrix(Matrix4x4<float>::Identity()),
    mProjectionMatrix(Matrix4x4<float>::Identity()),
    mProjectionViewMatrix(Matrix4x4<float>::Identity()),
    mPreViewMatrix(Matrix4x4<float>::Identity()),
    mPostProjectionMatrix(Matrix4x4<float>::Identity()),
    mIsPerspective(isPerspective),
    mPreViewIsIdentity(true),
    mPostProjectionIsIdentity(true),
    mValidateCameraFrame(true)
{
    for (int i = 0; i < VF_QUANTITY; ++i)
    {
        mFrustum[i] = 0.0f;
    }

    SetFrame({ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.0f });

    if (mIsPerspective)
    {
        SetFrustum(90.0f, 1.0f, 1.0f, 10000.0f);
    }
    else
    {
        SetFrustum(0.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    }

    SetPreViewMatrix(Matrix4x4<float>::Identity());
    SetPostProjectionMatrix(Matrix4x4<float>::Identity());
}
//----------------------------------------------------------------------------
void Camera::SetFrame(Vector4<float> const& position,
    Vector4<float> const& dVector, Vector4<float> const& uVector,
    Vector4<float> const& rVector)
{
    mPosition = position;
    SetAxes(dVector, uVector, rVector);
}
//----------------------------------------------------------------------------
void Camera::SetPosition(Vector4<float> const& position)
{
    mPosition = position;
    OnFrameChange();
}
//----------------------------------------------------------------------------
void Camera::SetAxes(Vector4<float> const& dVector,
    Vector4<float> const& uVector, Vector4<float> const& rVector)
{
    mDVector = dVector;
    mUVector = uVector;
    mRVector = rVector;

    float const epsilon = 0.01f;
    float det = DotCross(mDVector, mUVector, mRVector);
    if (std::abs(1.0f - det) > epsilon)
    {
#if defined(GTE_VALIDATE_CAMERA_FRAME_ONCE)
        if (mValidateCameraFrame)
        {
            mValidateCameraFrame = false;

            float lenD = Length(mDVector);
            float lenU = Length(mUVector);
            float lenR = Length(mRVector);
            float dotDU = Dot(mDVector, mUVector);
            float dotDR = Dot(mDVector, mRVector);
            float dotUR = Dot(mUVector, mRVector);
            if (std::abs(1.0f - lenD) > epsilon
            ||  std::abs(1.0f - lenU) > epsilon
            ||  std::abs(1.0f - lenR) > epsilon
            ||  std::abs(dotDU) > epsilon
            ||  std::abs(dotDR) > epsilon
            ||  std::abs(dotUR) > epsilon)
            {
                LogError("Camera frame is not orthonormal.");
            }
        }
#endif
        // The input vectors do not appear to form an orthonormal set.  Time
        // to renormalize.
        Vector4<float> v[3] = { mDVector, mUVector, mRVector };
        Orthonormalize<4,float>(3, v);
        mDVector = v[0];
        mUVector = v[1];
        mRVector = v[2];
    }

    OnFrameChange();
}
//----------------------------------------------------------------------------
void Camera::GetFrame(Vector4<float>& position, Vector4<float>& dVector,
    Vector4<float>& uVector, Vector4<float>& rVector) const
{
    position = mPosition;
    dVector = mDVector;
    uVector = mUVector;
    rVector = mRVector;
}
//----------------------------------------------------------------------------
void Camera::GetAxes(Vector4<float>& dVector, Vector4<float>& uVector,
    Vector4<float>& rVector) const
{
    dVector = mDVector;
    uVector = mUVector;
    rVector = mRVector;
}
//----------------------------------------------------------------------------
void Camera::SetFrustum(float dMin, float dMax, float uMin, float uMax,
    float rMin, float rMax)
{
    mFrustum[VF_DMIN] = dMin;
    mFrustum[VF_DMAX] = dMax;
    mFrustum[VF_UMIN] = uMin;
    mFrustum[VF_UMAX] = uMax;
    mFrustum[VF_RMIN] = rMin;
    mFrustum[VF_RMAX] = rMax;

    OnFrustumChange();
}
//----------------------------------------------------------------------------
void Camera::SetFrustum(const float* frustum)
{
    mFrustum[VF_DMIN] = frustum[VF_DMIN];
    mFrustum[VF_DMAX] = frustum[VF_DMAX];
    mFrustum[VF_UMIN] = frustum[VF_UMIN];
    mFrustum[VF_UMAX] = frustum[VF_UMAX];
    mFrustum[VF_RMIN] = frustum[VF_RMIN];
    mFrustum[VF_RMAX] = frustum[VF_RMAX];

    OnFrustumChange();
}
//----------------------------------------------------------------------------
void Camera::SetFrustum(float upFovDegrees, float aspectRatio, float dMin,
    float dMax)
{
    float halfAngleRadians = 0.5f*upFovDegrees*(float)GTE_C_DEG_TO_RAD;
    mFrustum[VF_UMAX] = dMin*tan(halfAngleRadians);
    mFrustum[VF_RMAX] = aspectRatio*mFrustum[VF_UMAX];
    mFrustum[VF_UMIN] = -mFrustum[VF_UMAX];
    mFrustum[VF_RMIN] = -mFrustum[VF_RMAX];
    mFrustum[VF_DMIN] = dMin;
    mFrustum[VF_DMAX] = dMax;

    OnFrustumChange();
}
//----------------------------------------------------------------------------
void Camera::GetFrustum(float& dMin, float& dMax, float& uMin, float& uMax,
    float& rMin, float& rMax) const
{
    dMin = mFrustum[VF_DMIN];
    dMax = mFrustum[VF_DMAX];
    uMin = mFrustum[VF_UMIN];
    uMax = mFrustum[VF_UMAX];
    rMin = mFrustum[VF_RMIN];
    rMax = mFrustum[VF_RMAX];
}
//----------------------------------------------------------------------------
bool Camera::GetFrustum(float& upFovDegrees, float& aspectRatio,
    float& dMin, float& dMax) const
{
    if (mFrustum[VF_RMIN] == -mFrustum[VF_RMAX]
    &&  mFrustum[VF_UMIN] == -mFrustum[VF_UMAX])
    {
        float tmp = mFrustum[VF_UMAX]/mFrustum[VF_DMIN];
        upFovDegrees = 2.0f*atan(tmp)*(float)GTE_C_RAD_TO_DEG;
        aspectRatio = mFrustum[VF_RMAX]/mFrustum[VF_UMAX];
        dMin = mFrustum[VF_DMIN];
        dMax = mFrustum[VF_DMAX];
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
void Camera::SetProjectionMatrix(Vector4<float> const& p00,
    Vector4<float> const& p10, Vector4<float> const& p11,
    Vector4<float> const& p01, float nearExtrude, float farExtrude)
{
    LogAssert(nearExtrude > 0.0f, "Invalid nearExtrude.");
    LogAssert(farExtrude > nearExtrude, "Invalid farExtrude.");

    // Compute the near face of the view volume.
    Vector4<float> origin{ 0.0f, 0.0f, 0.0f, 1.0f };
    Vector4<float> q000 = origin + nearExtrude*(p00 - origin);
    Vector4<float> q100 = origin + nearExtrude*(p10 - origin);
    Vector4<float> q110 = origin + nearExtrude*(p11 - origin);
    Vector4<float> q010 = origin + nearExtrude*(p01 - origin);

    // Compute the far face of the view volume.
    Vector4<float> q001 = origin + farExtrude*(p00 - origin);
    Vector4<float> q101 = origin + farExtrude*(p10 - origin);
    Vector4<float> q111 = origin + farExtrude*(p11 - origin);
    Vector4<float> q011 = origin + farExtrude*(p01 - origin);

    // Compute the representation of q111.
    Vector4<float> u0 = q100 - q000;
    Vector4<float> u1 = q010 - q000;
    Vector4<float> u2 = q001 - q000;
#if defined(GTE_USE_MAT_VEC)
    Matrix4x4<float> M;
    M.SetCol(0, u0);
    M.SetCol(1, u1);
    M.SetCol(2, u2);
    M.SetCol(3, q000);
    Matrix4x4<float> invM = Inverse(M);
    Vector4<float> a = invM*q111;
#else
    Matrix4x4<float> M;
    M.SetRow(0, u0);
    M.SetRow(1, u1);
    M.SetRow(2, u2);
    M.SetRow(3, q000);
    Matrix4x4<float> invM = Inverse(M);
    Vector4<float> a = q111*invM;
#endif

    // Compute the coefficients in the fractional linear transformation.
    //   y[i] = n[i]*x[i]/(d[0]*x[0] + d[1]*x[1] + d[2]*x[2] + d[3])
    float n0 = 2.0f*a[0];
    float n1 = 2.0f*a[1];
    float n2 = 2.0f*a[2];
    float d0 = +a[0] - a[1] - a[2] + 1.0f;
    float d1 = -a[0] + a[1] - a[2] + 1.0f;
    float d2 = -a[0] - a[1] + a[2] + 1.0f;
    float d3 = +a[0] + a[1] + a[2] - 1.0f;

    // Compute the perspective projection from the canonical cuboid to the
    // canonical cube [-1,1]^2 x [0,1].
    float n2divn0 = n2/n0;
    float n2divn1 = n2/n1;
    Matrix4x4<float> project;
#if defined(GTE_USE_MAT_VEC)
    project(0,0) = n2divn0*(2.0f*d3 + d0);
    project(0,1) = n2divn1*d1;
    project(0,2) = d2;
    project(0,3) = -n2;
    project(1,0) = n2divn0*d0;
    project(1,1) = n2divn1*(2.0f*d3 + d1);
    project(1,2) = d2;
    project(1,3) = -n2;
    project(2,0) = 0.0f;
    project(2,1) = 0.0f;
    project(2,2) = d3;
    project(2,3) = 0.0f;
    project(3,0) = -n2divn0*d0;
    project(3,1) = -n2divn1*d1;
    project(3,2) = -d2;
    project(3,3) = n2;

    // The full projection requires mapping the extruded-quadrilateral view
    // volume to the canonical cuboid, which is then followed by the
    // perspective projection to the canonical cube.
    SetProjectionMatrix(project*invM);
#else
    project(0,0) = n2divn0*(2.0f*d3 + d0);
    project(1,0) = n2divn1*d1;
    project(2,0) = d2;
    project(3,0) = -n2;
    project(0,1) = n2divn0*d0;
    project(1,1) = n2divn1*(2.0f*d3 + d1);
    project(2,1) = d2;
    project(3,1) = -n2;
    project(0,2) = 0.0f;
    project(1,2) = 0.0f;
    project(2,2) = d3;
    project(3,2) = 0.0f;
    project(0,3) = -n2divn0*d0;
    project(1,3) = -n2divn1*d1;
    project(2,3) = -d2;
    project(3,3) = n2;

    // The full projection requires mapping the extruded-quadrilateral view
    // volume to the canonical cuboid, which is then followed by the
    // perspective projection to the canonical cube.
    SetProjectionMatrix(invM*project);
#endif
}
//----------------------------------------------------------------------------
void Camera::SetPreViewMatrix(Matrix4x4<float> const& preViewMatrix)
{
    mPreViewMatrix = preViewMatrix;
    mPreViewIsIdentity = (mPreViewMatrix == Matrix4x4<float>::Identity());
    UpdatePVMatrix();
}
//----------------------------------------------------------------------------
void Camera::SetPostProjectionMatrix(Matrix4x4<float> const& postProjMatrix)
{
    mPostProjectionMatrix = postProjMatrix;
    mPostProjectionIsIdentity =
        (mPostProjectionMatrix == Matrix4x4<float>::Identity());
    UpdatePVMatrix();
}
//----------------------------------------------------------------------------
void Camera::OnFrameChange()
{
    // This leads to left-handed coordinates for the camera frame.
#if defined(GTE_USE_MAT_VEC)
    mViewMatrix(0,0) = mRVector[0];
    mViewMatrix(0,1) = mRVector[1];
    mViewMatrix(0,2) = mRVector[2];
    mViewMatrix(0,3) = -Dot(mPosition, mRVector);
    mViewMatrix(1,0) = mUVector[0];
    mViewMatrix(1,1) = mUVector[1];
    mViewMatrix(1,2) = mUVector[2];
    mViewMatrix(1,3) = -Dot(mPosition, mUVector);
    mViewMatrix(2,0) = mDVector[0];
    mViewMatrix(2,1) = mDVector[1];
    mViewMatrix(2,2) = mDVector[2];
    mViewMatrix(2,3) = -Dot(mPosition, mDVector);
    mViewMatrix(3,0) = 0.0f;
    mViewMatrix(3,1) = 0.0f;
    mViewMatrix(3,2) = 0.0f;
    mViewMatrix(3,3) = 1.0f;
#else
    mViewMatrix(0,0) = mRVector[0];
    mViewMatrix(1,0) = mRVector[1];
    mViewMatrix(2,0) = mRVector[2];
    mViewMatrix(3,0) = -Dot(mPosition, mRVector);
    mViewMatrix(0,1) = mUVector[0];
    mViewMatrix(1,1) = mUVector[1];
    mViewMatrix(2,1) = mUVector[2];
    mViewMatrix(3,1) = -Dot(mPosition, mUVector);
    mViewMatrix(0,2) = mDVector[0];
    mViewMatrix(1,2) = mDVector[1];
    mViewMatrix(2,2) = mDVector[2];
    mViewMatrix(3,2) = -Dot(mPosition, mDVector);
    mViewMatrix(0,3) = 0.0f;
    mViewMatrix(1,3) = 0.0f;
    mViewMatrix(2,3) = 0.0f;
    mViewMatrix(3,3) = 1.0f;
#endif

    UpdatePVMatrix();
}
//----------------------------------------------------------------------------
void Camera::OnFrustumChange()
{
    // Map (x,y,z) into [-1,1]x[-1,1]x[0,1].
    float dMin = mFrustum[VF_DMIN];
    float dMax = mFrustum[VF_DMAX];
    float uMin = mFrustum[VF_UMIN];
    float uMax = mFrustum[VF_UMAX];
    float rMin = mFrustum[VF_RMIN];
    float rMax = mFrustum[VF_RMAX];
    float invDDiff = 1.0f/(dMax - dMin);
    float invUDiff = 1.0f/(uMax - uMin);
    float invRDiff = 1.0f/(rMax - rMin);

    if (mIsPerspective)
    {
#if defined(GTE_USE_MAT_VEC)
        mProjectionMatrix(0,0) = 2.0f*dMin*invRDiff;
        mProjectionMatrix(0,1) = 0.0f;
        mProjectionMatrix(0,2) = -(rMin + rMax)*invRDiff;
        mProjectionMatrix(0,3) = 0.0f;
        mProjectionMatrix(1,0) = 0.0f;
        mProjectionMatrix(1,1) = 2.0f*dMin*invUDiff;
        mProjectionMatrix(1,2) = -(uMin + uMax)*invUDiff;
        mProjectionMatrix(1,3) = 0.0f;
        mProjectionMatrix(2,0) = 0.0f;
        mProjectionMatrix(2,1) = 0.0f;
        mProjectionMatrix(2,2) = dMax*invDDiff;
        mProjectionMatrix(2,3) = -dMin*dMax*invDDiff;
        mProjectionMatrix(3,0) = 0.0f;
        mProjectionMatrix(3,1) = 0.0f;
        mProjectionMatrix(3,2) = 1.0f;
        mProjectionMatrix(3,3) = 0.0f;
#else
        mProjectionMatrix(0,0) = 2.0f*dMin*invRDiff;
        mProjectionMatrix(1,0) = 0.0f;
        mProjectionMatrix(2,0) = -(rMin + rMax)*invRDiff;
        mProjectionMatrix(3,0) = 0.0f;
        mProjectionMatrix(0,1) = 0.0f;
        mProjectionMatrix(1,1) = 2.0f*dMin*invUDiff;
        mProjectionMatrix(2,1) = -(uMin + uMax)*invUDiff;
        mProjectionMatrix(3,1) = 0.0f;
        mProjectionMatrix(0,2) = 0.0f;
        mProjectionMatrix(1,2) = 0.0f;
        mProjectionMatrix(2,2) = dMax*invDDiff;
        mProjectionMatrix(3,2) = -dMin*dMax*invDDiff;
        mProjectionMatrix(0,3) = 0.0f;
        mProjectionMatrix(1,3) = 0.0f;
        mProjectionMatrix(2,3) = 1.0f;
        mProjectionMatrix(3,3) = 0.0f;
#endif
    }
    else
    {
#if defined (GTE_USE_MAT_VEC)
        mProjectionMatrix(0,0) = 2.0f*invRDiff;
        mProjectionMatrix(0,1) = 0.0f;
        mProjectionMatrix(0,2) = 0.0f;
        mProjectionMatrix(0,3) = -(rMin + rMax)*invRDiff;
        mProjectionMatrix(1,0) = 0.0f;
        mProjectionMatrix(1,1) = 2.0f*invUDiff;
        mProjectionMatrix(1,2) = 0.0f;
        mProjectionMatrix(1,3) = -(uMin + uMax)*invUDiff;
        mProjectionMatrix(2,0) = 0.0f;
        mProjectionMatrix(2,1) = 0.0f;
        mProjectionMatrix(2,2) = invDDiff;
        mProjectionMatrix(2,3) = -dMin*invDDiff;
        mProjectionMatrix(3,0) = 0.0f;
        mProjectionMatrix(3,1) = 0.0f;
        mProjectionMatrix(3,2) = 0.0f;
        mProjectionMatrix(3,3) = 1.0f;
#else
        mProjectionMatrix(0,0) = 2.0f*invRDiff;
        mProjectionMatrix(1,0) = 0.0f;
        mProjectionMatrix(2,0) = 0.0f;
        mProjectionMatrix(3,0) = -(rMin + rMax)*invRDiff;
        mProjectionMatrix(0,1) = 0.0f;
        mProjectionMatrix(1,1) = 2.0f*invUDiff;
        mProjectionMatrix(2,1) = 0.0f;
        mProjectionMatrix(3,1) = -(uMin + uMax)*invUDiff;
        mProjectionMatrix(0,2) = 0.0f;
        mProjectionMatrix(1,2) = 0.0f;
        mProjectionMatrix(2,2) = invDDiff;
        mProjectionMatrix(3,2) = -dMin*invDDiff;
        mProjectionMatrix(0,3) = 0.0f;
        mProjectionMatrix(1,3) = 0.0f;
        mProjectionMatrix(2,3) = 0.0f;
        mProjectionMatrix(3,3) = 1.0f;
#endif
    }

    UpdatePVMatrix();
}
//----------------------------------------------------------------------------
void Camera::UpdatePVMatrix()
{
    Matrix4x4<float>& pMatrix = mProjectionMatrix;
    Matrix4x4<float>& pvMatrix = mProjectionViewMatrix;

#if defined(GTE_USE_MAT_VEC)
    pvMatrix = pMatrix*mViewMatrix;
    if (!mPostProjectionIsIdentity)
    {
        pvMatrix = mPostProjectionMatrix*pvMatrix;
    }
    if (!mPreViewIsIdentity)
    {
        pvMatrix = pvMatrix*mPreViewMatrix;
    }
#else
    pvMatrix = mViewMatrix*pMatrix;
    if (!mPostProjectionIsIdentity)
    {
        pvMatrix = pvMatrix*mPostProjectionMatrix;
    }
    if (!mPreViewIsIdentity)
    {
        pvMatrix = mPreViewMatrix*pvMatrix;
    }
#endif
}
//----------------------------------------------------------------------------
