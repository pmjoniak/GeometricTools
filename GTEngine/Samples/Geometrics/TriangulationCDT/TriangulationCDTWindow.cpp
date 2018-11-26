// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.2 (2018/10/05)

#include "TriangulationCDTWindow.h"
#include <iostream>

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

    Window::Parameters parameters(L"TriangulationCDTWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<TriangulationCDTWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.NO_IDLE_LOOP);
    TheWindowSystem.Destroy<TriangulationCDTWindow>(window);
    return 0;
}

TriangulationCDTWindow::TriangulationCDTWindow(Parameters& parameters)
    :
    Window2(parameters)
{
    mClampToWindow = false;
    mDoFlip = true;
    UnindexedSimplePolygon();
}

bool TriangulationCDTWindow::OnCharPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0':
        UnindexedSimplePolygon();
        OnDisplay();
        return true;
    case '1':
        IndexedSimplePolygon();
        OnDisplay();
        return true;
    case '2':
        OneNestedPolygon();
        OnDisplay();
        return true;
    case '3':
        TwoNestedPolygons();
        OnDisplay();
        return true;
    case '4':
        TreeOfNestedPolygons();
        OnDisplay();
        return true;
    }

    return Window::OnCharPress(key, x, y);
}

void TriangulationCDTWindow::DrawTriangulation()
{
    ClearScreen(0xFFFFFFFF);

    Vector2<float> pmin{ 0.0f, 0.0f }, pmax{ 0.0f, 0.0f };
    ComputeExtremes(static_cast<int>(mPoints.size()), &mPoints[0], pmin, pmax);
    int xmin = static_cast<int>(std::floor(pmin[0]));
    int ymin = static_cast<int>(std::floor(pmin[1]));
    int xmax = static_cast<int>(std::ceil(pmax[0]));
    int ymax = static_cast<int>(std::ceil(pmax[1]));

    int tStart = 0;
    for (int y = ymin; y <= ymax; ++y)
    {
        std::cout << "y = " << y << std::endl;
        for (int x = xmin; x <= xmax; ++x)
        {
            Vector2<float> test{ static_cast<float>(x), static_cast<float>(y) };
            int t = mPMesher->GetContainingTriangle(test, tStart);
            if (t >= 0)
            {
                if (mTriangulator->IsInside(t))
                {
                    SetPixel(x, y, 0xFFFF8000);
                }
                else
                {
                    SetPixel(x, y, 0xFF0080FF);
                }

                tStart = t;
            }
            else
            {
                tStart = 0;
            }
        }
    }

    for (auto const& tri : mTriangulator->GetAllTriangles())
    {
        int v0 = tri[0];
        int v1 = tri[1];
        int v2 = tri[2];

        int x0 = static_cast<int>(mPoints[v0][0]);
        int y0 = static_cast<int>(mPoints[v0][1]);
        int x1 = static_cast<int>(mPoints[v1][0]);
        int y1 = static_cast<int>(mPoints[v1][1]);
        DrawLine(x0, y0, x1, y1, 0xFF000000);

        x0 = static_cast<int>(mPoints[v1][0]);
        y0 = static_cast<int>(mPoints[v1][1]);
        x1 = static_cast<int>(mPoints[v2][0]);
        y1 = static_cast<int>(mPoints[v2][1]);
        DrawLine(x0, y0, x1, y1, 0xFF000000);

        x0 = static_cast<int>(mPoints[v2][0]);
        y0 = static_cast<int>(mPoints[v2][1]);
        x1 = static_cast<int>(mPoints[v0][0]);
        y1 = static_cast<int>(mPoints[v0][1]);
        DrawLine(x0, y0, x1, y1, 0xFF000000);
    }

    mScreenTextureNeedsUpdate = true;
}

void TriangulationCDTWindow::UnindexedSimplePolygon()
{
    mPoints =
    {
        { 58.0f, 278.0f },
        { 156.0f, 198.0f },
        { 250.0f, 282.0f },
        { 328.0f, 232.0f },
        { 402.0f, 336.0f },
        { 314.0f, 326.0f },
        { 274.0f, 400.0f },
        { 196.0f, 268.0f },
        { 104.0f, 292.0f },
        { 110.0f, 382.0f }
    };

    mTriangulator = std::make_unique<Triangulator>(mPoints);
    (*mTriangulator)();
    auto const& triangles = mTriangulator->GetAllTriangles();
    int const numPoints = static_cast<int>(mPoints.size());
    int const numTriangles = static_cast<int>(triangles.size());
    mPMesher = std::make_unique<PlanarMesher>(numPoints, &mPoints[0],
        numTriangles, reinterpret_cast<int const*>(&triangles[0]));

    DrawTriangulation();
}

void TriangulationCDTWindow::IndexedSimplePolygon()
{
    mPoints =
    {
        { 58.0f, 278.0f },
        { 0.0f, 0.0f },
        { 156.0f, 198.0f },
        { 0.0f, 0.0f },
        { 250.0f, 282.0f },
        { 0.0f, 0.0f },
        { 328.0f, 232.0f },
        { 0.0f, 0.0f },
        { 402.0f, 336.0f },
        { 0.0f, 0.0f },
        { 314.0f, 326.0f },
        { 0.0f, 0.0f },
        { 274.0f, 400.0f },
        { 0.0f, 0.0f },
        { 196.0f, 268.0f },
        { 0.0f, 0.0f },
        { 104.0f, 292.0f },
        { 0.0f, 0.0f },
        { 110.0f, 382.0f },
        { 0.0f, 0.0f }
    };

    std::vector<int> outer = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };

    mTriangulator = std::make_unique<Triangulator>(mPoints);
    (*mTriangulator)(outer);
    auto const& triangles = mTriangulator->GetAllTriangles();
    int const numPoints = static_cast<int>(mPoints.size());
    int const numTriangles = static_cast<int>(triangles.size());
    mPMesher = std::make_unique<PlanarMesher>(numPoints, &mPoints[0],
        numTriangles, reinterpret_cast<int const*>(&triangles[0]));

    DrawTriangulation();
}

void TriangulationCDTWindow::OneNestedPolygon()
{
    mPoints =
    {
        { 128.0f, 256.0f },
        { 256.0f, 128.0f },
        { 384.0f, 256.0f },
        { 256.0f, 384.0f },
        { 320.0f, 256.0f },
        { 256.0f, 192.0f },
        { 256.0f, 320.0f }
    };

    std::vector<int> outer = { 0, 1, 2, 3 };
    std::vector<int> inner = { 4, 5, 6 };

    mTriangulator = std::make_unique<Triangulator>(mPoints);
    (*mTriangulator)(outer, inner);
    auto const& triangles = mTriangulator->GetAllTriangles();
    int const numPoints = static_cast<int>(mPoints.size());
    int const numTriangles = static_cast<int>(triangles.size());
    mPMesher = std::make_unique<PlanarMesher>(numPoints, &mPoints[0],
        numTriangles, reinterpret_cast<int const*>(&triangles[0]));

    DrawTriangulation();
}

void TriangulationCDTWindow::TwoNestedPolygons()
{
    mPoints =
    {
        { 58.0f, 278.0f },
        { 156.0f, 198.0f },
        { 250.0f, 282.0f },
        { 328.0f, 232.0f },
        { 402.0f, 336.0f },
        { 314.0f, 326.0f },
        { 274.0f, 400.0f },
        { 196.0f, 268.0f },
        { 104.0f, 292.0f },
        { 110.0f, 382.0f },
        { 280.0f, 336.0f },
        { 327.0f, 283.0f },
        { 240.0f, 317.0f },
        { 106.0f, 256.0f },
        { 152.0f, 255.0f },
        { 201.0f, 249.0f }
    };

    std::vector<int> outer = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<Triangulator::Polygon> inner{ { 11, 12, 10 }, { 13, 14, 15 } };

    mTriangulator = std::make_unique<Triangulator>(mPoints);
    (*mTriangulator)(outer, inner);
    auto const& triangles = mTriangulator->GetAllTriangles();
    int const numPoints = static_cast<int>(mPoints.size());
    int const numTriangles = static_cast<int>(triangles.size());
    mPMesher = std::make_unique<PlanarMesher>(numPoints, &mPoints[0],
        numTriangles, reinterpret_cast<int const*>(&triangles[0]));

    DrawTriangulation();
}

void TriangulationCDTWindow::TreeOfNestedPolygons()
{
    mPoints =
    {
        { 204.0f, 30.0f },
        { 466.0f, 174.0f },
        { 368.0f, 496.0f },
        { 66.0f, 464.0f },
        { 28.0f, 256.0f },
        { 274.0f, 84.0f },
        { 186.0f, 82.0f },
        { 274.0f, 158.0f },
        { 292.0f, 132.0f },
        { 322.0f, 426.0f },
        { 426.0f, 226.0f },
        { 216.0f, 134.0f },
        { 72.0f, 306.0f },
        { 178.0f, 440.0f },
        { 266.0f, 372.0f },
        { 294.0f, 474.0f },
        { 354.0f, 474.0f },
        { 368.0f, 404.0f },
        { 318.0f, 450.0f },
        { 172.0f, 226.0f },
        { 230.0f, 236.0f },
        { 196.0f, 268.0f },
        { 218.0f, 306.0f },
        { 136.0f, 266.0f },
        { 136.0f, 312.0f },
        { 230.0f, 350.0f },
        { 216.0f, 388.0f },
        { 160.0f, 384.0f },
        { 326.0f, 216.0f },
        { 370.0f, 216.0f },
        { 344.0f, 352.0f },
        { 158.0f, 340.0f },
        { 158.0f, 358.0f },
        { 176.0f, 358.0f },
        { 176.0f, 340.0f },
        { 192.0f, 358.0f },
        { 192.0f, 374.0f },
        { 206.0f, 374.0f },
        { 206.0f, 358.0f },
        { 338.0f, 242.0f },
        { 338.0f, 262.0f },
        { 356.0f, 262.0f },
        { 356.0f, 242.0f }
    };

    // The nested tree of polygons has the structure
    //
    // outer0
    //     inner0
    //     inner1
    //         outer3
    //             inner5
    //     inner2
    //         outer1
    //         outer2
    //             inner3
    //             inner4

    // outer0 polygon
    auto outer0 = std::make_shared<Triangulator::Tree>();
    outer0->polygon = { 0, 1, 2, 3, 4 };

    // inner0 polygon
    auto inner0 = std::make_shared<Triangulator::Tree>();
    inner0->polygon = { 5, 6, 7 };
    outer0->child.push_back(inner0);

    // inner1 polygon
    auto inner1 = std::make_shared<Triangulator::Tree>();
    inner1->polygon = { 8, 9, 10 };
    outer0->child.push_back(inner1);

    // inner2 polygon
    auto inner2 = std::make_shared<Triangulator::Tree>();
    inner2->polygon = { 11, 12, 13, 14, 15, 16, 17, 18 };
    outer0->child.push_back(inner2);

    // outer1 polygon (contained in inner2)
    auto outer1 = std::make_shared<Triangulator::Tree>();
    outer1->polygon = { 19, 20, 21, 22, 23 };
    inner2->child.push_back(outer1);

    // outer2 polygon (contained in inner2)
    auto outer2 = std::make_shared<Triangulator::Tree>();
    outer2->polygon = { 24, 25, 26, 27 };
    inner2->child.push_back(outer2);

    // outer3 polygon (contained in inner1)
    auto outer3 = std::make_shared<Triangulator::Tree>();
    outer3->polygon = { 28, 29, 30 };
    inner1->child.push_back(outer3);

    // inner3 polygon (contained in outer2)
    auto inner3 = std::make_shared<Triangulator::Tree>();
    inner3->polygon = { 31, 32, 33, 34 };
    outer2->child.push_back(inner3);

    // inner4 polygon (contained in outer2)
    auto inner4 = std::make_shared<Triangulator::Tree>();
    inner4->polygon = { 35, 36, 37, 38 };
    outer2->child.push_back(inner4);

    // inner5 polygon (contained in outer3)
    auto inner5 = std::make_shared<Triangulator::Tree>();
    inner5->polygon = { 39, 40, 41, 42 };
    outer3->child.push_back(inner5);

    mTriangulator = std::make_unique<Triangulator>(mPoints);
    (*mTriangulator)(outer0);
    auto const& triangles = mTriangulator->GetAllTriangles();
    int const numPoints = static_cast<int>(mPoints.size());
    int const numTriangles = static_cast<int>(triangles.size());
    mPMesher = std::make_unique<PlanarMesher>(numPoints, &mPoints[0],
        numTriangles, reinterpret_cast<int const*>(&triangles[0]));

    DrawTriangulation();
}
