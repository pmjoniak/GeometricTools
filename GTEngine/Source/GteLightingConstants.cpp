// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.2.1 (2014/12/02)

#include "GTEnginePCH.h"
#include "GteLightingConstants.h"
#include "GteConstants.h"
using namespace gte;

//----------------------------------------------------------------------------
LightingConstants::LightingConstants()
    :
    materialEmissive({ 0.0f, 0.0f, 0.0f, 1.0f }),
    materialAmbient({ 0.0f, 0.0f, 0.0f, 1.0f }),
    materialDiffuse({ 0.0f, 0.0f, 0.0f, 1.0f }),
    materialSpecular({ 0.0f, 0.0f, 0.0f, 1.0f }),
    lightAmbient({ 1.0f, 1.0f, 1.0f, 1.0f }),
    lightDiffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
    lightSpecular({ 1.0f, 1.0f, 1.0f, 1.0f }),
    lightSpotCutoff({ (float)GTE_C_HALF_PI, 0.0f, 1.0f, 1.0f }),
    lightAttenuation({ 1.0f, 0.0f, 0.0f, 1.0f }),
    cameraModelPosition({ 0.0f, 0.0f, 0.0f, 1.0f }),
    lightModelPosition({ 0.0f, 0.0f, 0.0f, 1.0f }),
    lightModelDirection({ 0.0f, 0.0f, 0.0f, 0.0f }),
    wMatrix(Matrix4x4<float>::Identity())
{
}
//----------------------------------------------------------------------------
