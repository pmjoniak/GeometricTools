// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <Graphics/GteShader.h>

namespace gte
{

class GTE_IMPEXP GeometryShader : public Shader
{
public:
    // Construction.
#if defined(GTE_DEV_OPENGL)
    GeometryShader(GLSLReflection const& reflector);
#else
    GeometryShader(HLSLShader const& program);
#endif
};

}
