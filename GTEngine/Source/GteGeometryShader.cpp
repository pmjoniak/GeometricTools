// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include "GTEnginePCH.h"
#include "GteGeometryShader.h"
using namespace gte;

//----------------------------------------------------------------------------
GeometryShader::GeometryShader(HLSLShader const& program)
    :
    Shader(program)
{
    mType = GT_GEOMETRY_SHADER;
}
//----------------------------------------------------------------------------
