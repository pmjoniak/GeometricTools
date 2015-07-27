// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.10.0 (2015/02/19)

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Sample command line,
//     HLSLToByteCode /T vs_5_0 /E VSMain /D GTE_USE_MAT_VEC=1 YourShader.hlsl
// Output
//     VS_YourShader.h, VS_YourShader.cpp
// Usage in a file
//     #include "VS_YourShader.h"
//     void Test()
//     {
//         std::shared_ptr<VertexShader> vshader(VS_YourShader());
//     }

int main(int numArguments, char const* arguments[])
{
    if (numArguments <= 1)
    {
        std::cout << "usage: HLSLToByteCode" << std::endl;
        std::cout << "    /T <profile>" << std::endl;
        std::cout << "    /E <entry>" << std::endl;
        std::cout << "    /D <id>=<text> [multiple occurrences allowed]"
            << std::endl;
        std::cout << "    YourShader.hlsl" << std::endl;
        std::cout << " output: XS_YourShader.h, XS_YourShader.cpp "
            << "[X in {V,P,G,C}]" << std::endl;
        return -1;
    }

    std::vector<std::string> args(numArguments);
    for (int i = 0; i < numArguments; ++i)
    {
        args[i] = arguments[i];
    }

    // Get YourShader.hlsl.
    std::string filename(arguments[numArguments - 1]);
    std::string filePrefix;
    auto dotLocation = filename.find_last_of('.');
    if (dotLocation != std::string::npos)
    {
        filePrefix = filename.substr(0, dotLocation);
    }
    else
    {
        filePrefix = filename;
    }

    // Create the bytecode output file YourShader.bytecode.
    std::string bcFilename = filePrefix + ".bytecode";

    // Create the command for compiling the HLSL shader.
    std::string type = "";
    std::string typeFilePrefix = "";
    std::string xsName = "";
    std::string command = "fxc ";
    for (int i = 1; i < numArguments - 1; ++i)
    {
        command += args[i];
        command += " ";
        if (args[i - 1] == "/T")
        {
            type = args[i].substr(0, 2);
            type[0] = (char)toupper(type[0]);
            type[1] = (char)toupper(type[1]);
            typeFilePrefix = type + "_" + filePrefix;
            if (type == "VS")
            {
                xsName = "Vertex";
            }
            else if (type == "PS")
            {
                xsName = "Pixel";
            }
            else if (type == "GS")
            {
                xsName = "Geometry";
            }
            else if (type == "CS")
            {
                xsName = "Compute";
            }
        }
    }
    command += "/Fo " + bcFilename + " " + filename;

    if (system(command.c_str()) != 0)
    {
        std::cout << "Failed to compile " << filename;
        return -2;
    }

    std::ifstream input(bcFilename, std::ios::ate | std::ios::binary);
    if (!input)
    {
        std::cout << "Cannot open input " << bcFilename << std::endl;
        return -3;
    }
    unsigned int numBytes = (unsigned int)input.tellg();
    std::vector<unsigned char> bytecode(numBytes);
    input.seekg(0);
    input.read((char*)&bytecode[0], numBytes);
    input.close();

    std::string hName = type + "_" + filePrefix + ".h";
    std::ofstream output(hName);
    if (!output)
    {
        std::cout << "Cannot open output " << hName << std::endl;
        return -4;
    }
    output << "#pragma once" << std::endl << std::endl;
    output << "#include <Gte" << xsName << "Shader.h>" << std::endl;
    output << "gte::" << xsName << "Shader* " << typeFilePrefix << "();" << std::endl;
    output.close();

    std::string cppName = type + "_" + filePrefix + ".cpp";
    output.open(cppName);
    if (!output)
    {
        std::cout << "Cannot open output " << cppName << std::endl;
        return -5;
    }
    output << "#include \"" + hName + "\"" << std::endl;
    output << "#include <GteShaderFactory.h>" << std::endl << std::endl;
    std::string varNumBytes = "gs_" + filePrefix + "_numBytes";
    std::string varByteCode = "gs_" + filePrefix + "_bytecode";
    output << "static int numBytes = " << bytecode.size() << ";" << std::endl;
    output << "static unsigned char bytecode[] =" << std::endl;
    output << "{" << std::endl;
    int const numPerRow = 16, numPerRowM1 = numPerRow - 1;
    unsigned char const* bytes = &bytecode[0];
    for (size_t i = 0; i < bytecode.size(); ++i, ++bytes)
    {
        output << std::setw(3) << (unsigned int)*bytes << ", ";
        if ((i % numPerRow) == numPerRowM1)
        {
            output << std::endl;
        }
    }
    output << std::endl << "};" << std::endl << std::endl;
    output << "gte::" << xsName << "Shader* " << typeFilePrefix << "()" << std::endl;
    output << "{" << std::endl;
    output << "    return gte::ShaderFactory::Create" << xsName << "(\""
        << filePrefix << "\", numBytes, bytecode);" << std::endl;
    output << "}" << std::endl;
    output.close();

    command = "del " + bcFilename;
    system(command.c_str());
    return 0;
}
