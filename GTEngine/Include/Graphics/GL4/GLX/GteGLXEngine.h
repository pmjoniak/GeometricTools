// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.2 (2017/06/29)

#pragma once

#include <Graphics/GL4/GteGL4Engine.h>

// Forward declarations to avoid name conflicts caused by #include-ing
// X11/Xlib.h and GL/glx.h.
struct _XDisplay;
struct __GLXcontextRec;

namespace gte
{

class GTE_IMPEXP GLXEngine : public GL4Engine
{
public:
    // Construction and destruction.  The first constructor is for windowed
    // graphics applications.  The second constructor is for windowless
    // compute-program applications.
    //
    // TODO: Currently, OpenGL 4.3 is required for compute shaders.  See
    // the comment in GteGL4Engine.h for 'MeetsRequirements()'.
    virtual ~GLXEngine();
    GLXEngine(_XDisplay* display, unsigned long window, __GLXcontextRec* context,
        int xSize, int ySize, bool useDepth24Stencil8, bool saveDriverInfo, int requiredMajor = 4, int requiredMinor = 3);
    GLXEngine(bool useDepth24Stencil8, bool saveDriverInfo, int requiredMajor = 4, int requiredMinor = 3);

    // Member access.
    inline _XDisplay* GetDisplay() const { return mDisplay; }
    inline unsigned long GetWindow() const { return mWindow; }
    inline __GLXcontextRec* GetImmediate() const { return mImmediate; }

    // Allow the user to switch between OpenGL contexts when there are
    // multiple instances of GL4Engine in an application.
    virtual bool IsActive() const override;
    virtual void MakeActive() override;

    // Support for clearing the color, depth, and stencil back buffers.
    virtual void DisplayColorBuffer(unsigned int syncInterval) override;

private:
    // Helpers for construction and destruction.
    virtual bool Initialize(int requiredMajor, int requiredMinor, bool useDepth24Stencil8, bool saveDriverInfo) override;
    void Terminate();

    _XDisplay* mDisplay;
    unsigned long mWindow;
    __GLXcontextRec* mImmediate;
    bool mIsComputeWindow;
};

}
