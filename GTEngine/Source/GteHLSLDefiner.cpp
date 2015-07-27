// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/12/02)

#include "GTEnginePCH.h"
#include "GteHLSLDefiner.h"
using namespace gte;

//----------------------------------------------------------------------------
HLSLDefiner::HLSLDefiner()
{
#if defined(GTE_USE_MAT_VEC)
    Set(DefineType::DT_INT, "GTE_USE_MAT_VEC", "1");
#else
    Set(DefineType::DT_INT, "GTE_USE_MAT_VEC", "0");
#endif
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::SetInt(std::string const& name, int value)
{
    return Set(DefineType::DT_INT, name, std::to_string(value));
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::SetUnsignedInt(std::string const& name,
    unsigned int value)
{
    return Set(DefineType::DT_UNSIGNED_INT, name, std::to_string(value));
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::SetFloat(std::string const& name, float value)
{
    return Set(DefineType::DT_FLOAT, name, std::to_string(value));
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::SetDouble(std::string const& name, double value)
{
    return Set(DefineType::DT_DOUBLE, name, std::to_string(value));
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::SetString(std::string const& name,
    std::string const& value)
{
    return Set(DefineType::DT_STRING, name, value);
}
//----------------------------------------------------------------------------
HLSLDefiner& HLSLDefiner::Set(DefineType type, std::string const& name,
    std::string const& value)
{
    // If an item already exists with the specified name, update it.
    for (auto& d : mDefines)
    {
        if (name == d.name)
        {
            d.value = value;
            return *this;
        }
    }

    // The item is new, so append it.
    Define define;
    define.type = type;
    define.name = name;
    define.value = value;
    mDefines.push_back(define);
    return *this;
}
//----------------------------------------------------------------------------
D3D_SHADER_MACRO const* HLSLDefiner::GetMacros() const
{
    // D3DCompile expects a null pointer when no defines are necessary.
    if (mDefines.empty())
    {
        return nullptr;
    }

    // Create the array that D3DCompile expects by setting pointers into
    // the Define members of the mDefines array.
    D3D_SHADER_MACRO macro;
    mMacros.clear();
    for (auto const& define : mDefines)
    {
        macro.Name = define.name.c_str();
        macro.Definition = define.value.c_str();
        mMacros.push_back(macro);
    }

    // Add the null terminator that D3DCompile expects.
    macro.Name = nullptr;
    macro.Definition = nullptr;
    mMacros.push_back(macro);

    // This pointer is valid only until another call to GetDefines().
    return &mMacros[0];
}
//----------------------------------------------------------------------------
void HLSLDefiner::Clear()
{
    mDefines.clear();
    mMacros.clear();

#if defined(GTE_USE_MAT_VEC)
    Set(DefineType::DT_INT, "GTE_USE_MAT_VEC", "1");
#else
    Set(DefineType::DT_INT, "GTE_USE_MAT_VEC", "0");
#endif
}
//----------------------------------------------------------------------------
