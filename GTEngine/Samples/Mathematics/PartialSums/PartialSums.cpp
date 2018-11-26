// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.2 (2018/10/20)

#include <GTEngine.h>
#if defined(__LINUX__)
#include <Graphics/GL4/GteGLSLProgramFactory.h>
#include <Graphics/GL4/GLX/GteGLXEngine.h>
#endif
#include <iomanip>
using namespace gte;

void TestPartialSums()
{
    Environment env;
    std::string path = env.GetVariable("GTE_PATH");
    if (path == "")
    {
        LogError("You must create the environment variable GTE_PATH.");
        return;
    }
    env.Insert(path + "/Samples/Mathematics/PartialSums/Shaders/");
#if defined(GTE_DEV_OPENGL)
    path = env.GetPath("PartialSums.glsl");
    if (path == "")
    {
        LogError("Cannot find file PartialSums.glsl.");
        return;
    }

    // Create an engine for compute shaders.
#if defined(__MSWINDOWS__)
    WGLEngine engine(true, false);
#else
    GLXEngine engine(true, false);
#endif
    GLSLProgramFactory factory;
#else
    path = env.GetPath("PartialSums.hlsl");
    if (path == "")
    {
        LogError("Cannot find file PartialSums.hlsl.");
        return;
    }

    // Create an engine for compute shaders.
    DX11Engine engine(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0);
    HLSLProgramFactory factory;
#endif

    // Compute partial sums of 8 numbers.
    int const LOGN = 3;
    int const n = (1 << LOGN);

    // Use a Mersenne twister engine for random numbers.
    std::mt19937 mte;
    std::uniform_real_distribution<float> unitRandom(0.0f, 1.0f);

    // Select random numbers and store as the diagonal of an n-by-n texture.
    std::shared_ptr<Texture2> sum = std::make_shared<Texture2>(DF_R32_FLOAT, n, n);
    sum->SetUsage(Resource::SHADER_OUTPUT);
    sum->SetCopyType(Resource::COPY_STAGING_TO_CPU);
    float* data = sum->Get<float>();
    memset(data, 0, sum->GetNumBytes());
    for (int i = 0; i < n; ++i)
    {
        data[i + n*i] = unitRandom(mte);
    }

    // Create the shader for each p with 1 <= p <= log(n).
    std::shared_ptr<ComputeProgram> partialSumProgram[LOGN];
    for (int i = 0, p = 1; i < LOGN; ++i, ++p)
    {
        factory.defines.Set("NUM_X_THREADS", (1 << (LOGN - p)));
        factory.defines.Set("NUM_Y_THREADS", (1 << i));
        factory.defines.Set("TWO_P", (1 << p));
        factory.defines.Set("TWO_PM1", (1 << i));
        partialSumProgram[i] = factory.CreateFromFile(path);
        if (!partialSumProgram[i])
        {
            return;
        }
        partialSumProgram[i]->GetCShader()->Set("sum", sum);
    }

    // Execute the shaders.
    for (int i = 0; i < LOGN; ++i)
    {
        engine.Execute(partialSumProgram[i], 1, 1, 1);
    }

    // Read back the results from GPU memory.
    engine.CopyGpuToCpu(sum);

    // Write the entire 2D sum texture to a file.  The first column contains
    // the partial sums.  The other nonzero entries in the texture are the
    // intermediate values computed by the shaders.
    std::ofstream output("PartialSumsResult.txt");
    output << std::setprecision(6) << std::left << std::setfill('0');
    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            output << std::setw(8) << data[col + n*row] << ' ';
        }
        output << std::endl;
    }
    output << std::endl;
    output.close();

    for (int i = 0; i < LOGN; ++i)
    {
        partialSumProgram[i] = nullptr;
    }
    sum = nullptr;
}

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

    TestPartialSums();
    return 0;
}
