// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.1.1 (2018/10/05)

#include "FoucaultPendulumWindow.h"

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

    Window::Parameters parameters(L"FoucaultPendulumWindow", 0, 0, 640, 480);
    auto window = TheWindowSystem.Create<FoucaultPendulumWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<FoucaultPendulumWindow>(window);
    return 0;
}

FoucaultPendulumWindow::FoucaultPendulumWindow(Parameters& parameters)
    :
    Window3(parameters),
    mNextPoint(0),
    mColorDiff(0.0f)
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    mEngine->SetClearColor({ 0.819607f, 0.909803f, 0.713725f, 1.0f });
    mWireState = std::make_shared<RasterizerState>();
    mWireState->fillMode = RasterizerState::FILL_WIREFRAME;

    CreateScene();

    float angle = static_cast<float>(0.1 * GTE_C_PI);
    float cs = std::cos(angle);
    float sn = std::sin(angle);
    InitializeCamera(60.0f, GetAspectRatio(), 0.1f, 100.0f, 0.01f, 0.001f,
        { 23.0f, 0.0f, 8.0f }, { -cs, 0.0f, -sn }, { -sn, 0.0f, cs });
    mPVWMatrices.Update();
}

void FoucaultPendulumWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

#if !defined(FOUCAULT_PENDULUM_SINGLE_STEP)
    PhysicsTick();
#endif
    GraphicsTick();

    mTimer.UpdateFrameCount();
}

bool FoucaultPendulumWindow::OnCharPress(unsigned char key, int x, int y)
{
#if defined(FOUCAULT_PENDULUM_SINGLE_STEP)
    if (key == 'g' || key == 'G')
    {
        PhysicsTick();
        return true;
    }
#endif
    if (key == 'w' || key == 'W')
    {
        if (mWireState == mEngine->GetRasterizerState())
        {
            mEngine->SetDefaultRasterizerState();
        }
        else
        {
            mEngine->SetRasterizerState(mWireState);
        }
        return true;
    }
    return Window3::OnCharPress(key, x, y);
}

bool FoucaultPendulumWindow::SetEnvironment()
{
    std::string path = GetGTEPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "/Samples/Data/");

    if (mEnvironment.GetPath("Wood.png") == "")
    {
        LogError("Cannot find file Wood.png");
        return false;
    }

    return true;
}

void FoucaultPendulumWindow::InitializeModule()
{
    mModule.angularSpeed = 0.0001f;
    mModule.latitude = static_cast<float>(GTE_C_QUARTER_PI);
    mModule.gDivL = 1.0f;

    float time = 0.0f;
    float deltaTime = 0.001f;
    float theta = 0.0f;
    float phi = 0.75f;
    float thetaDot = 0.1f;
    float phiDot = 0.0f;
    mModule.Initialize(time, deltaTime, theta, phi, thetaDot, phiDot);
}

void FoucaultPendulumWindow::CreateScene()
{
    InitializeModule();

    mScene = std::make_shared<Node>();
    mTrackball.Attach(mScene);

    CreateFloor();
    CreatePath();
    CreatePendulum();

    PhysicsTick();
}

void FoucaultPendulumWindow::CreateFloor()
{
    auto texture = WICFileIO::Load(mEnvironment.GetPath("Wood.png"), true);
    texture->AutogenerateMipmaps();
    auto effect = std::make_shared<Texture2Effect>(mProgramFactory, texture,
        SamplerState::MIN_L_MAG_L_MIP_L, SamplerState::CLAMP,
        SamplerState::CLAMP);

    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);

    MeshFactory mf;
    mf.SetVertexFormat(vformat);
    auto floor = mf.CreateRectangle(2, 2, 32.0f, 32.0f);
    floor->SetEffect(effect);
    mPVWMatrices.Subscribe(floor->worldTransform, effect->GetPVWMatrixConstant());
    mVisuals.push_back(floor);

    mScene->AttachChild(floor);
}

void FoucaultPendulumWindow::CreatePath()
{
    // The points used to display the path of the pendulum.
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_COLOR, DF_R32G32B32A32_FLOAT, 0);

    unsigned int const numPoints = 8192;
    auto vbuffer = std::make_shared<VertexBuffer>(vformat, numPoints);
    vbuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    memset(vbuffer->GetData(), 0, vbuffer->GetNumBytes());

    auto ibuffer = std::make_shared<IndexBuffer>(IP_POLYPOINT, numPoints);
    ibuffer->SetNumActivePrimitives(0);

    auto effect = std::make_shared<VertexColorEffect>(mProgramFactory);

    mPath = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mPVWMatrices.Subscribe(mPath->worldTransform, effect->GetPVWMatrixConstant());
    mVisuals.push_back(mPath);

    mNextPoint = 0;
    mColorDiff = 1.0f / static_cast<float>(numPoints);

    mScene->AttachChild(mPath);
}

void FoucaultPendulumWindow::CreatePendulum()
{
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);
    MeshFactory mf;
    mf.SetVertexFormat(vformat);

    // The pendulum rod.  Translate the pendulum joint to the origin for
    // the purpose of rotation.
    auto rod = mf.CreateCylinderOpen(2, 8, 0.05f, 12.0f);
    rod->localTransform.SetTranslation(0.0f, 0.0f, 10.0f);
    auto vbuffer = rod->GetVertexBuffer();
    unsigned int numVertices = vbuffer->GetNumElements();
    VertexPT* vertices = vbuffer->Get<VertexPT>();
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        vertices[i].position[2] -= 16.0f;
    }

    // The pendulum bulb.  Start with a sphere (to get the connectivity) and
    // then adjust the vertices to form a pair of joined cones.  Translate the
    // pendulum joint to the origin for the purpose of rotation.
    auto bulb = mf.CreateSphere(16, 32, 2.0f);
    vbuffer = bulb->GetVertexBuffer();
    numVertices = vbuffer->GetNumElements();
    vertices = vbuffer->Get<VertexPT>();
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        Vector3<float>& position = vertices[i].position;
        float r = std::sqrt(position[0] * position[0] + position[1] * position[1]);
        float z = (position[2] + 2.0f >= 2.0f ? 4.0f - r : r);
        position[2] = z - 16.0f;
    }

    // Group the objects into a single subtree.
    mPendulum = std::make_shared<Node>();
    mPendulum->AttachChild(rod);
    mPendulum->AttachChild(bulb);
    mScene->AttachChild(mPendulum);

    // Translate back to the original model position.
    mPendulum->localTransform.SetTranslation(0.0f, 0.0f, 16.0f);

    // Use a color gradient texture for visualization.
    unsigned int const height = 256;
    auto texture = std::make_shared<Texture2>(DF_R8G8B8A8_UNORM, 1, height);
    Vector4<float> color{ 0.99607f, 0.83920f, 0.67059f, 1.0f };
    std::array<uint8_t, 4>* texels = texture->Get<std::array<uint8_t, 4>>();
    float const multiplier = 255.0f / static_cast<float>(height - 1);
    for (unsigned int i = 0; i < height; ++i)
    {
        float t = static_cast<float>(i) * multiplier;
        for (int j = 0; j < 3; ++j)
        {
            texels[i][j] = static_cast<uint8_t>(color[j] * t);
        }
        texels[i][3] = 255;
    }

    auto effect = std::make_shared<Texture2Effect>(mProgramFactory, texture,
        SamplerState::MIN_L_MAG_L_MIP_P, SamplerState::CLAMP, SamplerState::CLAMP);
    rod->SetEffect(effect);
    mPVWMatrices.Subscribe(rod->worldTransform, effect->GetPVWMatrixConstant());
    mVisuals.push_back(rod);

    effect = std::make_shared<Texture2Effect>(mProgramFactory, texture,
        SamplerState::MIN_L_MAG_L_MIP_P, SamplerState::CLAMP, SamplerState::CLAMP);
    bulb->SetEffect(effect);
    mPVWMatrices.Subscribe(bulb->worldTransform, effect->GetPVWMatrixConstant());
    mVisuals.push_back(bulb);
}

void FoucaultPendulumWindow::PhysicsTick()
{
    mModule.Update();

    // Update the pendulum mechanism.  The pendulum rod is attached at
    // (x,y,z) = (0,0,16).  The update here has the z-translation value 16
    // hard-coded.
    mPendulum->localTransform.SetRotation(mModule.GetOrientation());
    mPendulum->Update();

    // Draw only the active quantity of pendulum points for the initial
    // portion of the simulation.  Once all points are activated, then all
    // are drawn.
    auto const& ibuffer = mPath->GetIndexBuffer();
    unsigned int numActive = ibuffer->GetNumActivePrimitives() + 1;
    if (numActive > ibuffer->GetNumPrimitives())
    {
        numActive = ibuffer->GetNumPrimitives();
    }
    ibuffer->SetNumActivePrimitives(numActive);

    // Add the new pendulum point to the point system.  The initial color is
    // white.  All previously known points have their colors decremented to
    // cause them to become dim over time.
    Vector4<float> translation{ 0.0f, 0.0f, -16.0f, 1.0f };
#if defined(GTE_USE_MAT_VEC)
    Vector4<float> proj = mPendulum->worldTransform * translation;
#else
    Vector4<float> proj = translation * mPendulum->worldTransform;
#endif
    proj[2] = 0.0f;

    auto const& vbuffer = mPath->GetVertexBuffer();
    unsigned int const numVertices = vbuffer->GetNumElements();
    VertexPC* vertices = vbuffer->Get<VertexPC>();
    vertices[mNextPoint].position = HProject(proj);
    vertices[mNextPoint].color = { 1.0f, 1.0f, 1.0f, 1.0f };
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        if (i != mNextPoint)
        {
            Vector4<float>& color = vertices[i].color;
            color[0] -= mColorDiff;
            color[1] -= mColorDiff;
            color[2] -= mColorDiff;
        }
    }
    mEngine->Update(vbuffer);

    // Prepare for the next pendulum point.
    if (++mNextPoint == numVertices)
    {
        mNextPoint = 0;
    }
    mPath->Update();

    mPVWMatrices.Update();
}

void FoucaultPendulumWindow::GraphicsTick()
{
    mEngine->ClearBuffers();
    for (auto const& visual : mVisuals)
    {
        mEngine->Draw(visual);
    }
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);
}
