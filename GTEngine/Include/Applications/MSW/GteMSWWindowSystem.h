// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.3 (2018/03/11)

#pragma once

#include <Applications/MSW/GteMSWWindow.h>
#include <map>
#include <memory>

namespace gte
{

class GTE_IMPEXP WindowSystem
{
public:
    // This class has code that is common to DX11-based and WGL-based window
    // applications, except for the function CreateEngineAndProgramFactory
    // that has specialized implementations for each graphics system (in
    // the Applications/MSW/{DX11,WGL} subfolders).
    WindowSystem();
    ~WindowSystem();

public:
    // Create and destroy windows.  Derived classes may extend the inputs
    // using a nested class derived from Window::Parameters
    template <typename WindowType>
    std::shared_ptr<WindowType> Create(typename WindowType::Parameters& parameters);

    template <typename WindowType>
    void Destroy(std::shared_ptr<WindowType>& window);

    enum
    {
        DEFAULT_ACTION = 0,
        NO_IDLE_LOOP = 1
    };

    template <typename WindowType>
    void MessagePump(std::shared_ptr<WindowType> const& window, unsigned int flags);

protected:
    // Get the true window size for the specified client size.  The true size
    // includes extra space for window decorations (window border, menu bar,
    // and so on).  This information is useful to know before creating a
    // window to ensure the to-be-created window fits within the monitor
    // resolution.
    static bool GetWindowRectangle(int xClientSize, int yClientSize,
        DWORD style, RECT& windowRectangle);

    // Window creation and destruction.  The CreateEngineAndProgramFactory
    // function has an implementation for DX11 and an implementation for WGL.
    // It is not possible to have both DX11-based and WGL-based window
    // creation in the same application, although it is possible to have
    // DX11-based and WGL-based graphics engines in the same application.
    void CreateFrom(MSWWindow::Parameters& parameters);
    void CreateEngineAndProgramFactory(MSWWindow::Parameters& parameters);

    // Extraction of cursor location, avoiding the extraction in <windows.h>
    // that does not work when you have dual monitors.
    static void Extract(LPARAM lParam, int& x, int& y);
    static void Extract(WPARAM wParam, int& x, int& y);

    // The event handler.
    static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

    wchar_t const* mWindowClassName;
    ATOM mAtom;
    std::map<HWND, std::shared_ptr<MSWWindow>> mHandleMap;
};


template <typename WindowType>
std::shared_ptr<WindowType> WindowSystem::Create(typename WindowType::Parameters& parameters)
{
    CreateFrom(parameters);
    if (parameters.created)
    {
        std::shared_ptr<WindowType> window = std::make_shared<WindowType>(parameters);
        mHandleMap[parameters.handle] = window;
        if (parameters.created)
        {
            return window;
        }
        Destroy(window);
    }
    // else: CreateFrom will report the problem via the logger system.
    return nullptr;
}

template <typename WindowType>
void WindowSystem::Destroy(std::shared_ptr<WindowType>& window)
{
    if (window)
    {
        HWND handle = window->GetHandle();
        mHandleMap.erase(handle);
        window = nullptr;
        DestroyWindow(handle);
    }
}

template <typename WindowType>
void WindowSystem::MessagePump(std::shared_ptr<WindowType> const& window, unsigned int flags)
{
    if (window)
    {
        HWND handle = window->GetHandle();
        ShowWindow(handle, SW_SHOW);
        UpdateWindow(handle);

        for (;;)
        {
            if (flags & NO_IDLE_LOOP)
            {
                WaitMessage();
            }

            MSG msg;
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                if (!(flags & NO_IDLE_LOOP))
                {
                    if (!window->IsMinimized())
                    {
                        window->OnIdle();
                    }
                }
            }
        }
    }
}

extern GTE_IMPEXP WindowSystem TheWindowSystem;

}
