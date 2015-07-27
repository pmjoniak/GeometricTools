// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.0 (2014/08/11)

#pragma once

#include <regex>
#include <string>

class TemplateVC120
{
public:
    ~TemplateVC120();
    TemplateVC120(std::string const& name, std::string const& gtPath,
        bool& success);

private:
    bool Create(std::string const& name, int numLines, char const* lines[]);
    static std::string GetGuidString();

    // Matching patterns.
    static std::regex const mGPPattern;
    static std::regex const mPNPattern;
    static std::regex const mPGPattern;
    static std::regex const mRQPattern;
    static std::regex const mGTPattern;

    // Replacements for patterns.
    std::string mName;
    std::string mGTPath;
    std::string mProjectGUID;
    std::string mRequiredGUID;
    static std::string const msGTGUID;

    enum
    {
        NUM_SOLUTION_LINES = 42,
        NUM_PROJECT_LINES = 170,
        NUM_FILTER_LINES = 26,
        NUM_APP_LINES = 63,
        NUM_WINH_LINES = 23,
        NUM_WINC_LINES = 32
    };

    // Solution template.
    static char const* msSolutionLines[NUM_SOLUTION_LINES];

    // Project templates.
    static char const* msProjectLines[NUM_PROJECT_LINES];
    static char const* msFilterLines[NUM_FILTER_LINES];

    // Source code templates.
    static char const* msAppLines[NUM_APP_LINES];
    static char const* msWinHLines[NUM_WINH_LINES];
    static char const* msWinCLines[NUM_WINC_LINES];
};
