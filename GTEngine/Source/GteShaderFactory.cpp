// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.3 (2015/02/19)

#include "GTEnginePCH.h"
#include "GteShaderFactory.h"
using namespace gte;

std::string ShaderFactory::defaultShaderModel = "5_0";

//----------------------------------------------------------------------------
VertexShader* ShaderFactory::CreateVertex(std::string const& filename,
    HLSLDefiner const& definer, std::string const& entry,
    std::string const& target, unsigned int flags)
{
    Shader* shader = Create(filename, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_VERTEX_SHADER)
    {
        return static_cast<VertexShader*>(shader);
    }
    else
    {
        LogError("CreateVertex failed for file " + filename + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
PixelShader* ShaderFactory::CreatePixel(std::string const& filename,
    HLSLDefiner const& definer, std::string const& entry,
    std::string const& target, unsigned int flags)
{
    Shader* shader = Create(filename, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_PIXEL_SHADER)
    {
        return static_cast<PixelShader*>(shader);
    }
    else
    {
        LogError("CreatePixel failed for file " + filename + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
GeometryShader* ShaderFactory::CreateGeometry(std::string const& filename,
    HLSLDefiner const& definer, std::string const& entry,
    std::string const& target, unsigned int flags)
{
    Shader* shader = Create(filename, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_GEOMETRY_SHADER)
    {
        return static_cast<GeometryShader*>(shader);
    }
    else
    {
        LogError("CreateGeometry failed for file " + filename + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
ComputeShader* ShaderFactory::CreateCompute(std::string const& filename,
    HLSLDefiner const& definer, std::string const& entry,
    std::string const& target, unsigned int flags)
{
    Shader* shader = Create(filename, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_COMPUTE_SHADER)
    {
        return static_cast<ComputeShader*>(shader);
    }
    else
    {
        LogError("CreateCompute failed for file " + filename + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
VertexShader* ShaderFactory::CreateVertex(std::string const& name,
    std::string const& source, HLSLDefiner const& definer,
    std::string const& entry, std::string const& target,
    unsigned int flags)
{
    Shader* shader = Create(name, source, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_VERTEX_SHADER)
    {
        return static_cast<VertexShader*>(shader);
    }
    else
    {
        LogError("CreateVertex failed for string " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
PixelShader* ShaderFactory::CreatePixel(std::string const& name,
    std::string const& source, HLSLDefiner const& definer,
    std::string const& entry, std::string const& target,
    unsigned int flags)
{
    Shader* shader = Create(name, source, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_PIXEL_SHADER)
    {
        return static_cast<PixelShader*>(shader);
    }
    else
    {
        LogError("CreatePixel failed for string " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
GeometryShader* ShaderFactory::CreateGeometry(std::string const& name,
    std::string const& source, HLSLDefiner const& definer,
    std::string const& entry, std::string const& target,
    unsigned int flags)
{
    Shader* shader = Create(name, source, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_GEOMETRY_SHADER)
    {
        return static_cast<GeometryShader*>(shader);
    }
    else
    {
        LogError("CreateGeometry failed for string " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
ComputeShader* ShaderFactory::CreateCompute(std::string const& name,
    std::string const& source, HLSLDefiner const& definer,
    std::string const& entry, std::string const& target,
    unsigned int flags)
{
    Shader* shader = Create(name, source, definer, entry, target, flags);
    if (shader && shader->GetType() == GT_COMPUTE_SHADER)
    {
        return static_cast<ComputeShader*>(shader);
    }
    else
    {
        LogError("CreateCompute failed for string " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
VertexShader* ShaderFactory::CreateVertex(std::string const& name,
    size_t numBytes, unsigned char const* bytecode, std::string const& entry,
    std::string const& target)
{
    Shader* shader = Create(name, entry, target, numBytes, bytecode);
    if (shader && shader->GetType() == GT_VERTEX_SHADER)
    {
        return static_cast<VertexShader*>(shader);
    }
    else
    {
        LogError("CreateVertex failed for bytecode " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
PixelShader* ShaderFactory::CreatePixel(std::string const& name,
    size_t numBytes, unsigned char const* bytecode, std::string const& entry,
    std::string const& target)
{
    Shader* shader = Create(name, entry, target, numBytes, bytecode);
    if (shader && shader->GetType() == GT_PIXEL_SHADER)
    {
        return static_cast<PixelShader*>(shader);
    }
    else
    {
        LogError("CreatePixel failed for bytecode " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
GeometryShader* ShaderFactory::CreateGeometry(std::string const& name,
    size_t numBytes, unsigned char const* bytecode, std::string const& entry,
    std::string const& target)
{
    Shader* shader = Create(name, entry, target, numBytes, bytecode);
    if (shader && shader->GetType() == GT_GEOMETRY_SHADER)
    {
        return static_cast<GeometryShader*>(shader);
    }
    else
    {
        LogError("CreateGeometry failed for bytecode " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
ComputeShader* ShaderFactory::CreateCompute(std::string const& name,
    size_t numBytes, unsigned char const* bytecode, std::string const& entry,
    std::string const& target)
{
    Shader* shader = Create(name, entry, target, numBytes, bytecode);
    if (shader && shader->GetType() == GT_COMPUTE_SHADER)
    {
        return static_cast<ComputeShader*>(shader);
    }
    else
    {
        LogError("CreateCompute failed for bytecode " + name + ".");
        return nullptr;
    }
}
//----------------------------------------------------------------------------
Shader* ShaderFactory::Create(std::string const& filename,
    HLSLDefiner const& definer, std::string const& entry,
    std::string const& target, unsigned int flags)
{
    HLSLShader hlsl = HLSLShaderFactory::CreateFromFile(filename, entry,
        target, definer, flags);
    return (hlsl.IsValid() ? new Shader(hlsl) : nullptr);
}
//----------------------------------------------------------------------------
Shader* ShaderFactory::Create(std::string const& name,
    std::string const& source, HLSLDefiner const& definer,
    std::string const& entry, std::string const& target, unsigned int flags)
{
    HLSLShader hlsl = HLSLShaderFactory::CreateFromString(name, source, entry,
        target, definer, flags);
    return (hlsl.IsValid() ? new Shader(hlsl) : nullptr);
}
//----------------------------------------------------------------------------
Shader* ShaderFactory::Create(std::string const& name,
    std::string const& entry, std::string const& target, size_t numBytes,
    unsigned char const* bytecode)
{
    HLSLShader hlsl = HLSLShaderFactory::CreateFromBytecode(name, entry,
        target, numBytes, bytecode);
    return (hlsl.IsValid() ? new Shader(hlsl) : nullptr);
}
//----------------------------------------------------------------------------
