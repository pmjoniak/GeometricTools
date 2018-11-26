// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.5.0 (2016/11/27)

#include "DistanceOrientedBoxesWindow.h"

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

    Window::Parameters parameters(L"DistanceOrientedBoxesWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<DistanceOrientedBoxesWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<DistanceOrientedBoxesWindow>(window);
    return 0;
}

DistanceOrientedBoxesWindow::DistanceOrientedBoxesWindow(Parameters& parameters)
    :
    Window3(parameters),
    mActiveBox(nullptr)
{
    mNoCullState = std::make_shared<RasterizerState>();
    mNoCullState->cullMode = RasterizerState::CULL_NONE;
    mEngine->SetRasterizerState(mNoCullState);

    mNoCullWireState = std::make_shared<RasterizerState>();
    mNoCullWireState->cullMode = RasterizerState::CULL_NONE;
    mNoCullWireState->fillMode = RasterizerState::FILL_WIREFRAME;

    mBlendState = std::make_shared<BlendState>();
    mBlendState->target[0].enable = true;
    mBlendState->target[0].srcColor = BlendState::BM_SRC_ALPHA;
    mBlendState->target[0].dstColor = BlendState::BM_INV_SRC_ALPHA;
    mBlendState->target[0].srcAlpha = BlendState::BM_SRC_ALPHA;
    mBlendState->target[0].dstAlpha = BlendState::BM_INV_SRC_ALPHA;
    mEngine->SetBlendState(mBlendState);

    CreateScene();
    InitializeCamera(60.0f, GetAspectRatio(), 1.0f, 5000.0f, 0.1f, 0.01f,
        { 0.0f, 0.0f, -24.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });

    DoQuery();
    mPVWMatrices.Update();
}

void DistanceOrientedBoxesWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    mEngine->ClearBuffers();
    mEngine->Draw(mBox0Mesh);
    mEngine->Draw(mBox1Mesh);
    mEngine->Draw(mSegment);
    mEngine->Draw(mPoint0);
    mEngine->Draw(mPoint1);
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool DistanceOrientedBoxesWindow::OnCharPress(unsigned char key, int x, int y)
{
    float const delta = 0.1f;
    Quaternion<float> q, incr;

    switch (key)
    {
    case 'w':
    case 'W':
        if (mEngine->GetRasterizerState() == mNoCullState)
        {
            mEngine->SetRasterizerState(mNoCullWireState);
        }
        else
        {
            mEngine->SetRasterizerState(mNoCullState);
        }
        return true;

    case ' ':
        DoQuery();
        return true;

    case '0':
        mActiveBox = &mBox0;
        mActiveBoxMesh = mBox0Mesh;
        return true;

    case '1':
        mActiveBox = &mBox1;
        mActiveBoxMesh = mBox1Mesh;
        return true;

    case 'x':  // decrement x-center of box
        Translate(0, -delta);
        return true;

    case 'X':  // increment x-center of box
        Translate(0, +delta);
        return true;

    case 'y':  // decrement y-center of box
        Translate(1, -delta);
        return true;

    case 'Y':  // increment y-center of box
        Translate(1, +delta);
        return true;

    case 'z':  // decrement z-center of box
        Translate(2, -delta);
        return true;

    case 'Z':  // increment z-center of box
        Translate(2, +delta);
        return true;

    case 'p':  // rotate about axis[0]
        Rotate(0, -delta);
        return true;

    case 'P':  // rotate about axis[0]
        Rotate(0, +delta);
        return true;

    case 'r':  // rotate about axis[1]
        Rotate(1, -delta);
        return true;

    case 'R':  // rotate about axis[1]
        Rotate(1, +delta);
        return true;

    case 'h':  // rotate about axis[2]
        Rotate(2, -delta);
        return true;

    case 'H':  // rotate about axis[2]
        Rotate(2, +delta);
        return true;
    }

    return Window::OnCharPress(key, x, y);
}

void DistanceOrientedBoxesWindow::CreateScene()
{
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    MeshFactory mf;
    mf.SetVertexFormat(vformat);

    mBox0.center = { 0.0f, 0.0f, 0.0f };
    mBox0.axis[0] = { 1.0f, 0.0f, 0.0f };
    mBox0.axis[1] = { 0.0f, 1.0f, 0.0f };
    mBox0.axis[2] = { 0.0f, 0.0f, 1.0f };
    mBox0.extent = { 0.5f, 1.0f, 1.5f };

    mBox1.center = { 2.5f, 3.0f, 3.5f };
    mBox1.axis[0] = { 1.0f, 0.0f, 0.0f };
    mBox1.axis[1] = { 0.0f, 1.0f, 0.0f };
    mBox1.axis[2] = { 0.0f, 0.0f, 1.0f };
    mBox1.extent = { 0.5f, 1.0f, 1.5f };

    mBox0Mesh = mf.CreateBox(mBox0.extent[0], mBox0.extent[1], mBox0.extent[2]);
    mBox0Mesh->localTransform.SetTranslation(mBox0.center);

    mBox1Mesh = mf.CreateBox(mBox1.extent[0], mBox1.extent[1], mBox1.extent[2]);
    mBox1Mesh->localTransform.SetTranslation(mBox1.center);

    auto effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.5f, 0.0f, 0.5f }));
    mBox0Mesh->SetEffect(effect);
    mPVWMatrices.Subscribe(mBox0Mesh->worldTransform, effect->GetPVWMatrixConstant());

    mRedEffect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.5f, 0.0f, 0.0f, 0.5f }));

    mBlueEffect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.0f, 0.5f, 0.5f }));

    mBox1Mesh->SetEffect(mBlueEffect);
    mPVWMatrices.Subscribe(mBox1Mesh->worldTransform, mBlueEffect->GetPVWMatrixConstant());

    auto vbuffer = std::make_shared<VertexBuffer>(vformat, 2);
    vbuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    auto ibuffer = std::make_shared<IndexBuffer>(IP_POLYSEGMENT_DISJOINT, 1);
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.0f, 0.0f, 1.0f }));
    mSegment = std::make_shared<Visual>(vbuffer, ibuffer, effect);
    mPVWMatrices.Subscribe(mSegment->worldTransform, effect->GetPVWMatrixConstant());

    mPoint0 = mf.CreateSphere(8, 8, 0.0625f);
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.0f, 0.0f, 1.0f }));
    mPoint0->SetEffect(effect);
    mPVWMatrices.Subscribe(mPoint0->worldTransform, effect->GetPVWMatrixConstant());

    mPoint1 = mf.CreateSphere(8, 8, 0.0625f);
    effect = std::make_shared<ConstantColorEffect>(mProgramFactory,
        Vector4<float>({ 0.0f, 0.0f, 0.0f, 1.0f }));
    mPoint1->SetEffect(effect);
    mPVWMatrices.Subscribe(mPoint1->worldTransform, effect->GetPVWMatrixConstant());

    mTrackball.Attach(mBox0Mesh);
    mTrackball.Attach(mBox1Mesh);
    mTrackball.Attach(mSegment);
    mTrackball.Attach(mPoint0);
    mTrackball.Attach(mPoint1);
    mTrackball.Update();

    mActiveBox = &mBox0;
    mActiveBoxMesh = mBox0Mesh;
}

void DistanceOrientedBoxesWindow::Translate(int direction, float delta)
{
    mActiveBox->center[direction] += delta;
    mActiveBoxMesh->localTransform.SetTranslation(mActiveBox->center);
    mActiveBoxMesh->Update();
    DoQuery();
    mPVWMatrices.Update();
}

void DistanceOrientedBoxesWindow::Rotate(int direction, float delta)
{
    Quaternion<float> incr = Rotation<3, float>(
        AxisAngle<3, float>(mActiveBox->axis[direction], delta));
    for (int i = 0; i < 3; ++i)
    {
        if (i != direction)
        {
            mActiveBox->axis[i] = HProject(
                gte::Rotate(incr, HLift(mActiveBox->axis[i], 0.0f)));
        }
    }
    Quaternion<float> q;
    mActiveBoxMesh->localTransform.GetRotation(q);
    mActiveBoxMesh->localTransform.SetRotation(incr * q);
    mActiveBoxMesh->Update();
    DoQuery();
    mPVWMatrices.Update();
}

void DistanceOrientedBoxesWindow::DoQuery()
{
    mPVWMatrices.Unsubscribe(mBox1Mesh->worldTransform);

    auto result = mQuery(mBox0, mBox1);
    float const epsilon = 1e-04f;
    if (result.distance > epsilon)
    {
        mBox1Mesh->SetEffect(mRedEffect);
        mPVWMatrices.Subscribe(mBox1Mesh->worldTransform, mRedEffect->GetPVWMatrixConstant());
    }
    else
    {
        mBox1Mesh->SetEffect(mBlueEffect);
        mPVWMatrices.Subscribe(mBox1Mesh->worldTransform, mBlueEffect->GetPVWMatrixConstant());
    }

    Vector3<float>* vertices = mSegment->GetVertexBuffer()->Get<Vector3<float>>();
    vertices[0] = result.closestPoint[0];
    vertices[1] = result.closestPoint[1];
    mEngine->Update(mSegment->GetVertexBuffer());

    mPoint0->localTransform.SetTranslation(vertices[0]);
    mPoint1->localTransform.SetTranslation(vertices[1]);
    mTrackball.Update();
}
