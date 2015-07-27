// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#include <cstdlib>
#include <iostream>
#include "ProjectTemplate.h"

//----------------------------------------------------------------------------
int main(int numArguments, char* arguments[])
{
    if (3 != numArguments)
    {
        std::cout <<
            "usage: GenerateProject nesting projname" << std::endl;
        std::cout <<
            "Nesting is number of levels from GTEngine folder" << std::endl;
        std::cout << "Example: nesting is 3 for ";
        std::cout << "GTEngine/Samples/Basics/VertexColoring" << std::endl;
        return -1;
    }

    int nesting = atoi(arguments[1]);
    if (nesting <= 0)
    {
        std::cout << "Nesting must be positive" << std::endl;
        return -2;
    }

    // Generate the relative path to GTEngine.vcxproj.
    std::string gtPath;
    for (int i = 0; i < nesting; ++i)
    {
        gtPath += "..\\";
    }

    // Generate the files for the project.
    std::string name = arguments[2];
    bool success = false;
    TemplateVC120 generate(name, gtPath, success);
    if (!success)
    {
        std::cout << "Could not create the project files." << std::endl;
        return -5;
    }

    return 0;
}
//----------------------------------------------------------------------------
