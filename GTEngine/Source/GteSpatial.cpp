// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteSpatial.h"
#include "GteCuller.h"
using namespace gte;

//----------------------------------------------------------------------------
Spatial::~Spatial ()
{
    // The mParent member is not reference counted by Spatial, so do not
    // release it here.
}
//----------------------------------------------------------------------------
Spatial::Spatial ()
    :
    culling(CULL_DYNAMIC),
    mParent(nullptr)
{
}
//----------------------------------------------------------------------------
void Spatial::Update (bool initiator)
{
    UpdateWorldData();
    UpdateWorldBound();
    if (initiator)
    {
        PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
void Spatial::OnGetVisibleSet (Culler& culler, bool noCull)
{
    if (culling == CULL_ALWAYS)
    {
        return;
    }

    if (culling == CULL_NEVER)
    {
        noCull = true;
    }

    unsigned int savePlaneState = culler.GetPlaneState();
    if (noCull || culler.IsVisible(worldBound))
    {
        GetVisibleSet(culler, noCull);
    }
    culler.SetPlaneState(savePlaneState);
}
//----------------------------------------------------------------------------
void Spatial::UpdateWorldData ()
{
    if (mParent)
    {
#if defined(GTE_USE_MAT_VEC)
        worldTransform = mParent->worldTransform*localTransform;
#else
        worldTransform = localTransform*mParent->worldTransform;
#endif
    }
    else
    {
        worldTransform = localTransform;
    }
}
//----------------------------------------------------------------------------
void Spatial::PropagateBoundToRoot ()
{
    if (mParent)
    {
        mParent->UpdateWorldBound();
        mParent->PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
