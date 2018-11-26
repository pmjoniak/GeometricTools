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

layout(location = 0) in vec3 modelPosition;
layout(location = 1) in vec2 modelTCoord;

layout(location = 0) out float vertexHeight;

uniform sampler2D displacementSampler;

void main()
{
    vec4 displacedPosition;
    displacedPosition.xy = modelPosition.xy;
    displacedPosition.z = textureLod(displacementSampler, modelTCoord, 0).x;
    displacedPosition.w = 1.0f;

    vertexHeight = displacedPosition.z;

#if GTE_USE_MAT_VEC
    gl_Position = pvwMatrix * displacedPosition;
#else
    gl_Position = displacedPosition * pvwMatrix;
#endif
}
