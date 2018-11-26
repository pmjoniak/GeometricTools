// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.1 (2018/10/05)

#include "ApproximateEllipsesByArcsWindow.h"

int main(int, char const*[])
{
#if defined(_DEBUG)
    LogReporter reporter(
        "LogReport.txt",
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL,
        Logger::Listener::LISTEN_FOR_ALL);
#endif

    Window::Parameters parameters(L"ApproximateEllipsesByArcsWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<ApproximateEllipsesByArcsWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<ApproximateEllipsesByArcsWindow>(window);
    return 0;
}

ApproximateEllipsesByArcsWindow::ApproximateEllipsesByArcsWindow(Parameters& parameters)
    :
    Window2(parameters),
    mA(2.0f),
    mB(1.0f),
    mNumArcs(2),
    mMultiplier(0.375f * mXSize / mA),
    mOffset(Vector2<float>{ 0.5f * mXSize, 0.5f * mYSize })
{
    ApproximateEllipseByArcs(mA, mB, mNumArcs, mPoints, mCenters, mRadii);
    OnDisplay();
}

void ApproximateEllipsesByArcsWindow::OnDisplay()
{
    ClearScreen(0xFFFFFFFF);

    // Draw the ellipse itself.
    int xCenter = mXSize / 2;
    int yCenter = mYSize / 2;
    int xExtent = static_cast<int>(mMultiplier * mA);
    int yExtent = static_cast<int>(mMultiplier * mB);
    DrawEllipse(xCenter, yCenter, xExtent, yExtent, 0xFFFF0000);

    // Draw the circular arcs that approximate the ellipse.  The drawing is
    // inefficient (not the concern of this sample application) in that it
    // overdraws pixels (unlike a Bresenham-style algorithm).
    int const numArcSamples = mXSize;
    for (int i = 0; i < mNumArcs; ++i)
    {
        // Get the arc endpoints, center, and radius in screen coordinates.
        Vector2<float> p0 = mPoints[i];
        Vector2<float> p1 = mPoints[i + 1];
        Vector2<float> center = mCenters[i];
        float radius = mRadii[i];

        // Compute the angle between p0 and p1.
        Vector2<float> v0 = p0 - center;
        Vector2<float> v1 = p1 - center;
        float cosAngle = Dot(v0, v1) / (radius * radius);
        cosAngle = std::max(std::min(cosAngle, (float)GTE_C_PI), 0.0f);
        float angle = acos(cosAngle);
        float relativeAngle = angle / static_cast<float>(numArcSamples);

        // Draw the arc in the first quadrant.
        for (int j = 0; j <= numArcSamples; ++j)
        {
            float t = j * relativeAngle;
            float cs = std::cos(t), sn = std::sin(t);
            Matrix2x2<float> rot{ cs, -sn, sn, cs };
            Vector2<float> p = center + rot * v0;
            Vector2<float> pScreen = mMultiplier * p + mOffset;
            int x = static_cast<int>(pScreen[0]);
            int y = static_cast<int>(pScreen[1]);
            int rx = mXSize - 1 - x;
            int ry = mYSize - 1 - y;
            SetPixel(x, y, 0xFF0000FF);
            SetPixel(x, ry, 0xFF0000FF);
            SetPixel(rx, y, 0xFF0000FF);
            SetPixel(rx, ry, 0xFF0000FF);
        }
    }

    // Draw the arc endpoints.
    for (auto const& p : mPoints)
    {
        Vector2<float> pScreen = mMultiplier * p + mOffset;
        int x = static_cast<int>(pScreen[0]);
        int y = static_cast<int>(pScreen[1]);
        int rx = mXSize - 1 - x;
        int ry = mYSize - 1 - y;
        DrawThickPixel(x, y, 1, 0xFF000000);
        DrawThickPixel(x, ry, 1, 0xFF000000);
        DrawThickPixel(rx, y, 1, 0xFF000000);
        DrawThickPixel(rx, ry, 1, 0xFF000000);
    }

    mScreenTextureNeedsUpdate = true;
    Window2::OnDisplay();
    std::string message = "number of arcs = " + std::to_string(mNumArcs);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, message);
    mEngine->DisplayColorBuffer(0);
}

bool ApproximateEllipsesByArcsWindow::OnCharPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '+':
    case '=':
        if (mNumArcs < 32)
        {
            ++mNumArcs;
            ApproximateEllipseByArcs(mA, mB, mNumArcs, mPoints, mCenters, mRadii);
            OnDisplay();
        }
        return true;

    case '-':
    case '_':
        if (mNumArcs > 2)
        {
            --mNumArcs;
            ApproximateEllipseByArcs(mA, mB, mNumArcs, mPoints, mCenters, mRadii);
            OnDisplay();
        }
        return true;
    }

    return Window2::OnCharPress(key, x, y);
}
