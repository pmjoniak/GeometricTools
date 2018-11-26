// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

uniform PMIParameters
{
    mat4 pvMatrix;
    mat4 wMatrix;

    // The planes are of form Dot(planeVector,(x,y,z,1)) = i for integer i
    // where (x,y,z,1) is the world position.
    vec4 planeVector0;
    vec4 planeVector1;
};

layout(location = 0) in vec3 inModelPosition;

layout(location = 0) out vec3 vertexColor;
layout(location = 1) noperspective out vec2 planeConstant;

void main()
{
    vertexColor = vec3(0.0f, 0.0f, 1.0f);

    // Compute the world position of the vertex.
    vec4 modelPosition = vec4(inModelPosition, 1.0f);
    vec4 worldPosition;
#if GTE_USE_MAT_VEC
    worldPosition = wMatrix * modelPosition;
#else
    worldPosition = modelPosition * wMatrix;
#endif
    // Compute the plane constant c of the vertex.
    planeConstant.x = dot(planeVector0, worldPosition);
    planeConstant.y = dot(planeVector1, worldPosition);

    // Compute the clip position of the vertex.
#if GTE_USE_MAT_VEC
    gl_Position = pvMatrix * worldPosition;
#else
    gl_Position = worldPosition * pvMatrix;
#endif
}
