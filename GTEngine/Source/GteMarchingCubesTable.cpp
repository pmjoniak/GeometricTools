// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteMarchingCubesTable.h"

namespace gte
{
int const gMarchingCubesTable[256][41] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 0, 1, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 4, 0, 2, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 2, 6, 2, 3, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 1, 3, 1, 5, 0, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 0, 4, 2, 6, 2, 3, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 1, 0, 4, 0, 2, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 1, 5, 0, 1, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 1, 5, 0, 4, 0, 2, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 3, 7, 1, 3, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 2, 6, 3, 7, 1, 3, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 3, 7, 1, 5, 0, 1, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 4, 2, 6, 3, 7, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 4, 5, 4, 6, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 1, 1, 3, 1, 5, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 0, 2, 1, 3, 1, 5, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 4, 4, 5, 4, 6, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 0, 1, 4, 5, 4, 6, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 0, 2, 2, 6, 2, 3, 1, 3, 1, 5, 0, 1, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 5, 4, 6, 2, 6, 2, 3, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 2, 2, 3, 3, 7, 1, 3, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 1, 4, 5, 4, 6, 0, 2, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 1, 2, 3, 3, 7, 1, 5, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 5, 4, 6, 0, 2, 2, 3, 3, 7, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 3, 2, 6, 3, 7, 1, 3, 0, 2, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 6, 2, 6, 3, 7, 1, 3, 0, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 8, 4, 3, 7, 1, 5, 0, 1, 0, 2, 2, 6, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 5, 3, 3, 7, 2, 6, 4, 6, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 5, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 4, 0, 2, 0, 1, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 5, 7, 4, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 1, 3, 5, 7, 4, 5, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 2, 2, 6, 2, 3, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 4, 2, 6, 2, 3, 0, 1, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 1, 3, 5, 7, 4, 5, 0, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 5, 0, 4, 2, 6, 2, 3, 1, 3, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 2, 5, 7, 4, 5, 1, 5, 1, 3, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 0, 1, 0, 4, 0, 2, 2, 3, 3, 7, 1, 3, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 0, 1, 2, 3, 3, 7, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 5, 7, 4, 5, 0, 4, 0, 2, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 3, 1, 3, 0, 2, 2, 6, 3, 7, 5, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 2, 6, 3, 7, 1, 3, 0, 1, 0, 4, 5, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 6, 4, 5, 7, 4, 5, 0, 1, 0, 2, 2, 6, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 2, 6, 0, 4, 4, 5, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 4, 6, 0, 4, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 4, 6, 0, 2, 0, 1, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 5, 7, 4, 6, 0, 4, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 2, 1, 3, 5, 7, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 4, 1, 5, 5, 7, 4, 6, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 1, 5, 5, 7, 4, 6, 2, 6, 2, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 8, 4, 5, 7, 4, 6, 0, 4, 0, 1, 1, 3, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 5, 3, 5, 7, 1, 3, 2, 3, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 5, 7, 4, 6, 0, 4, 1, 5, 1, 3, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 4, 6, 0, 2, 0, 1, 1, 5, 5, 7, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 6, 4, 3, 7, 5, 7, 4, 6, 0, 4, 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 4, 6, 5, 7, 3, 7, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 3, 7, 1, 3, 0, 2, 2, 6, 1, 5, 5, 7, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 7, 5, 3, 7, 2, 6, 4, 6, 5, 7, 1, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 7, 5, 4, 6, 5, 7, 3, 7, 2, 6, 0, 2, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 4, 2, 2, 6, 4, 6, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 2, 0, 1, 0, 4, 4, 6, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 1, 3, 1, 5, 0, 1, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 2, 1, 3, 1, 5, 0, 4, 4, 6, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 4, 6, 6, 7, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 2, 3, 0, 1, 0, 4, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 0, 2, 4, 6, 6, 7, 2, 3, 1, 3, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 6, 6, 7, 2, 3, 1, 3, 1, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 2, 3, 7, 1, 3, 2, 3, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 2, 3, 3, 7, 1, 3, 0, 1, 0, 4, 0, 2, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 3, 7, 1, 5, 0, 1, 2, 3, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 1, 5, 0, 4, 0, 2, 2, 3, 3, 7, 4, 6, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 5, 3, 0, 2, 4, 6, 6, 7, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 4, 6, 6, 7, 3, 7, 1, 3, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 4, 6, 7, 3, 7, 1, 5, 0, 1, 0, 2, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 1, 5, 3, 7, 6, 7, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 6, 7, 2, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 4, 5, 6, 7, 2, 6, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 4, 5, 6, 7, 2, 6, 0, 4, 0, 1, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 2, 6, 0, 2, 1, 3, 1, 5, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 6, 7, 2, 3, 0, 2, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 1, 4, 5, 6, 7, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 6, 7, 2, 3, 0, 2, 0, 4, 4, 5, 1, 3, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 5, 3, 6, 7, 4, 5, 1, 5, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 2, 6, 0, 4, 4, 5, 6, 7, 3, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 4, 5, 6, 7, 2, 6, 0, 2, 0, 1, 3, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 8, 4, 1, 5, 0, 1, 2, 3, 3, 7, 0, 4, 4, 5, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 7, 5, 6, 7, 4, 5, 1, 5, 3, 7, 2, 3, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 3, 7, 1, 3, 0, 2, 0, 4, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 4, 5, 0, 1, 1, 3, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 7, 5, 1, 5, 3, 7, 6, 7, 4, 5, 0, 4, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 4, 2, 4, 5, 1, 5, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 4, 5, 1, 5, 5, 7, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 4, 5, 1, 5, 5, 7, 6, 7, 2, 6, 4, 6, 0, 4, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 4, 5, 0, 1, 1, 3, 5, 7, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 1, 3, 5, 7, 4, 5, 0, 4, 0, 2, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 3, 6, 7, 2, 3, 0, 2, 4, 6, 4, 5, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 2, 3, 0, 1, 0, 4, 4, 6, 6, 7, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 8, 4, 4, 6, 6, 7, 2, 3, 0, 2, 5, 7, 4, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 7, 5, 5, 7, 1, 3, 2, 3, 6, 7, 4, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 9, 3, 6, 7, 2, 6, 4, 6, 4, 5, 1, 5, 5, 7, 3, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 12, 4, 0, 1, 0, 4, 0, 2, 2, 6, 4, 6, 6, 7, 2, 3, 3, 7, 1, 3, 1, 5, 5, 7, 4, 5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0, 0 },
    { 8, 4, 0, 1, 2, 3, 3, 7, 5, 7, 4, 5, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 9, 5, 4, 6, 0, 4, 4, 5, 5, 7, 3, 7, 6, 7, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 8, 4, 0, 2, 4, 6, 6, 7, 3, 7, 1, 3, 4, 5, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 9, 5, 5, 7, 3, 7, 6, 7, 4, 6, 0, 4, 4, 5, 1, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 7, 5, 4, 6, 0, 2, 0, 1, 4, 5, 5, 7, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 5, 7, 3, 7, 6, 7, 4, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 5, 3, 0, 4, 1, 5, 5, 7, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 2, 0, 1, 1, 5, 5, 7, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 4, 6, 7, 2, 6, 0, 4, 0, 1, 1, 3, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 1, 3, 0, 2, 2, 6, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 2, 0, 4, 1, 5, 5, 7, 6, 7, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 2, 3, 6, 7, 5, 7, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 7, 5, 2, 3, 6, 7, 5, 7, 1, 3, 0, 1, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 4, 2, 1, 3, 2, 3, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 0, 4, 1, 5, 5, 7, 6, 7, 2, 6, 1, 3, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 9, 5, 1, 3, 1, 5, 0, 1, 0, 2, 2, 6, 2, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 7, 5, 2, 3, 0, 1, 0, 4, 2, 6, 6, 7, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 2, 3, 0, 2, 2, 6, 6, 7, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 7, 5, 1, 5, 0, 4, 0, 2, 1, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 1, 5, 0, 1, 1, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 6, 2, 0, 1, 0, 4, 0, 2, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 3, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 0, 1, 0, 4, 0, 2, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 1, 5, 0, 1, 1, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 1, 5, 0, 4, 0, 2, 1, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 2, 3, 0, 2, 2, 6, 6, 7, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 2, 3, 0, 1, 0, 4, 2, 6, 6, 7, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 1, 3, 1, 5, 0, 1, 0, 2, 2, 6, 2, 3, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 0, 4, 2, 6, 2, 3, 1, 3, 1, 5, 6, 7, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 4, 2, 1, 3, 2, 3, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 2, 3, 6, 7, 5, 7, 1, 3, 0, 1, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 2, 3, 6, 7, 5, 7, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 5, 7, 1, 5, 0, 4, 0, 2, 2, 3, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 1, 3, 0, 2, 2, 6, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 6, 7, 5, 7, 1, 3, 0, 1, 0, 4, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 4, 6, 7, 5, 7, 1, 5, 0, 1, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 0, 4, 1, 5, 5, 7, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 2, 5, 7, 3, 7, 6, 7, 4, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 4, 6, 0, 2, 0, 1, 4, 5, 5, 7, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 9, 3, 5, 7, 3, 7, 6, 7, 4, 6, 0, 4, 4, 5, 1, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 0, 2, 1, 3, 1, 5, 4, 5, 4, 6, 3, 7, 6, 7, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 9, 3, 4, 6, 0, 4, 4, 5, 5, 7, 3, 7, 6, 7, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 0, 1, 4, 5, 4, 6, 2, 6, 2, 3, 5, 7, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 12, 4, 1, 3, 1, 5, 0, 1, 0, 4, 4, 5, 4, 6, 0, 2, 2, 6, 2, 3, 3, 7, 6, 7, 5, 7, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0, 0 },
    { 9, 5, 6, 7, 2, 6, 4, 6, 4, 5, 1, 5, 5, 7, 3, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 7, 3, 5, 7, 1, 3, 2, 3, 6, 7, 4, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 4, 5, 4, 6, 0, 2, 0, 1, 6, 7, 5, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 8, 4, 2, 3, 6, 7, 5, 7, 1, 5, 0, 1, 4, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 6, 7, 2, 3, 0, 2, 4, 6, 4, 5, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 8, 4, 1, 3, 0, 2, 2, 6, 6, 7, 5, 7, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 4, 5, 0, 1, 1, 3, 5, 7, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 9, 5, 4, 5, 1, 5, 5, 7, 6, 7, 2, 6, 4, 6, 0, 4, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 6, 4, 4, 5, 1, 5, 5, 7, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 4, 2, 4, 5, 1, 5, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 1, 5, 3, 7, 6, 7, 4, 5, 0, 4, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 4, 5, 0, 1, 1, 3, 3, 7, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 3, 7, 6, 7, 4, 5, 0, 4, 0, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 3, 6, 7, 4, 5, 1, 5, 3, 7, 2, 3, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 2, 6, 2, 3, 0, 1, 0, 4, 3, 7, 6, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 8, 4, 4, 5, 0, 1, 1, 3, 3, 7, 6, 7, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 2, 6, 0, 4, 4, 5, 6, 7, 3, 7, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 5, 3, 6, 7, 4, 5, 1, 5, 1, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 6, 7, 4, 5, 1, 5, 1, 3, 2, 3, 0, 4, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 4, 2, 0, 1, 2, 3, 6, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 6, 7, 2, 3, 0, 2, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 1, 5, 1, 3, 0, 2, 2, 6, 6, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 4, 5, 6, 7, 2, 6, 0, 4, 0, 1, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 5, 3, 4, 5, 6, 7, 2, 6, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 6, 7, 2, 6, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 1, 5, 3, 7, 6, 7, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 6, 7, 4, 6, 0, 2, 0, 1, 1, 5, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 6, 4, 3, 7, 6, 7, 4, 6, 0, 4, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 0, 2, 4, 6, 6, 7, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 1, 5, 3, 7, 6, 7, 4, 6, 0, 4, 2, 3, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 3, 7, 1, 5, 0, 1, 2, 3, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 9, 5, 2, 3, 3, 7, 1, 3, 0, 1, 0, 4, 0, 2, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 6, 4, 3, 7, 1, 3, 2, 3, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 6, 4, 4, 6, 0, 4, 1, 5, 1, 3, 2, 3, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 0, 2, 4, 6, 6, 7, 2, 3, 1, 3, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 5, 3, 2, 3, 0, 1, 0, 4, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 4, 6, 6, 7, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 5, 0, 2, 1, 3, 1, 5, 0, 4, 4, 6, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 2, 1, 3, 1, 5, 0, 1, 2, 6, 4, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 3, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 2, 0, 1, 0, 4, 4, 6, 6, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 3, 1, 6, 7, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 2, 6, 4, 6, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 4, 6, 5, 7, 3, 7, 2, 6, 0, 2, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 7, 3, 3, 7, 2, 6, 4, 6, 5, 7, 1, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 0, 4, 0, 2, 1, 3, 1, 5, 2, 6, 4, 6, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 0, 0 },
    { 5, 3, 4, 6, 5, 7, 3, 7, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 3, 7, 2, 3, 0, 1, 0, 4, 4, 6, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 8, 4, 4, 6, 5, 7, 3, 7, 2, 3, 0, 2, 1, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 5, 7, 4, 6, 0, 4, 1, 5, 1, 3, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 5, 3, 5, 7, 1, 3, 2, 3, 2, 6, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 5, 7, 1, 3, 2, 3, 2, 6, 4, 6, 0, 1, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 6, 4, 1, 5, 0, 1, 2, 3, 2, 6, 4, 6, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 0, 4, 1, 5, 5, 7, 4, 6, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 4, 2, 0, 2, 4, 6, 5, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 5, 7, 4, 6, 0, 4, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 4, 6, 0, 2, 0, 1, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 4, 6, 0, 4, 1, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 2, 6, 0, 4, 4, 5, 5, 7, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 5, 7, 3, 7, 2, 6, 0, 2, 0, 1, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 8, 4, 2, 6, 0, 4, 4, 5, 5, 7, 3, 7, 0, 1, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 7, 5, 1, 3, 0, 2, 2, 6, 3, 7, 5, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 5, 7, 3, 7, 2, 3, 0, 2, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 5, 3, 0, 1, 2, 3, 3, 7, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 9, 5, 0, 1, 0, 4, 0, 2, 2, 3, 3, 7, 1, 3, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 6, 4, 5, 7, 4, 5, 1, 5, 1, 3, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 6, 4, 2, 3, 2, 6, 0, 4, 4, 5, 5, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 1, 3, 5, 7, 4, 5, 0, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 7, 5, 0, 4, 2, 6, 2, 3, 0, 1, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 2, 0, 2, 2, 6, 2, 3, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 3, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 1, 3, 5, 7, 4, 5, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 5, 7, 4, 5, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 4, 0, 2, 0, 1, 1, 5, 5, 7, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 3, 1, 5, 7, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 3, 7, 2, 6, 4, 6, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 0, 0, 0, 0, 0 },
    { 8, 4, 3, 7, 2, 6, 4, 6, 4, 5, 1, 5, 0, 2, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 5, 6, 7, 0, 0, 0 },
    { 6, 4, 1, 3, 3, 7, 2, 6, 4, 6, 4, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 2, 6, 3, 7, 1, 3, 0, 2, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 4, 2, 3, 0, 2, 4, 6, 4, 5, 1, 5, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 7, 5, 0, 1, 2, 3, 3, 7, 1, 5, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 7, 5, 0, 1, 4, 5, 4, 6, 0, 2, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 6, 5, 1, 0, 5, 2, 1, 5, 3, 2, 5, 4, 3 },
    { 6, 2, 2, 3, 3, 7, 1, 3, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 3, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 1, 3, 2, 3, 2, 6, 4, 6, 4, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 0, 0 },
    { 9, 5, 0, 2, 2, 6, 2, 3, 1, 3, 1, 5, 0, 1, 0, 4, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1, 4, 3, 1, 7, 4, 1, 8, 7, 0, 5, 6 },
    { 5, 3, 0, 1, 4, 5, 4, 6, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 4, 4, 5, 4, 6, 2, 6, 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 5, 3, 0, 2, 1, 3, 1, 5, 4, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 1, 1, 3, 1, 5, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 4, 2, 4, 5, 4, 6, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 4, 5, 4, 6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 4, 1, 5, 3, 7, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 3, 7, 1, 5, 0, 1, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 2, 6, 3, 7, 1, 3, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 3, 7, 1, 3, 0, 2, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 1, 5, 0, 4, 0, 2, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 1, 5, 0, 1, 2, 3, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 0, 1, 0, 4, 0, 2, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 3, 1, 2, 3, 3, 7, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 3, 0, 4, 2, 6, 2, 3, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 1, 3, 1, 5, 0, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 0, 0, 0 },
    { 4, 2, 2, 6, 2, 3, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 0, 2, 2, 6, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 2, 0, 4, 0, 2, 1, 3, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 0, 1, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
}
