// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GtePickRecord.h"
using namespace gte;

//----------------------------------------------------------------------------
PickRecord::~PickRecord()
{
}
//----------------------------------------------------------------------------
PickRecord::PickRecord()
    :
    visual(nullptr),
    primitiveType(IP_NONE),
    primitiveIndex(0),
    t(0.0f)
{
    bary[0] = 0.0f;
    bary[1] = 0.0f;
    bary[2] = 0.0f;
}
//----------------------------------------------------------------------------
