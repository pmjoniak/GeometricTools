// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

uniform Reflectivity
{
    float reflectivity;
};

uniform samplerCube cubeSampler;

layout(location = 0) in vec4 vertexColor;
layout(location = 1) in vec3 cubeTCoord;

layout(location = 0) out vec4 pixelColor;

void main()
{
    vec4 reflectedColor = texture(cubeSampler, cubeTCoord);

    // In HLSL lerp(x,y,s) -> x + s*(y-x)
    // pixelColor = lerp(vertexColor, reflectedColor, reflectivity)
    float s = clamp(reflectivity, 0.0f, 1.0f);
    pixelColor = vertexColor + s * (reflectedColor - vertexColor);
}
