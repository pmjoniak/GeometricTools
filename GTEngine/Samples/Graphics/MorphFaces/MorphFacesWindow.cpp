// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#include "MorphFacesWindow.h"

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

    Window::Parameters parameters(L"MorphFacesWindow", 0, 0, 512, 512);
    auto window = TheWindowSystem.Create<MorphFacesWindow>(parameters);
    TheWindowSystem.MessagePump(window, TheWindowSystem.DEFAULT_ACTION);
    TheWindowSystem.Destroy<MorphFacesWindow>(window);
    return 0;
}

MorphFacesWindow::MorphFacesWindow(Parameters& parameters)
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

    CreateScene();
    mScene->Update();
    mMorphResult->localTransform.SetTranslation(-mScene->worldBound.GetCenter());
    InitializeCamera(60.0f, GetAspectRatio(), 1.0f, 1000.0f, 0.01f, 0.001f,
        { 0.0f, -1.5f * mScene->worldBound.GetRadius(), 0.0f },
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    mScene->Update();
    mPVWMatrices.Update();
    mAnimStartTime = mAnimTimer.GetSeconds();
}

void MorphFacesWindow::OnIdle()
{
    mTimer.Measure();

    if (mCameraRig.Move())
    {
        mPVWMatrices.Update();
    }

    // The input time is relative to the starting time of the application.
    UpdateMorph(static_cast<float>(mAnimTimer.GetSeconds() - mAnimStartTime));

    mEngine->ClearBuffers();
    for (auto const& visual : mVisuals)
    {
        mEngine->Draw(visual);
    }
    mEngine->Draw(8, mYSize - 8, { 0.0f, 0.0f, 0.0f, 1.0f }, mTimer.GetFPS());
    mEngine->DisplayColorBuffer(0);

    mTimer.UpdateFrameCount();
}

bool MorphFacesWindow::OnCharPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        if (mWireState == mEngine->GetRasterizerState())
        {
            mEngine->SetDefaultRasterizerState();
        }
        else
        {
            mEngine->SetRasterizerState(mWireState);
        }
        return true;

    case '0':
        mAnimStartTime = mAnimTimer.GetSeconds();
        return true;
    }
    return Window3::OnCharPress(key, x, y);
}

bool MorphFacesWindow::SetEnvironment()
{
    std::string path = GetGTEPath();
    if (path == "")
    {
        return false;
    }

    mEnvironment.Insert(path + "/Samples/Graphics/MorphFaces/Data/");
#if defined(GTE_DEV_OPENGL)
    mEnvironment.Insert(path + "/Samples/Graphics/MorphFaces/Shaders/");
#endif

    std::vector<std::string> inputs =
    {
        "Eye.png",
        "LightColorSampler.txt",
        "M0BasePosNor.txt",
        "M10FullLeftPosNor.txt",
        "M10FullLeftWeights.txt",
        "M11UpNotUsedPosNor.txt",
        "M12DownNotUsedPosNor.txt",
        "M1Viseme01PosNor.txt",
        "M1Viseme01Weights.txt",
        "M2Viseme02PosNor.txt",
        "M2Viseme02Weights.txt",
        "M3Viseme03aPosNor.txt",
        "M3Viseme03aWeights.txt",
        "M3Viseme03bPosNor.txt",
        "M3Viseme03bWeights.txt",
        "M4TheNoLookPosNor.txt",
        "M4TheNoLookWeights.txt",
        "M5SmilePosNor.txt",
        "M5SmileWeights.txt",
        "M6AngerPosNor.txt",
        "M6AngerWeights.txt",
        "M7FullRightPosNor.txt",
        "M7FullRightWeights.txt",
        "M8HalfRightPosNor.txt",
        "M8HalfRightWeights.txt",
        "M9HalfLeftPosNor.txt",
        "M9HalfLeftWeights.txt",
        "SharedTexTri.txt"
#if defined(GTE_DEV_OPENGL)
        ,
        "Texture2PNTVS.glsl",
        "Texture2PNTPS.glsl"
#endif
    };

    for (auto const& input : inputs)
    {
        if (mEnvironment.GetPath(input) == "")
        {
            LogError("Cannot find file " + input);
            return false;
        }
    }

    return true;
}

void MorphFacesWindow::CreateScene()
{
    mScene = std::make_shared<Node>();
    mTrackball.Attach(mScene);
    CreateMorphResult();

    // Load the position/normal data for the morph targets.  Also, create the
    // weight interpolators.
    LoadTarget(0, "M0Base");
    LoadTarget(1, "M1Viseme01");
    LoadTarget(2, "M2Viseme02");
    LoadTarget(3, "M3Viseme03a");
    LoadTarget(4, "M3Viseme03b");
    LoadTarget(5, "M4TheNoLook");
    LoadTarget(6, "M5Smile");
    LoadTarget(7, "M10FullLeft");
    LoadTarget(8, "M7FullRight");
    LoadTarget(9, "M9HalfLeft");
    LoadTarget(10, "M8HalfRight");
    LoadTarget(11, "M6Anger");

    // The color interpolator is used to make the soldier's face red when
    // he is angry.
    std::string filename = mEnvironment.GetPath("LightColorSampler.txt");
    mColorInterpolator = std::make_shared<CubicInterpolator<3, float>>(filename);

    // Initially populate the vertex buffer.
    UpdateMorph(0.0f);
}

void MorphFacesWindow::CreateMorphResult()
{
    mMorphResult = std::make_shared<Node>();
    mScene->AttachChild(mMorphResult);

    std::array<std::shared_ptr<Material>, 4> materials;
    for (int i = 0; i < 4; ++i)
    {
        if (i != 1)
        {
            materials[i] = std::make_shared<Material>();
        }
    }

    materials[0]->emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
    materials[0]->ambient = { 0.694118f, 0.607843f, 0.545098f, 1.0f };
    materials[0]->diffuse = { 0.694118f, 0.607843f, 0.545098f, 1.0f };
    materials[0]->specular = { 0.9f, 0.9f, 0.9f, 10.0f };

    materials[2]->emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
    materials[2]->ambient = { 0.388235f, 0.282353f, 0.168627f, 1.0f };
    materials[2]->diffuse = { 0.388235f, 0.282353f, 0.168627f, 1.0f };
    materials[2]->specular = { 0.9f, 0.9f, 0.9f, 10.0f };

    materials[3]->emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
    materials[3]->ambient = { 0.364706f, 0.0f, 0.0f, 1.0f };
    materials[3]->diffuse = { 0.364706f, 0.0f, 0.0f, 1.0f };
    materials[3]->specular = { 0.9f, 0.9f, 0.9f, 10.0f };

    mLighting = std::make_shared<Lighting>();
    mLighting->ambient = { 1.0f, 0.945098f, 0.792157f, 1.0f };
    mLighting->diffuse = { 1.0f, 0.945098f, 0.792157f, 1.0f };
    mLighting->specular = { 1.0f, 0.945098f, 0.792157f, 1.0f };
    mLighting->attenuation = { 1.0f, 0.0f, 0.0f, 0.5f };

    mLightGeometry = std::make_shared<LightCameraGeometry>();
    mLightWorldPosition = { -1186.77f, -1843.32f, -50.7567f, 1.0f };

    for (int i = 0; i < 4; ++i)
    {
        if (i != 1)
        {
            mPLEffects[i] = std::make_shared<PointLightEffect>(
                mProgramFactory, mUpdater, 1, materials[i], mLighting, mLightGeometry);
        }
    }

    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_NORMAL, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);

    std::ifstream input(mEnvironment.GetPath("SharedTexTri.txt"));
    input >> mNumVertices;
    std::shared_ptr<VertexBuffer> vbuffer = std::make_shared<VertexBuffer>(
        vformat, mNumVertices);
    vbuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    OutVertex* vertices = vbuffer->Get<OutVertex>();
    for (int i = 0; i < mNumVertices; ++i)
    {
        vertices[i].position = { 0.0f, 0.0f, 0.0f };
        vertices[i].normal = { 0.0f, 0.0f, 0.0f };
        input >> vertices[i].tcoord[0];
        input >> vertices[i].tcoord[1];
    }

    for (int j = 0; j < 4; ++j)
    {
        int numSubTriangles;
        input >> numSubTriangles;
        int numSubIndices = 3 * numSubTriangles;
        std::shared_ptr<IndexBuffer> ibuffer = std::make_shared<IndexBuffer>(
            IP_TRIMESH, numSubTriangles, sizeof(unsigned int));
        unsigned int* subIndices = ibuffer->Get<unsigned int>();
        for (int i = 0; i < numSubIndices; ++i)
        {
            int index;
            input >> index;
            *subIndices++ = index;
        }

        std::shared_ptr<Visual> visual = std::make_shared<Visual>(vbuffer, ibuffer);
        mMorphResult->AttachChild(visual);
        if (j != 1)
        {
            visual->SetEffect(mPLEffects[j]);
            mPVWMatrices.Subscribe(visual->worldTransform, mPLEffects[j]->GetPVWMatrixConstant());
        }
        else
        {
            std::string path = mEnvironment.GetPath("Eye.png");
            std::shared_ptr<Texture2> texture = WICFileIO::Load(path, true);
            texture->AutogenerateMipmaps();

#if defined(GTE_DEV_OPENGL)
            // The locations of the vertex shader inputs are hard-coded in
            // the GLSL files.
            auto program = mProgramFactory->CreateFromFiles(
                mEnvironment.GetPath("Texture2PNTVS.glsl"),
                mEnvironment.GetPath("Texture2PNTPS.glsl"),
                "");
            auto pvwMatrixConstant = std::make_shared<ConstantBuffer>(sizeof(Matrix4x4<float>), true);
            auto sampler = std::make_shared<SamplerState>();
            sampler->filter = SamplerState::MIN_L_MAG_L_MIP_L;
            sampler->mode[0] = SamplerState::WRAP;
            sampler->mode[1] = SamplerState::WRAP;
            auto vshader = program->GetVShader();
            auto pshader = program->GetPShader();
            vshader->Set("PVWMatrix", pvwMatrixConstant);
            pshader->Set("baseSampler", texture);
            pshader->Set("baseSampler", sampler);
            auto txEffect = std::make_shared<VisualEffect>(program);
            visual->SetEffect(txEffect);
            mPVWMatrices.Subscribe(visual->worldTransform, pvwMatrixConstant);
#else
            // DX11/HLSL can connect the vertex buffer channels to VSMain inputs
            // using semantics.
            std::shared_ptr<Texture2Effect> txEffect = std::make_shared<Texture2Effect>(
                mProgramFactory, texture, SamplerState::MIN_L_MAG_L_MIP_L, SamplerState::WRAP,
                SamplerState::WRAP);
            visual->SetEffect(txEffect);
            mPVWMatrices.Subscribe(visual->worldTransform, txEffect->GetPVWMatrixConstant());
#endif
        }
        mVisuals.push_back(visual);
    }
    input.close();
}

void MorphFacesWindow::LoadTarget(int i, std::string const& targetName)
{
    std::string filename = mEnvironment.GetPath(targetName + "PosNor.txt");
    std::ifstream inFile(filename);
    mVertices[i].resize(mNumVertices);
    for (auto& vertex : mVertices[i])
    {
        inFile >> vertex.position[0];
        inFile >> vertex.position[1];
        inFile >> vertex.position[2];
        inFile >> vertex.normal[0];
        inFile >> vertex.normal[1];
        inFile >> vertex.normal[2];
    }
    inFile.close();

    if (i > 0)
    {
        filename = mEnvironment.GetPath(targetName + "Weights.txt");
        mWeightInterpolator[i] = std::make_shared<CubicInterpolator<1, float>>(filename);
    }
    // else: The base target's weights are 1 minus the sum of the weights of
    // the other targets.  There is no need for mWeightInterpolator[0].
}

void MorphFacesWindow::UpdateMorph(float time)
{
    // Get a pointer to the output vertex buffer.  This buffer is shared by
    // all children, so it suffices to get the pointer from child 0.
    auto visual = std::static_pointer_cast<Visual>(mMorphResult->GetChild(0));
    OutVertex* output = visual->GetVertexBuffer()->Get<OutVertex>();

    // Sample the weights at the specified time.  Ensure that the sum of the
    // weights is 1.
    std::array<float, NUM_TARGETS> weights;
    weights[0] = 1.0f;
    for (int i = 1; i < NUM_TARGETS; ++i)
    {
        std::array<float, 1> interp = (*mWeightInterpolator[i])(time);
        weights[i] = interp[0];
        weights[0] -= interp[0];
    }

    // Compute the weighted sums.
    InVertex const* inVertex = mVertices[0].data();
    OutVertex* outVertex = output;
    float weight = weights[0];
    for (int j = 0; j < mNumVertices; ++j, ++inVertex, ++outVertex)
    {
        outVertex->position = weight * inVertex->position;
        outVertex->normal = weight * inVertex->normal;
    }
    for (int i = 1; i < NUM_TARGETS; ++i)
    {
        inVertex = mVertices[i].data();
        outVertex = output;
        weight = weights[i];
        if (weight > 0.0f)
        {
            for (int j = 0; j < mNumVertices; ++j, ++inVertex, ++outVertex)
            {
                outVertex->position += weight * inVertex->position;
                outVertex->normal += weight * inVertex->normal;
            }
        }
    }

    // Normalize the normals.
    outVertex = output;
    for (int j = 0; j < mNumVertices; ++j, ++outVertex)
    {
        Normalize(outVertex->normal);
    }

    // Update the VRAM copy.
    mEngine->Update(visual->GetVertexBuffer());

    // Update the bounding spheres.
    for (int j = 0; j < 4; ++j)
    {
        visual = std::static_pointer_cast<Visual>(mMorphResult->GetChild(j));
        visual->UpdateModelBound();
    }
    mScene->Update();

    // Update the light colors.
    std::array<float, 3> color = (*mColorInterpolator)(time);
    mLighting->ambient = { color[0], color[1], color[2], 1.0f };
    mLighting->diffuse = mLighting->ambient;
    mLighting->specular = mLighting->ambient;
    Matrix4x4<float> hinverse = mScene->worldTransform.GetHInverse();
#if defined(GTE_USE_MAT_VEC)
    mLightGeometry->lightModelPosition = hinverse * mLightWorldPosition;
    mLightGeometry->cameraModelPosition = hinverse * mCamera->GetPosition();
#else
    mLightGeometry->lightModelPosition = mLightWorldPosition * hinverse;
    mLightGeometry->cameraModelPosition = mCamera->GetPosition() * hinverse;
#endif
    for (int i = 0; i < 4; ++i)
    {
        if (i != 1)
        {
            mPLEffects[i]->UpdateLightingConstant();
            mPLEffects[i]->UpdateGeometryConstant();
        }
    }
}
