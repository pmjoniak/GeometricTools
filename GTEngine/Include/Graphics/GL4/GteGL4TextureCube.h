// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <Graphics/GteTextureCube.h>
#include <Graphics/GL4/GteGL4TextureArray.h>

namespace gte
{

class GTE_IMPEXP GL4TextureCube : public GL4TextureArray
{
public:
    // Construction and destruction.
    virtual ~GL4TextureCube();
    GL4TextureCube(TextureCube const* textureArray);
    static std::shared_ptr<GEObject> Create(void* unused, GraphicsObject const* object);

    // Member access.
    inline TextureCube* GetTexture() const;

    // Returns true if mipmaps need to be generated.
    virtual bool CanAutoGenerateMipmaps() const override;

protected:
    virtual void LoadTextureLevel(unsigned int item, unsigned int level, void const* data) override;
};

inline TextureCube* GL4TextureCube::GetTexture() const
{
    return static_cast<TextureCube*>(mGTObject);
}

}
