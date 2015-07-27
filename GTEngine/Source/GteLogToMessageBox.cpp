// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/08/31)

#include "GTEnginePCH.h"
#include "GteLogToMessageBox.h"
#include "GteEnvironment.h"
#include <Windows.h>
using namespace gte;

//----------------------------------------------------------------------------
LogToMessageBox::LogToMessageBox(int flags)
    :
    Logger::Listener(flags)
{
}
//----------------------------------------------------------------------------
void LogToMessageBox::Report(std::string const& message)
{
    std::string output = message + "Do you want to debug?";

    std::wstring text = Environment::Convert(output);
    int selection = MessageBox(nullptr, text.c_str(), L"Report",
        MB_ICONERROR | MB_YESNOCANCEL | MB_APPLMODAL | MB_TOPMOST);

    switch (selection)
    {
    case IDYES:
        // Break and debug.
        __debugbreak();
        break;

    case IDNO:
        // Continue execution.
        break;

    case IDCANCEL:
    default:
        // Terminate execution.
        exit(0);
        break;
    }
}
//----------------------------------------------------------------------------
