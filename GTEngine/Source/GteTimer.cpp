// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/08/31)

#include "GTEnginePCH.h"
#include "GteTimer.h"
#include <Windows.h>
using namespace gte;

//----------------------------------------------------------------------------
Timer::Timer()
    :
    mFrequency(0),
    mInitialTicks(0),
    mInvFrequency(0.0)
{
    LARGE_INTEGER frequency = { 1 }, counter = { 0 };
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    mFrequency = static_cast<int64_t>(frequency.QuadPart);
    mInitialTicks = static_cast<int64_t>(counter.QuadPart);
    mInvFrequency = 1.0/static_cast<double>(mFrequency);
}
//----------------------------------------------------------------------------
int64_t Timer::GetTicks()
{
    LARGE_INTEGER counter = { 0 };
    QueryPerformanceCounter(&counter);
    return static_cast<int64_t>(counter.QuadPart) - mInitialTicks;
}
//----------------------------------------------------------------------------
double Timer::GetSeconds()
{
    return GetSeconds(GetTicks());
}
//----------------------------------------------------------------------------
double Timer::GetSeconds(int64_t numTicks) const
{
    return mInvFrequency*static_cast<double>(numTicks);
}
//----------------------------------------------------------------------------
int64_t Timer::GetTicks(double seconds) const
{
    return static_cast<int64_t>(seconds*static_cast<double>(mFrequency));
}
//----------------------------------------------------------------------------
void Timer::Reset()
{
    LARGE_INTEGER counter = { 0 };
    QueryPerformanceCounter(&counter);
    mInitialTicks = static_cast<int64_t>(counter.QuadPart);
}
//----------------------------------------------------------------------------
