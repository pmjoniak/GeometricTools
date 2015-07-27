// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/18)

#include "ProjectTemplate.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <regex>
#include <Rpc.h>

//----------------------------------------------------------------------------
TemplateVC120::~TemplateVC120()
{
}
//----------------------------------------------------------------------------
TemplateVC120::TemplateVC120(std::string const& name,
    std::string const& gtPath, bool& success)
    :
    mName(name),
    mGTPath(gtPath),
    mProjectGUID(GetGuidString()),
    mRequiredGUID(GetGuidString())
{
    std::string solnName = mName + ".sln";
    std::string projName = mName + ".vcxproj";
    std::string filtName = mName + ".vcxproj.filters";
    std::string appcName = mName + ".cpp";
    std::string winhName = mName + "Window.h";
    std::string wincName = mName + "Window.cpp";

    success =
        Create(solnName, NUM_SOLUTION_LINES, msSolutionLines) &&
        Create(projName, NUM_PROJECT_LINES, msProjectLines) &&
        Create(filtName, NUM_FILTER_LINES, msFilterLines) &&
        Create(appcName, NUM_APP_LINES, msAppLines) &&
        Create(winhName, NUM_WINH_LINES, msWinHLines) &&
        Create(wincName, NUM_WINC_LINES, msWinCLines);
}
//----------------------------------------------------------------------------
bool TemplateVC120::Create(std::string const& name, int numLines,
    char const* lines[])
{
    std::ofstream outFile(name.c_str());
    if (outFile)
    {
        for (int i = 0; i < numLines; ++i)
        {
            std::string line(lines[i]);
            line = std::regex_replace(line, mGPPattern, mGTPath);
            line = std::regex_replace(line, mPNPattern, mName);
            line = std::regex_replace(line, mPGPattern, mProjectGUID);
            line = std::regex_replace(line, mRQPattern, mRequiredGUID);
            line = std::regex_replace(line, mGTPattern, msGTGUID);
            outFile << line.c_str() << std::endl;
        }

        outFile.close();
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
std::string TemplateVC120::GetGuidString()
{
    UUID guid;
    RPC_STATUS status = UuidCreate(&guid);
    if (RPC_S_OK != status)
    {
        return "";
    }

    RPC_CSTR stringUuid = nullptr;
    status = UuidToString(&guid, &stringUuid);
    if (RPC_S_OK != status || nullptr == stringUuid)
    {
        return "";
    }

    std::string stringGuid(reinterpret_cast<char*>(stringUuid));
    RpcStringFree(&stringUuid);
    return stringGuid;
}
//----------------------------------------------------------------------------


std::regex const TemplateVC120::mGPPattern("GTPATH");
std::regex const TemplateVC120::mPNPattern("PROJECTNAME");
std::regex const TemplateVC120::mPGPattern("PROJECTGUID");
std::regex const TemplateVC120::mRQPattern("REQUIREDGUID");
std::regex const TemplateVC120::mGTPattern("GTGUID");
std::string const TemplateVC120::msGTGUID("702CEC70-EE3E-43A0-8D93-48B58FA50D3C");

char const* TemplateVC120::msSolutionLines[NUM_SOLUTION_LINES] =
{
"﻿Microsoft Visual Studio Solution File, Format Version 12.00",
"# Visual Studio 2013",
"VisualStudioVersion = 12.0.30110.1",
"MinimumVisualStudioVersion = 10.0.40219.1",
"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"PROJECTNAME\", \"PROJECTNAME.vcxproj\", \"{PROJECTGUID}\"",
"EndProject",
"Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Required\", \"Required\", \"{REQUIREDGUID}\"",
"EndProject",
"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"GTEngine\", \"GTPATHGTEngine.vcxproj\", \"{GTGUID}\"",
"EndProject",
"Global",
"	GlobalSection(SolutionConfigurationPlatforms) = preSolution",
"		Debug|Win32 = Debug|Win32",
"		Debug|x64 = Debug|x64",
"		Release|Win32 = Release|Win32",
"		Release|x64 = Release|x64",
"	EndGlobalSection",
"	GlobalSection(ProjectConfigurationPlatforms) = postSolution",
"		{PROJECTGUID}.Debug|Win32.ActiveCfg = Debug|Win32",
"		{PROJECTGUID}.Debug|Win32.Build.0 = Debug|Win32",
"		{PROJECTGUID}.Debug|x64.ActiveCfg = Debug|x64",
"		{PROJECTGUID}.Debug|x64.Build.0 = Debug|x64",
"		{PROJECTGUID}.Release|Win32.ActiveCfg = Release|Win32",
"		{PROJECTGUID}.Release|Win32.Build.0 = Release|Win32",
"		{PROJECTGUID}.Release|x64.ActiveCfg = Release|x64",
"		{PROJECTGUID}.Release|x64.Build.0 = Release|x64",
"		{GTGUID}.Debug|Win32.ActiveCfg = Debug|Win32",
"		{GTGUID}.Debug|Win32.Build.0 = Debug|Win32",
"		{GTGUID}.Debug|x64.ActiveCfg = Debug|x64",
"		{GTGUID}.Debug|x64.Build.0 = Debug|x64",
"		{GTGUID}.Release|Win32.ActiveCfg = Release|Win32",
"		{GTGUID}.Release|Win32.Build.0 = Release|Win32",
"		{GTGUID}.Release|x64.ActiveCfg = Release|x64",
"		{GTGUID}.Release|x64.Build.0 = Release|x64",
"	EndGlobalSection",
"	GlobalSection(SolutionProperties) = preSolution",
"		HideSolutionNode = FALSE",
"	EndGlobalSection",
"	GlobalSection(NestedProjects) = preSolution",
"		{GTGUID} = {REQUIREDGUID}",
"	EndGlobalSection",
"EndGlobal"
};

char const* TemplateVC120::msProjectLines[NUM_PROJECT_LINES] =
{
    "﻿<?xml version=\"1.0\" encoding=\"utf-8\"?>",
    "<Project DefaultTargets=\"Build\" ToolsVersion=\"12.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">",
    "  <ItemGroup Label=\"ProjectConfigurations\">",
    "    <ProjectConfiguration Include=\"Debug|Win32\">",
    "      <Configuration>Debug</Configuration>",
"      <Platform>Win32</Platform>",
"    </ProjectConfiguration>",
"    <ProjectConfiguration Include=\"Debug|x64\">",
"      <Configuration>Debug</Configuration>",
"      <Platform>x64</Platform>",
"    </ProjectConfiguration>",
"    <ProjectConfiguration Include=\"Release|Win32\">",
"      <Configuration>Release</Configuration>",
"      <Platform>Win32</Platform>",
"    </ProjectConfiguration>",
"    <ProjectConfiguration Include=\"Release|x64\">",
"      <Configuration>Release</Configuration>",
"      <Platform>x64</Platform>",
"    </ProjectConfiguration>",
"  </ItemGroup>",
"  <PropertyGroup Label=\"Globals\">",
"    <ProjectGuid>{PROJECTGUID}</ProjectGuid>",
"    <Keyword>Win32Proj</Keyword>",
"    <RootNamespace>PROJECTNAME</RootNamespace>",
"  </PropertyGroup>",
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">",
"    <ConfigurationType>Application</ConfigurationType>",
"    <UseDebugLibraries>true</UseDebugLibraries>",
"    <PlatformToolset>v120</PlatformToolset>",
"    <CharacterSet>Unicode</CharacterSet>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">",
"    <ConfigurationType>Application</ConfigurationType>",
"    <UseDebugLibraries>true</UseDebugLibraries>",
"    <PlatformToolset>v120</PlatformToolset>",
"    <CharacterSet>Unicode</CharacterSet>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">",
"    <ConfigurationType>Application</ConfigurationType>",
"    <UseDebugLibraries>false</UseDebugLibraries>",
"    <PlatformToolset>v120</PlatformToolset>",
"    <WholeProgramOptimization>true</WholeProgramOptimization>",
"    <CharacterSet>Unicode</CharacterSet>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">",
"    <ConfigurationType>Application</ConfigurationType>",
"    <UseDebugLibraries>false</UseDebugLibraries>",
"    <PlatformToolset>v120</PlatformToolset>",
"    <WholeProgramOptimization>true</WholeProgramOptimization>",
"    <CharacterSet>Unicode</CharacterSet>",
"  </PropertyGroup>",
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />",
"  <ImportGroup Label=\"ExtensionSettings\">",
"  </ImportGroup>",
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"PropertySheets\">",
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />",
"  </ImportGroup>",
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"PropertySheets\">",
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />",
"  </ImportGroup>",
"  <PropertyGroup Label=\"UserMacros\" />",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">",
"    <LinkIncremental>true</LinkIncremental>",
"    <OutDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</OutDir>",
"    <IntDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</IntDir>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">",
"    <LinkIncremental>true</LinkIncremental>",
"    <OutDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</OutDir>",
"    <IntDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</IntDir>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">",
"    <LinkIncremental>false</LinkIncremental>",
"    <OutDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</OutDir>",
"    <IntDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</IntDir>",
"  </PropertyGroup>",
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">",
"    <LinkIncremental>false</LinkIncremental>",
"    <OutDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</OutDir>",
"    <IntDir>_Output\\$(PlatformToolset)\\$(Platform)\\$(Configuration)\\</IntDir>",
"  </PropertyGroup>",
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">",
"    <ClCompile>",
"      <PrecompiledHeader>",
"      </PrecompiledHeader>",
"      <WarningLevel>Level4</WarningLevel>",
"      <Optimization>Disabled</Optimization>",
"      <PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>",
"      <AdditionalIncludeDirectories>GTPATHInclude</AdditionalIncludeDirectories>",
"      <TreatWarningAsError>true</TreatWarningAsError>",
"    </ClCompile>",
"    <Link>",
"      <SubSystem>Console</SubSystem>",
"      <GenerateDebugInformation>true</GenerateDebugInformation>",
"      <AdditionalDependencies>d3d11.lib;d3dcompiler.lib;dxgi.lib;dxguid.lib;Windowscodecs.lib;%(AdditionalDependencies)</AdditionalDependencies>",
"    </Link>",
"  </ItemDefinitionGroup>",
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">",
"    <ClCompile>",
"      <PrecompiledHeader>",
"      </PrecompiledHeader>",
"      <WarningLevel>Level4</WarningLevel>",
"      <Optimization>Disabled</Optimization>",
"      <PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>",
"      <AdditionalIncludeDirectories>GTPATHInclude</AdditionalIncludeDirectories>",
"      <TreatWarningAsError>true</TreatWarningAsError>",
"    </ClCompile>",
"    <Link>",
"      <SubSystem>Console</SubSystem>",
"      <GenerateDebugInformation>true</GenerateDebugInformation>",
"      <AdditionalDependencies>d3d11.lib;d3dcompiler.lib;dxgi.lib;dxguid.lib;Windowscodecs.lib;%(AdditionalDependencies)</AdditionalDependencies>",
"    </Link>",
"  </ItemDefinitionGroup>",
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">",
"    <ClCompile>",
"      <WarningLevel>Level4</WarningLevel>",
"      <PrecompiledHeader>",
"      </PrecompiledHeader>",
"      <Optimization>MaxSpeed</Optimization>",
"      <FunctionLevelLinking>true</FunctionLevelLinking>",
"      <IntrinsicFunctions>true</IntrinsicFunctions>",
"      <PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>",
"      <AdditionalIncludeDirectories>GTPATHInclude</AdditionalIncludeDirectories>",
"      <TreatWarningAsError>true</TreatWarningAsError>",
"    </ClCompile>",
"    <Link>",
"      <SubSystem>Console</SubSystem>",
"      <GenerateDebugInformation>true</GenerateDebugInformation>",
"      <EnableCOMDATFolding>true</EnableCOMDATFolding>",
"      <OptimizeReferences>true</OptimizeReferences>",
"      <AdditionalDependencies>d3d11.lib;d3dcompiler.lib;dxgi.lib;dxguid.lib;Windowscodecs.lib;%(AdditionalDependencies)</AdditionalDependencies>",
"    </Link>",
"  </ItemDefinitionGroup>",
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">",
"    <ClCompile>",
"      <WarningLevel>Level4</WarningLevel>",
"      <PrecompiledHeader>",
"      </PrecompiledHeader>",
"      <Optimization>MaxSpeed</Optimization>",
"      <FunctionLevelLinking>true</FunctionLevelLinking>",
"      <IntrinsicFunctions>true</IntrinsicFunctions>",
"      <PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>",
"      <AdditionalIncludeDirectories>GTPATHInclude</AdditionalIncludeDirectories>",
"      <TreatWarningAsError>true</TreatWarningAsError>",
"    </ClCompile>",
"    <Link>",
"      <SubSystem>Console</SubSystem>",
"      <GenerateDebugInformation>true</GenerateDebugInformation>",
"      <EnableCOMDATFolding>true</EnableCOMDATFolding>",
"      <OptimizeReferences>true</OptimizeReferences>",
"      <AdditionalDependencies>d3d11.lib;d3dcompiler.lib;dxgi.lib;dxguid.lib;Windowscodecs.lib;%(AdditionalDependencies)</AdditionalDependencies>",
"    </Link>",
"  </ItemDefinitionGroup>",
"  <ItemGroup>",
"    <ClCompile Include=\"PROJECTNAME.cpp\" />",
"    <ClCompile Include=\"PROJECTNAMEWindow.cpp\" />",
"  </ItemGroup>",
"  <ItemGroup>",
"    <ClInclude Include=\"PROJECTNAMEWindow.h\" />",
"  </ItemGroup>",
"  <ItemGroup>",
"    <ProjectReference Include=\"GTPATHGTEngine.vcxproj\">",
"      <Project>{GTGUID}</Project>",
"    </ProjectReference>",
"  </ItemGroup>",
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />",
"  <ImportGroup Label=\"ExtensionTargets\">",
"  </ImportGroup>",
"</Project>"
};

char const* TemplateVC120::msFilterLines[NUM_FILTER_LINES] =
{
"﻿<?xml version=\"1.0\" encoding=\"utf-8\"?>",
"<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">",
"  <ItemGroup>",
"    <Filter Include=\"Source Files\">",
"      <UniqueIdentifier>{4FC737F1-C7A5-4376-A066-2A32D752A2FF}</UniqueIdentifier>",
"      <Extensions>cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx</Extensions>",
"    </Filter>",
"    <Filter Include=\"Header Files\">",
"      <UniqueIdentifier>{93995380-89BD-4b04-88EB-625FBE52EBFB}</UniqueIdentifier>",
"      <Extensions>h;hpp;hxx;hm;inl;inc;xsd</Extensions>",
"    </Filter>",
"  </ItemGroup>",
"  <ItemGroup>",
"    <ClCompile Include=\"PROJECTNAME.cpp\">",
"      <Filter>Source Files</Filter>",
"    </ClCompile>",
"    <ClCompile Include=\"PROJECTNAMEWindow.cpp\">",
"      <Filter>Source Files</Filter>",
"    </ClCompile>",
"  </ItemGroup>",
"  <ItemGroup>",
"    <ClInclude Include=\"PROJECTNAMEWindow.h\">",
"      <Filter>Header Files</Filter>",
"    </ClInclude>",
"  </ItemGroup>",
"</Project>"
};

char const* TemplateVC120::msAppLines[NUM_APP_LINES] =
{
"// Geometric Tools LLC, Redmond WA 98052",
"// Copyright (c) 1998-2014",
"// Distributed under the Boost Software License, Version 1.0.",
"// http://www.boost.org/LICENSE_1_0.txt",
"// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt",
"// File Version: 1.0.0 (yyyy/mm/dd)",
"",
"#include \"PROJECTNAMEWindow.h\"",
"",
"int main(int, char const*[])",
"{",
"    LogReporter reporter(",
"        \"LogReport.txt\",",
"        Logger::Listener::LISTEN_FOR_ALL,",
"        Logger::Listener::LISTEN_FOR_ALL,",
"        Logger::Listener::LISTEN_FOR_ALL,",
"        Logger::Listener::LISTEN_FOR_ALL);",
"",
"    Window::Parameters parameters;",
"    parameters.title = L\"PROJECTNAMEWindow\";",
"    parameters.xOrigin = 0;",
"    parameters.yOrigin = 0;",
"    parameters.xSize = 512;",
"    parameters.ySize = 512;",
"    parameters.allowResize = true;",
"    parameters.deviceCreationFlags = D3D11_CREATE_DEVICE_DEBUG;",
"",
"    PROJECTNAMEWindow* window =",
"        TheWindowSystem.Create<PROJECTNAMEWindow>(parameters);",
"",
"    if (window)",
"    {",
"        HWND handle = window->GetHandle();",
"        ShowWindow(handle, SW_SHOW);",
"        UpdateWindow(handle);",
"",
"        for (;;)",
"        {",
"            MSG msg;",
"            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))",
"            {",
"                if (msg.message == WM_QUIT)",
"                {",
"                    break;",
"                }",
"",
"                TranslateMessage(&msg);",
"                DispatchMessage(&msg);",
"            }",
"            else",
"            {",
"                if (!window->IsMinimized())",
"                {",
"                    window->OnIdle();",
"                }",
"            }",
"        }",
"",
"        TheWindowSystem.Destroy<PROJECTNAMEWindow>(window);",
"    }",
"",
"    return 0;",
"}"
};

char const* TemplateVC120::msWinHLines[NUM_WINH_LINES] =
{
"// Geometric Tools LLC, Redmond WA 98052",
"// Copyright (c) 1998-2014",
"// Distributed under the Boost Software License, Version 1.0.",
"// http://www.boost.org/LICENSE_1_0.txt",
"// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt",
"// File Version: 1.0.0 (yyyy/mm/dd)",
"",
"#pragma once",
"",
"#include <GTEngine.h>",
"using namespace gte;",
"",
"class PROJECTNAMEWindow : public Window",
"{",
"public:",
"    virtual ~PROJECTNAMEWindow();",
"    PROJECTNAMEWindow(Parameters& parameters);",
"",
"    virtual void OnIdle();",
"",
"private:",
"    Vector4<float> mTextColor;",
"};"
};

char const* TemplateVC120::msWinCLines[NUM_WINC_LINES] =
{
"// Geometric Tools LLC, Redmond WA 98052",
"// Copyright (c) 1998-2014",
"// Distributed under the Boost Software License, Version 1.0.",
"// http://www.boost.org/LICENSE_1_0.txt",
"// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt",
"// File Version: 1.0.0 (yyyy/mm/dd)",
"",
"#include \"PROJECTNAMEWindow.h\"",
"",
"//----------------------------------------------------------------------------",
"PROJECTNAMEWindow::~PROJECTNAMEWindow()",
"{",
"}",
"//----------------------------------------------------------------------------",
"PROJECTNAMEWindow::PROJECTNAMEWindow(Parameters& parameters)",
"    :",
"    Window(parameters),",
"    mTextColor({ 0.0f, 0.0f, 0.0f, 1.0f })",
"{",
"}",
"//----------------------------------------------------------------------------",
"void PROJECTNAMEWindow::OnIdle()",
"{",
"    MeasureTime();",
"",
"    mEngine->ClearBuffers();",
"    DrawFrameRate(8, mYSize-8, mTextColor);",
"    mEngine->DisplayColorBuffer(0);",
"",
"    UpdateFrameCount();",
"}",
"//----------------------------------------------------------------------------"
};
