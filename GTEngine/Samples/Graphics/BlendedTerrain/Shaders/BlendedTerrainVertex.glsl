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

uniform FlowDirection
{
    vec2 flowDirection;
};

layout(location = 0) in vec3 modelPosition;
layout(location = 1) in vec2 modelGroundTCoord;
layout(location = 2) in float modelBlendTCoord;
layout(location = 3) in vec2 modelCloudTCoord;

layout(location = 0) out vec2 vertexGroundTCoord;
layout(location = 1) out float vertexBlendTCoord;
layout(location = 2) out vec2 vertexCloudTCoord;
layout(location = 3) out vec2 vertexFlowDirection;

void main()
{
    // Pass through the texture coordinates.
    vertexGroundTCoord = modelGroundTCoord;
    vertexBlendTCoord = modelBlendTCoord;
    vertexCloudTCoord = modelCloudTCoord;

    // Pass through flow direction, to be used as an offset in the pixel program.
    vertexFlowDirection = flowDirection;

    // Transform the position from model space to clip space.
#if GTE_USE_MAT_VEC
    gl_Position = pvwMatrix * vec4(modelPosition, 1.0f);
#else
    gl_Position = vec4(modelPosition, 1.0f) * pvwMatrix;
#endif
}
