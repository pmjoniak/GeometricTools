// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2017/07/02)

uniform PVWMatrix
{
    mat4 pvwMatrix;
};

layout(location = 0) in vec3 modelPosition;
layout(location = 1) in vec2 modelTCoord;

layout(location = 0) out vec2 vertexTCoord;
layout(location = 1) noperspective out float perspectiveDepth;

void main()
{
#if GTE_USE_MAT_VEC
    vec4 clipPosition = pvwMatrix * vec4(modelPosition, 1.0f);
#else
    vec4 clipPosition = vec4(modelPosition, 1.0f) * pvwMatrix;
#endif
    clipPosition.y = -clipPosition.y;
    gl_Position = clipPosition;
    vertexTCoord = modelTCoord;
    perspectiveDepth = clipPosition.z / clipPosition.w;
}
