// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/08/31)

#include "GTEnginePCH.h"
#include "GteLogToOutputWindow.h"
#include "GteEnvironment.h"
#include <Windows.h>
using namespace gte;

//----------------------------------------------------------------------------
LogToOutputWindow::LogToOutputWindow(int flags)
    :
    Logger::Listener(flags)
{
}
//----------------------------------------------------------------------------
void LogToOutputWindow::Report(std::string const& message)
{
    std::wstring text = Environment::Convert(message);
    OutputDebugString(text.c_str());
}
//----------------------------------------------------------------------------
