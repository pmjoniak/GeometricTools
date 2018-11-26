// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.13.0 (2018/06/07)

//#define GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION
#if defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
//#define USE_RUNTIME_TEMPLATE
#endif

#include <GTEngine.h>
using namespace gte;

Environment gEnvironment;
std::string gPath;
int gCacheMode = 0;

void DoIntpBSplineUniform1()
{
    // This example is included in
    // https://www.geometrictools.com/Documentation/BSplineInterpolation.pdf

    struct Controls
    {
        // The type of the control points.
        typedef double Type;

        int GetSize(int) const
        {
            return static_cast<int>(signal.size());
        }

        Type operator() (int const* tuple) const
        {
            return signal[tuple[0]];
        }

#if !defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
        Type operator() (int x) const
        {
            return signal[x];
        }
#endif

        std::vector<double> signal;
    };

    Controls controls;
    controls.signal.resize(6);
    controls.signal[0] = 1.0;
    controls.signal[1] = 2.0;
    controls.signal[2] = 1.5;
    controls.signal[3] = 0.25;
    controls.signal[4] = 1.25;
    controls.signal[5] = 1.25;

#if defined(USE_RUNTIME_TEMPLATE)
    IntpBSplineUniform<double, Controls> interp({ 2 }, controls, 0.0, gCacheMode);
#else
    IntpBSplineUniform<double, Controls, 1> interp({ 2 }, controls, 0.0, gCacheMode);
#endif

    // Draw the graph as a sequence of points to see that it looks similar
    // to that in the PDF.
    auto texture = std::make_shared<Texture2>(DF_R8G8B8A8_UNORM, 512, 512);
    uint32_t* texels = texture->Get<uint32_t>();
    memset(texels, 0xFF, texture->GetNumBytes());
    double fmin = -0.1, fmax = 2.1;
    double tmin = interp.GetTMin(0), tmax = interp.GetTMax(0);
    for (uint32_t x = 0; x < 512; ++x)
    {
        double t = tmin + (static_cast<double>(x) / 511.0) * (tmax - tmin);
        double f = 511.0 * (interp.Evaluate({ 0 }, { t }) - fmin) / (fmax - fmin);
        uint32_t y = 511 - static_cast<uint32_t>(std::round(f));
        texels[x + 512 * y] = 0;
    }

    // Compute the first-order derivative of the B-spline and superimpose
    // it on the graph.  The zero-derivative line is drawn so that you can
    // see where the derivative switches between positive and negative.  The
    // B-spline is piecewise quadratic, so the derivative is piecewise linear.
    fmin = std::numeric_limits<double>::max();
    fmax = -fmin;
    std::vector<double> deriv(512);
    for (uint32_t x = 0; x < 512; ++x)
    {
        double t = tmin + (static_cast<double>(x) / 511.0) * (tmax - tmin);
        deriv[x] = interp.Evaluate({ 1 }, { t });
        fmin = std::min(fmin, deriv[x]);
        fmax = std::max(fmax, deriv[x]);
    }

    for (uint32_t x = 0; x < 512; ++x)
    {
        double f = 511.0 * (deriv[x] - fmin) / (fmax - fmin);
        uint32_t y = 511 - static_cast<uint32_t>(std::round(f));
        texels[x + 512 * y] = 0xFF0000FF;
    }

    double f = 511.0 * (0.0 - fmin) / (fmax - fmin);
    uint32_t y = 511 - static_cast<uint32_t>(std::round(f));
    for (uint32_t x = 0; x < 512; ++x)
    {
        texels[x + 512 * y] = 0xFF00FF00;
    }

    std::string outPath = gPath + "/Samples/Imagics/BSplineInterpolation/Curve";
    std::string suffix = "_mode" + std::to_string(gCacheMode) + ".png";
#if defined(USE_RUNTIME_TEMPLATE)
    std::string filename = outPath + "RT" + suffix;
#else
#if defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
    std::string filename = outPath + "CT" + suffix;
#else
    std::string filename = outPath + "SP" + suffix;
#endif
#endif
    WICFileIO::SaveToPNG(filename, texture);
}

void DoIntpBSplineUniform2()
{
    // This example shows how to have control points that are not necessarily
    // scalar-valued.  The native image format is RGBA stored as 32-bit
    // unsigned integers.  The B-spline interpolation needs to know how to
    // add RGBA colors and multiply RGBA colors by a scalar.  The ColorType
    // wrapper provides the minimial interface to do this, using 4-tuple
    // float-valued colors for the arithmetic.
    struct ColorType
    {
        // Create the color (0,0,0,0).
        ColorType()
            :
            color({ 0.0f, 0.0f, 0.0f, 0.0f })
        {
        }

        ColorType(Vector<4, float> const& inColor)
            :
            color(inColor)
        {
        }

        ColorType operator+(ColorType const& other)
        {
            return color + other.color;
        }

        ColorType operator*(float scalar)
        {
            return color * scalar;
        }

        // The color is (r,g,b,a) with channel values in [0,255].
        Vector<4, float> color;
    };

    // The Controls structure wraps the 2D image container, in this case
    // as a texture object.  It provides the operator()(x,y) accessor to
    // convert a 32-bit unsigned int color to a 4-tuple float color.
    struct Controls
    {
        typedef ColorType Type;

        int GetSize(int i) const
        {
            return size[i];
        }

        Type operator() (int const* tuple) const
        {
            uint32_t const* texels = image->Get<uint32_t>();
            int index = tuple[0] + size[0] * tuple[1];
            uint32_t texel = texels[index];
            Vector<4, float> color;
            color[0] = (float)(texel & 0x000000FF);
            color[1] = (float)((texel & 0x0000FF00) >> 8);
            color[2] = (float)((texel & 0x00FF0000) >> 16);
            color[3] = (float)((texel & 0xFF000000) >> 24);
            return color;
        }

#if !defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
        Type operator() (int x, int y) const
        {
            uint32_t const* texels = image->Get<uint32_t>();
            uint32_t texel = texels[x + size[0] * y];
            Vector<4, float> color;
            color[0] = (float)(texel & 0x000000FF);
            color[1] = (float)((texel & 0x0000FF00) >> 8);
            color[2] = (float)((texel & 0x00FF0000) >> 16);
            color[3] = (float)((texel & 0xFF000000) >> 24);
            return color;
        }
#endif

        std::array<int, 2> size;
        std::shared_ptr<Texture2> image;
    };

    std::string infile = gEnvironment.GetPath("Magician.png");
    if (infile == "")
    {
        LogError("Cannot find Magician.png");
        return;
    }
    Controls controls;
    controls.image = WICFileIO::Load(infile, false);
    controls.size[0] = controls.image->GetWidth();
    controls.size[1] = controls.image->GetHeight();

#if defined(USE_RUNTIME_TEMPLATE)
    IntpBSplineUniform<float, Controls> interp({ 3, 3 }, controls, ColorType(), gCacheMode);
#else
    IntpBSplineUniform<float, Controls, 2> interp({ 3, 3 }, controls, ColorType(), gCacheMode);
#endif

    auto texture = std::make_shared<Texture2>(DF_R8G8B8A8_UNORM, controls.size[0], controls.size[1]);
    uint32_t* texels = texture->Get<uint32_t>();
    memset(texels, 0xFF, texture->GetNumBytes());
    for (int y = 0; y < controls.size[1]; ++y)
    {
        float t1 = static_cast<float>(y);
        for (int x = 0; x < controls.size[0]; ++x)
        {
            float t0 = static_cast<float>(x);
            ColorType result = interp.Evaluate({ 0, 0 }, { t0, t1 });
            uint32_t r = static_cast<uint32_t>(result.color[0]);
            uint32_t g = static_cast<uint32_t>(result.color[1]);
            uint32_t b = static_cast<uint32_t>(result.color[2]);
            uint32_t a = static_cast<uint32_t>(result.color[3]);
            uint32_t& color = texels[x + controls.size[0] * y];
            color = r | (g << 8) | (b << 16) | (a << 24);
        }
    }

    std::string outPath = gPath + "/Samples/Imagics/BSplineInterpolation/Surface";
    std::string suffix = "_mode" + std::to_string(gCacheMode) + ".png";
#if defined(USE_RUNTIME_TEMPLATE)
    std::string filename = outPath + "RT" + suffix;
#else
#if defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
    std::string filename = outPath + "CT" + suffix;
#else
    std::string filename = outPath + "SP" + suffix;
#endif
#endif
    WICFileIO::SaveToPNG(filename, texture);
}

void DoIntpBSplineUniform3()
{
    // In this example, the 3D image is stored in a 1D array and the Controls
    // structure wraps this representation so that the B-spline interpolator
    // can perform its arithmetic.
    struct Controls
    {
        typedef float Type;

        int GetSize(int i) const
        {
            return size[i];
        }

        Type operator() (int const* tuple) const
        {
            int index = tuple[0] + size[0] * (tuple[1] + size[1] * tuple[2]);
            uint8_t value = image[index];
            return static_cast<Type>(value);
        }

#if !defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
        Type operator() (int x, int y, int z) const
        {
            uint8_t value = image[x + size[0] * (y + size[1] * z)];
            return static_cast<Type>(value);
        }
#endif

        std::array<int, 3> size;
        std::vector<uint8_t> image;
    };

    // To save disk space, the 97x97x116 3D image for an x-ray crystallography
    // of a molecule is stored as a 16x8 array of slices, each slice 97x97.
    // The last row of the 16 row of slices contains only 4 tiles, the rest
    // set to zero.
    std::string infile = gEnvironment.GetPath("Molecule97x97x116.png");
    if (infile == "")
    {
        LogError("Cannot find file Molecule97x97x116.png");
        return;
    }

    // Let's store the image in a 1-dimensional array just to show that the
    // Controls structure does not necessarily have to wrap a Texture2 as done
    // in DoIntpBSplineUniform2().  This requires iterating over the tiles and
    // repackaging into a single row-major-order image; that is, a simple copy
    // from the texels to the 1-dimensional image array is not correct.
    auto texture = WICFileIO::Load(infile, false);
    auto texels = texture->Get<uint8_t>();
    Controls controls;
    controls.size[0] = 97;
    controls.size[1] = 97;
    controls.size[2] = 116;
    controls.image.resize(controls.size[0] * controls.size[1] * controls.size[2]);
    for (int yTile = 0, z = 0; yTile < 8; ++yTile)
    {
        int yMin = yTile * controls.size[1];
        for (int xTile = 0; xTile < 16; ++xTile)
        {
            int xMin = xTile * controls.size[0];
            for (int y = 0; y < controls.size[1]; ++y)
            {
                for (int x = 0; x < controls.size[0]; ++x)
                {
                    int src = (xMin + x) + texture->GetWidth() * (yMin + y);
                    int trg = x + controls.size[0] * (y + controls.size[1] * z);
                    controls.image[trg] = texels[src];
                }
            }

            if (++z == controls.size[2])
            {
                break;
            }
        }

        if (z == controls.size[2])
        {
            break;
        }
    }

#if defined(USE_RUNTIME_TEMPLATE)
    IntpBSplineUniform<float, Controls> interp({ 3, 3, 2 }, controls, 0.0f, gCacheMode);
#else
    IntpBSplineUniform<float, Controls, 3> interp({ 3, 3, 2 }, controls, 0.0f, gCacheMode);
#endif

    std::vector<uint8_t> output(controls.image.size());
    for (int z = 0, i = 0; z < controls.size[2]; ++z)
    {
        float t2 = static_cast<float>(z);
        for (int y = 0; y < controls.size[1]; ++y)
        {
            float t1 = -0.5f + static_cast<float>(y);
            for (int x = 0; x < controls.size[0]; ++x, ++i)
            {
                float t0 = -0.5f + static_cast<float>(x);
                output[i] = static_cast<uint8_t>(interp.Evaluate({ 0, 0, 0 }, { t0, t1, t2 }));
            }
        }
    }

    // Write the output 3D image as an array of 2D slices.
    memset(texels, 0, texture->GetNumBytes());
    for (int yTile = 0, z = 0; yTile < 8; ++yTile)
    {
        int yMin = yTile * controls.size[1];
        for (int xTile = 0; xTile < 16; ++xTile)
        {
            int xMin = xTile * controls.size[0];
            for (int y = 0; y < controls.size[1]; ++y)
            {
                for (int x = 0; x < controls.size[0]; ++x)
                {
                    int src = x + controls.size[0] * (y + controls.size[1] * z);
                    int trg = (xMin + x) + texture->GetWidth() * (yMin + y);
                    texels[trg] = output[src];
                }
            }

            if (++z == controls.size[2])
            {
                break;
            }
        }

        if (z == controls.size[2])
        {
            break;
        }
    }

    std::string outPath = gPath + "/Samples/Imagics/BSplineInterpolation/Volume";
    std::string suffix = "_mode" + std::to_string(gCacheMode) + ".png";
#if defined(USE_RUNTIME_TEMPLATE)
    std::string filename = outPath + "RT" + suffix;
#else
#if defined(GTE_INTP_BSPLINE_UNIFORM_NO_SPECIALIZATION)
    std::string filename = outPath + "CT" + suffix;
#else
    std::string filename = outPath + "SP" + suffix;
#endif
#endif
    WICFileIO::SaveToPNG(filename, texture);
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

    gPath = gEnvironment.GetVariable("GTE_PATH");
    if (gPath == "")
    {
        LogError("You must set the GTE_PATH environment variable.");
        return -1;
    }
    gEnvironment.Insert(gPath + "/Samples/Data/");

    DoIntpBSplineUniform1();
    DoIntpBSplineUniform2();
    DoIntpBSplineUniform3();
    return 0;
}
