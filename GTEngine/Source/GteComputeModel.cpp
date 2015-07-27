// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.4.0 (2014/09/25)

#include "GTEnginePCH.h"
#include "GteComputeModel.h"
using namespace gte;

//----------------------------------------------------------------------------
ComputeModel::~ComputeModel()
{
}
//----------------------------------------------------------------------------
ComputeModel::ComputeModel()
    :
    numThreads(1)
#if defined(WIN32)
    ,
    engine(nullptr)
#endif
{

}
//----------------------------------------------------------------------------
ComputeModel::ComputeModel(unsigned int inNumThreads)
    :
    numThreads(inNumThreads > 0 ? inNumThreads : 1)
#if defined(WIN32)
    ,
    engine(nullptr)
#endif
{
}
//----------------------------------------------------------------------------
#if defined(WIN32)

ComputeModel::ComputeModel(DX11Engine* inEngine, unsigned int inNumThreads)
    :
    numThreads(inNumThreads > 0 ? inNumThreads : 1),
    engine(inEngine)
{
}

#endif
//----------------------------------------------------------------------------
