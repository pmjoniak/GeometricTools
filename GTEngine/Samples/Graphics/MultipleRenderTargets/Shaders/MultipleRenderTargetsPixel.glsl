// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2017/07/02)

uniform FarNearRatio
{
    float farNearRatio;
};

uniform sampler2D baseSampler;

layout(location = 0) in vec2 vertexTCoord;
layout(location = 1) noperspective in float perspectiveDepth;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 screenPosition;

void main()
{
    color = texture(baseSampler, vertexTCoord);
    screenPosition = gl_FragCoord;

    // For OpenGL, perspective depth d is computed from the camera
    // z-value (view direction component) and the near and far plane
    // values n and f using d = (f+n)/(f-n) - 2*f*n/((f-n)*z), where
    // z in [n,f] and d in [-1,+1].  Additionally, the standard
    // glDepthRange(0,1) maps d in [-1,1] to values d' in [0,1].  The
    // values of perspectiveDepth are the interpolated d'-values
    // computed by the rasterizer.  Solve for linear depth
    // L = (z-n)/(f-n) in [0,1] to obtain L = (2-r*(1-d))/(r*(1-d)+2*d)
    // where r = f/n.

    float d = perspectiveDepth;
    float temp = farNearRatio * (1.0f - d);
    gl_FragDepth = (2.0f - temp) / (temp + 2.0f * d);
};
