// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

uniform PVWMatrix
{
    mat4 pvwMatrix;
};

const vec4 color[4] =
{
    vec4(0.0f, 0.0f, 0.5f, 1.0f),
    vec4(0.5f, 0.0f, 0.0f, 1.0f),
    vec4(0.0f, 0.5f, 0.5f, 1.0f),
    vec4(0.5f, 0.5f, 0.0f, 1.0f)
};

buffer positions { vec4 data[]; } positionsSB;
buffer colorIndices { uint data[]; } colorIndicesSB;

layout(location = 0) out vec4 vertexColor;

void main()
{
#if GTE_USE_MAT_VEC
    gl_Position = pvwMatrix * positionsSB.data[gl_VertexID];
#else
    gl_Position = positionsSB.data[gl_VertexID] * pvwMatrix;
#endif
    vertexColor = color[colorIndicesSB.data[gl_VertexID]];
}
