// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.3 (2016/08/29)

#include "RopeWindow.h"

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

    Window::Parameters parameters(L"RopeWindow", 0, 0, 800, 600);
    auto window = TheWindowSystem.Create<RopeWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<RopeWindow>(window);
    return 0;
}

RopeWindow::RopeWindow(Parameters& parameters)
    :
    Window3(parameters)
{
    if (!SetEnvironment())
    {
        parameters.created = false;
        return;
    }

    mWireState = std::make_shared<RasterizerState>();
    mWireState->fillMode = RasterizerState::FILL_WIREFRAME;
    mEngine->SetClearColor({ 0.75f, 0.85f, 0.95f, 1.0f });

    CreateSprings();
    CreateRope();
    InitializeCamera(60.0f, GetAspectRatio(), 0.1f, 1000.0f, 0.001f, 0.001f,
        { 0.0f, 1.25f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    mPVWMatrices.Update();

    mTime0 = std::chrono::high_resolution_clock::now();
}

void RopeWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    // Clamp to 120 frames per second.
    mTime1 = std::chrono::high_resolution_clock::now();
    int64_t delta = std::chrono::duration_cast<std::chrono::milliseconds>(
        mTime1 - mTime0).count();

    if (120 * delta >= 1000)
    {
        mTime0 = mTime1;
        PhysicsTick();
        GraphicsTick();
    }

    mTimer.UpdateFrameCount();
}

bool RopeWindow::OnCharPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':  // toggle wireframe
    case 'W':
        if (mWireState != mEngine->GetRasterizerState())
        {
            mEngine->SetRasterizerState(mWireState);
        }
        else
        {
            mEngine->SetDefaultRasterizerState();
        }
        return true;
    case 'm':  // decrease mass
        if (mModule->GetMass(1) > 0.05f)
        {
            for (int i = 1; i < mModule->GetNumParticles() - 1; ++i)
            {
                mModule->SetMass(i, mModule->GetMass(i) - 0.01f);
            }
        }
        return true;
    case 'M':  // increase mass
        for (int i = 1; i < mModule->GetNumParticles() - 1; ++i)
        {
            mModule->SetMass(i, mModule->GetMass(i) + 0.01f);
        }
        return true;
    case 'c':  // decrease spring constant
        if (mModule->GetConstant(0) > 0.05f)
        {
            for (int i = 0; i < mModule->GetNumSprings(); ++i)
            {
                mModule->SetConstant(i, mModule->GetConstant(i) - 0.01f);
            }
        }
        return true;
    case 'C':  // increase spring constant
        for (int i = 0; i < mModule->GetNumSprings(); ++i)
        {
            mModule->SetConstant(i, mModule->GetConstant(i) + 0.01f);
        }
        return true;
    case 'l':  // decrease spring resting length
        if (mModule->GetLength(0) > 0.05f)
        {
            for (int i = 0; i < mModule->GetNumSprings(); ++i)
            {
                mModule->SetLength(i, mModule->GetLength(i) - 0.01f);
            }
        }
        return true;
    case 'L':  // increase spring resting length
        for (int i = 0; i < mModule->GetNumSprings(); ++i)
        {
            mModule->SetLength(i, mModule->GetLength(i) + 0.01f);
        }
        return true;
    case 'f':  // toggle wind force on/off
    case 'F':
        mModule->enableWind = !mModule->enableWind;
        return true;
    case 'r':  // toggle random wind direction change on/off
    case 'R':
        mModule->enableWindChange = !mModule->enableWindChange;
        return true;
    }

    return Window::OnCharPress(key, x, y);
}

bool RopeWindow::SetEnvironment()
{
    std::string path = GetGTEPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "/Samples/Data/");

    if (mEnvironment.GetPath("Rope.png") == "")
    {
        LogError("Cannot find file Rope.png.");
        return false;
    }

    return true;
}

void RopeWindow::CreateSprings()
{
    int numParticles = 8;
    float step = 0.1f;
    Vector3<float> gravity{ 0.0f, 0.0f, -1.0f };
    Vector3<float> wind{ 0.0f, -0.25f, 0.0f };
    float windChangeAmplitude = 0.01f;
    float viscosity = 10.0f;
    mModule = std::make_unique<PhysicsModule>(numParticles, step, gravity,
        wind, windChangeAmplitude, viscosity);

    // Constant mass at interior points (endpoints are immovable).
    mModule->SetMass(0, std::numeric_limits<float>::max());
    mModule->SetMass(numParticles - 1, std::numeric_limits<float>::max());
    for (int i = 1; i < numParticles - 1; ++i)
    {
        mModule->SetMass(i, 1.0f);
    }

    // Initial position on a horizontal line segment.
    float factor = 1.0f / (float)(numParticles - 1);
    for (int i = 0; i < numParticles; ++i)
    {
        mModule->SetPosition(i, { i * factor, 0.0f, 1.0f });
    }

    // Initial velocities are all zero.
    for (int i = 0; i < numParticles; ++i)
    {
        mModule->SetVelocity(i, Vector3<float>::Zero());
    }

    // Springs are at rest in the initial horizontal configuration.
    int numSprings = numParticles - 1;
    float restLength = 1.0f / (float)numSprings;
    for (int i = 0; i < numSprings; ++i)
    {
        mModule->SetConstant(i, 10.0f);
        mModule->SetLength(i, restLength);
    }
}

void RopeWindow::CreateRope()
{
    MeshDescription desc(MeshTopology::CYLINDER, 64, 8);

    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);
    auto vbuffer = std::make_shared<VertexBuffer>(vformat, desc.numVertices);
    vbuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    Vertex* vertices = vbuffer->Get<Vertex>();
    auto ibuffer = std::make_shared<IndexBuffer>(IP_TRIMESH, desc.numTriangles, sizeof(unsigned int));

    desc.vertexAttributes =
    {
        VertexAttribute("position", &vertices[0].position, sizeof(Vertex)),
        VertexAttribute("tcoord", &vertices[0].tcoord, sizeof(Vertex))
    };

    desc.indexAttribute = IndexAttribute(ibuffer->GetData(), ibuffer->GetElementSize());

    BasisFunctionInput<float> input(mModule->GetNumParticles(), 2);
    mSpline = std::make_shared<BSplineCurve<3, float>>(input, &mModule->GetPosition(0));
    mSurface = std::make_unique<TubeMesh<float>>(desc, mSpline,
        [](float){ return 0.025f; }, false, false, Vector3<float>{ 0.0f, 0.0f, 1.0f });

    std::string path = mEnvironment.GetPath("Rope.png");
    std::shared_ptr<Texture2> texture = WICFileIO::Load(path, true);
    texture->AutogenerateMipmaps();
    std::shared_ptr<Texture2Effect> effect =
        std::make_shared<Texture2Effect>(mProgramFactory, texture,
        SamplerState::MIN_L_MAG_L_MIP_L, SamplerState::WRAP,
        SamplerState::WRAP);

    mRope = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mRope->UpdateModelBound();
    mRope->localTransform.SetTranslation(-mRope->modelBound.GetCenter());
    mPVWMatrices.Subscribe(mRope->worldTransform, effect->GetPVWMatrixConstant());

    mTrackball.Attach(mRope);
    mTrackball.Update();
}

void RopeWindow::PhysicsTick()
{
    // Forces are independent of time, just pass in t = 0.
    mModule->Update(0.0f);

    // Update spline curve.  Remember that the spline maintains its own copy
    // of the control points, so this update is necessary.
    int numControls = mModule->GetNumParticles();
    for (int i = 0; i < numControls; ++i)
    {
        mSpline->SetControl(i, mModule->GetPosition(i));
    }

    // Update the GPU copy of the vertices.
    mSurface->Update();
    mEngine->Update(mRope->GetVertexBuffer());
    mPVWMatrices.Update();
}

void RopeWindow::GraphicsTick()
{
    mEngine->ClearBuffers();
    mEngine->Draw(mRope);
    mEngine->DisplayColorBuffer(0);
}
