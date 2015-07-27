// Geometric Tools LLC, Redmond WA 98052
// Copyright (c) 1998-2015
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 1.0.1 (2014/08/29)

#include "GTEnginePCH.h"
#include "GteShader.h"
using namespace gte;

//----------------------------------------------------------------------------
Shader::Data::Data(GraphicsObjectType inType, std::string const& inName,
    int inBindPoint, int inNumBytes, unsigned int inExtra,
    bool inIsGpuWritable)
    :
    type(inType),
    name(inName),
    bindPoint(inBindPoint),
    numBytes(inNumBytes),
    extra(inExtra),
    isGpuWritable(inIsGpuWritable)
{
}
//----------------------------------------------------------------------------
Shader::Shader(HLSLShader const& program)
    :
    mCompiledCode(program.GetCompiledCode()),
    mNumXThreads(program.GetNumXThreads()),
    mNumYThreads(program.GetNumYThreads()),
    mNumZThreads(program.GetNumZThreads())
{
    mCBufferLayouts.resize(program.GetCBuffers().size());
    int i = 0;
    for (auto const& cb : program.GetCBuffers())
    {
        mData[ConstantBuffer::shaderDataLookup].push_back(
            Data(GT_CONSTANT_BUFFER, cb.GetName(), cb.GetBindPoint(),
            cb.GetNumBytes(), 0, false));

        cb.GenerateLayout(mCBufferLayouts[i]);
        ++i;
    }
    
    mTBufferLayouts.resize(program.GetTBuffers().size());
    i = 0;
    for (auto const& tb : program.GetTBuffers())
    {
        mData[TextureBuffer::shaderDataLookup].push_back(
            Data(GT_TEXTURE_BUFFER, tb.GetName(), tb.GetBindPoint(),
            tb.GetNumBytes(), 0, false));

        tb.GenerateLayout(mTBufferLayouts[i]);
        ++i;
    }

    for (auto const& sb : program.GetSBuffers())
    {
        unsigned int ctrtype = 0xFFFFFFFFu;
        switch (sb.GetType())
        {
        case HLSLStructuredBuffer::SBT_BASIC:
            ctrtype = StructuredBuffer::CT_NONE;
            break;

        case HLSLStructuredBuffer::SBT_APPEND:
        case HLSLStructuredBuffer::SBT_CONSUME:
            ctrtype = StructuredBuffer::CT_APPEND_CONSUME;
            break;

        case HLSLStructuredBuffer::SBT_COUNTER:
            ctrtype = StructuredBuffer::CT_COUNTER;
            break;

        default:
            LogError("Unexpected structured buffer option: " +
                std::to_string(static_cast<int>(sb.GetType())));
        }

        mData[StructuredBuffer::shaderDataLookup].push_back(
            Data(GT_STRUCTURED_BUFFER, sb.GetName(), sb.GetBindPoint(),
            sb.GetNumBytes(), ctrtype, sb.IsGpuWritable()));
    }

    for (auto const& rb : program.GetRBuffers())
    {
        mData[RawBuffer::shaderDataLookup].push_back(
            Data(GT_RAW_BUFFER, rb.GetName(), rb.GetBindPoint(),
            rb.GetNumBytes(), 0, rb.IsGpuWritable()));
    }

    for (auto const& tx : program.GetTextures())
    {
        mData[TextureSingle::shaderDataLookup].push_back(
            Data(GT_TEXTURE_SINGLE, tx.GetName(), tx.GetBindPoint(), 0,
            tx.GetNumDimensions(), tx.IsGpuWritable()));
    }

    for (auto const& ta : program.GetTextureArrays())
    {
        mData[TextureArray::shaderDataLookup].push_back(
            Data(GT_TEXTURE_ARRAY, ta.GetName(), ta.GetBindPoint(), 0,
            ta.GetNumDimensions(), ta.IsGpuWritable()));
    }

    for (auto const& s : program.GetSamplerStates())
    {
        mData[SamplerState::shaderDataLookup].push_back(
            Data(GT_SAMPLER_STATE, s.GetName(), s.GetBindPoint(), 0, 0,
            false));
    }

    // The conversion depends on the HLSLShader::Type ordering to be the
    // same as GraphicsObjectType for GL_SHADER through GL_COMPUTE_SHADER.
    int index = program.GetShaderTypeIndex();
    mType = static_cast<GraphicsObjectType>(GT_SHADER + 1 + index);
}
//----------------------------------------------------------------------------
int Shader::Get(std::string const& name) const
{
    for (int lookup = 0; lookup < NUM_LOOKUP_INDICES; ++lookup)
    {
        int handle = 0;
        for (auto const& data : mData[lookup])
        {
            if (name == data.name)
            {
                return handle;
            }
            ++handle;
        }
    }

    LogWarning("Cannot find data.");
    return -1;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, ConstantBuffer* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_CONSTANT_BUFFER)
    {
        LogError("Mismatch of buffer type.");
        return false;
    }

    if (resource->GetNumBytes() >= static_cast<size_t>(goal.numBytes))
    {
        return true;
    }

    LogError("Invalid number of bytes.");
    return false;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, TextureBuffer* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_TEXTURE_BUFFER)
    {
        LogError("Mismatch of buffer type.");
        return false;
    }

    if (resource->GetNumBytes() >= static_cast<size_t>(goal.numBytes))
    {
        return true;
    }

    LogError("Invalid number of bytes.");
    return false;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, StructuredBuffer* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_STRUCTURED_BUFFER)
    {
        LogError("Mismatch of buffer type.");
        return false;
    }

    if (goal.isGpuWritable && resource->GetUsage() != Resource::SHADER_OUTPUT)
    {
        LogError("Mismatch of GPU write flag.");
        return false;
    }

    // A countered structure buffer can be attached as a read-only input to
    // a shader.  We care about the mismatch in counter type only when the
    // shader needs a countered structure buffer but the attached resource
    // does not have one.
    if (goal.extra != 0 
    &&  goal.extra != static_cast<unsigned int>(resource->GetCounterType()))
    {
        LogError("Mismatch of counter type.");
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, RawBuffer* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_RAW_BUFFER)
    {
        LogError("Mismatch of buffer type.");
        return false;
    }

    if (goal.isGpuWritable && resource->GetUsage() != Resource::SHADER_OUTPUT)
    {
        LogError("Mismatch of GPU write flag.");
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, TextureSingle* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_TEXTURE_SINGLE)
    {
        LogError("Mismatch of texture type.");
        return false;
    }

    if (goal.isGpuWritable && resource->GetUsage() != Resource::SHADER_OUTPUT)
    {
        LogError("Mismatch of GPU write flag.");
        return false;
    }

    if (goal.extra != resource->GetNumDimensions())
    {
        LogError("Mismatch of texture dimensions.");
        return false;
    }

    // TODO: Add validation for HLSLTexture::Component and number of
    // components (requires comparison to TextureFormat value).
    return true;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, TextureArray* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_TEXTURE_ARRAY)
    {
        LogError("Mismatch of texture type.");
        return false;
    }

    if (goal.isGpuWritable && resource->GetUsage() != Resource::SHADER_OUTPUT)
    {
        LogError("Mismatch of GPU write flag.");
        return false;
    }

    if (goal.extra != resource->GetNumDimensions())
    {
        LogError("Mismatch of texture dimensions.");
        return false;
    }

    // TODO: Add validation for HLSLTexture::Component and number of
    // components (requires comparison to TextureFormat value).
    return true;
}
//----------------------------------------------------------------------------
bool Shader::IsValid(Data const& goal, SamplerState* resource) const
{
    if (!resource)
    {
        LogError("Resource is null.");
        return false;
    }

    if (goal.type != GT_SAMPLER_STATE)
    {
        LogError("Mismatch of state.");
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
