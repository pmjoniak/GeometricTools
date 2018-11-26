// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.2 (2018/10/05)

#include "DeformableBall.h"
#include <Imagics/GteSurfaceExtractor.h>

DeformableBall::DeformableBall(float duration, float period, std::shared_ptr<Texture2Effect> const& effect)
    :
    mDeforming(false),
    mDoAffine(true)
{
    Set(duration, period);
    CreateBall(effect);
}

void DeformableBall::Set(float duration, float period)
{
    mDuration = duration;
    mDeformMult = 4.0f / (mDuration * mDuration);
    mPeriod = period;
    mMinActive = 0.5f * (mPeriod - mDuration);
    mMaxActive = 0.5f * (mPeriod + mDuration);
    mInvActiveRange = 1.0f / (mMaxActive - mMinActive);
}

void DeformableBall::CreateBall(std::shared_ptr<Texture2Effect> const& effect)
{
    // Create initial image for surface extraction (16 x 16 x 16).
    int const bound = 16;
    float const invBoundM1 = 1.0f / (bound - 1);
    Image3<float> image(bound, bound, bound);
    SurfaceExtractor<float> extractor(image);

    // Scale function values to [-1024,1024].
    float const imageScale = 1024.0f;

    // Initialize image and extract level surface F = 0.  Data stores samples
    // for (x,y,z) in [-1,1]x[-1,1]x[0,2].
    Vector3<float> position;
    for (int z = 0, i = 0; z < bound; ++z)
    {
        position[2] = -0.1f + 2.2f * invBoundM1 * z;
        for (int y = 0; y < bound; ++y)
        {
            position[1] = -1.1f + 2.2f * invBoundM1 * y;
            for (int x = 0; x < bound; ++x, ++i)
            {
                position[0] = -1.1f + 2.2f * invBoundM1 * x;

                float function;
                Vector3<float> gradient;
                ComputeFunction(position, 0.0f, function, gradient);
                image[i] = round(imageScale * function);
            }
        }
    }

    // Extract the level surface.
    std::vector<Vector3<float>> vertices;
    std::vector<int> indices;
    extractor.Extract(0.0f, vertices, indices);
    extractor.MakeUnique(vertices, indices);
    extractor.OrientTriangles(vertices, indices, true);

    // Convert to a triangle mesh.  Keep track of the level value of the
    // vertices.  Since a vertex might not be exactly on the level surface,
    // we will use
    //     e = max{|F(x,y,z)| : (x,y,z) is a vertex}
    // as the error tolerance for Newton's method in the level surface
    // evolution.
    VertexFormat vformat;
    vformat.Bind(VA_POSITION, DF_R32G32B32_FLOAT, 0);
    vformat.Bind(VA_TEXCOORD, DF_R32G32_FLOAT, 0);

    unsigned int const numVertices = static_cast<unsigned int>(vertices.size());
    std::shared_ptr<VertexBuffer> vbuffer = std::make_shared<VertexBuffer>(vformat, numVertices);
    vbuffer->SetUsage(Resource::DYNAMIC_UPDATE);
    Vertex* vertex = vbuffer->Get<Vertex>();
    float maxAbsLevel = 0.0f;
    for (unsigned int i = 0; i < numVertices; ++i)
    {
        Vector3<float> surfacePosition = vertices[i];
        vertex[i].position[0] = -1.1f + 2.2f * invBoundM1 * surfacePosition[0];
        vertex[i].position[1] = -1.1f + 2.2f * invBoundM1 * surfacePosition[1];
        vertex[i].position[2] = -0.1f + 2.2f * invBoundM1 * surfacePosition[2];

        float absLevel = std::abs(Dot(vertex[i].position, vertex[i].position) - 2.0f * position[2]);
        if (absLevel > maxAbsLevel)
        {
            maxAbsLevel = absLevel;
        }

        float temp = std::atan2(vertex[i].position[1], vertex[i].position[2]) / (float)GTE_C_PI;
        float width = 0.5f * (1.0f + temp);  // in [0,1)
        if (width < 0.0f)
        {
            width = 0.0f;
        }
        else if (width >= 1.0f)
        {
            width = 0.999999f;
        }

        float height = 0.5f * vertex[i].position[2]; // in [0,1)
        if (height < 0.0f)
        {
            height = 0.0f;
        }
        else if (height >= 1.0f)
        {
            height = 0.999999f;
        }

        vertex[i].tcoord[0] = width;
        vertex[i].tcoord[1] = height;
    }

    unsigned int const numTriangles = static_cast<unsigned int>(indices.size() / 3);
    std::shared_ptr<IndexBuffer> ibuffer = std::make_shared<IndexBuffer>(IP_TRIMESH,
        numTriangles, sizeof(unsigned int));
    memcpy(ibuffer->GetData(), indices.data(), ibuffer->GetNumBytes());

    mMesh = std::make_shared<Visual>(vbuffer, ibuffer, effect);

    mMaxIterations = 8;
    mErrorTolerance = maxAbsLevel;
    CreateSmoother();
    Update(0.0f);
}

void DeformableBall::CreateSmoother()
{
    unsigned int const numVertices = mMesh->GetVertexBuffer()->GetNumElements();
    mNormal.resize(numVertices);
    mMean.resize(numVertices);
    mNeighborCount.resize(numVertices);

    // Count the number of vertex neighbors.
    std::fill(mNeighborCount.begin(), mNeighborCount.end(), 0);
    unsigned int const numTriangles = mMesh->GetIndexBuffer()->GetNumElements() / 3;
    unsigned int const* indices = mMesh->GetIndexBuffer()->Get<unsigned int>();
    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        mNeighborCount[*indices++] += 2;
        mNeighborCount[*indices++] += 2;
        mNeighborCount[*indices++] += 2;
    }
}

void DeformableBall::Update(float time)
{
    unsigned int const numVertices = mMesh->GetVertexBuffer()->GetNumElements();
    Vertex* vertices = mMesh->GetVertexBuffer()->Get<Vertex>();
    unsigned int const numTriangles = mMesh->GetIndexBuffer()->GetNumElements() / 3;
    unsigned int const* indices = mMesh->GetIndexBuffer()->Get<unsigned int>();

    Vector3<float> zero = Vector3<float>::Zero();
    std::fill(mNormal.begin(), mNormal.end(), zero);
    std::fill(mMean.begin(), mMean.end(), zero);

    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        int i0 = *indices++;
        int i1 = *indices++;
        int i2 = *indices++;

        Vector3<float> v0 = vertices[i0].position;
        Vector3<float> v1 = vertices[i1].position;
        Vector3<float> v2 = vertices[i2].position;

        Vector3<float> edge1 = v1 - v0;
        Vector3<float> edge2 = v2 - v0;
        Vector3<float> normal = Cross(edge1, edge2);

        mNormal[i0] += normal;
        mNormal[i1] += normal;
        mNormal[i2] += normal;

        mMean[i0] += v1 + v2;
        mMean[i1] += v2 + v0;
        mMean[i2] += v0 + v1;
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        Normalize(mNormal[i]);
        mMean[i] /= static_cast<float>(mNeighborCount[i]);
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        Vector3<float> position = vertices[i].position;
        if (VertexInfluenced(i, time, position))
        {
            Vector3<float> localDiff = mMean[i] - position;
            Vector3<float> surfaceNormal = Dot(localDiff, mNormal[i]) * mNormal[i];
            Vector3<float> tangent = localDiff - surfaceNormal;
            float tWeight = GetTangentWeight(i, time, position);
            float nWeight = GetNormalWeight(i, time, position);
            position += tWeight * tangent + nWeight * mNormal[i];
            vertices[i].position = position;
        }
    }
}

bool DeformableBall::DoSimulationStep(float realTime)
{
    float time = std::fmod(realTime, mPeriod);

    if (mMinActive < time && time < mMaxActive)
    {
        // Deform the mesh.
        mDeforming = true;
        Update(time);

        if (mDoAffine)
        {
            // Nonuniform scaling as a hack to make it appear that the body is
            // compressing in the z-direction.  The transformations only
            // affect the display.  If the actual body coordinates were needed
            // for other physics, you would have to modify the mesh vertices.
            //
            // The x- and y-scales vary from 1 to 1.1 to 1 during the time
            // interval [(p-d)/2,(p+d)/2].  The z-scale is the inverse of this
            // scale.  (Expand radially, compress in z-direction.)
            float const maxExpand = 0.1f;
            float amp = 4.0f * maxExpand * mInvActiveRange;
            float xyScale = 1.0f + amp * (time - mMinActive) * (mMaxActive - time);
            float zScale = 1.0f / xyScale;
            mMesh->localTransform.SetScale(xyScale, xyScale, zScale);
        }

        // Deformation requires update of bounding sphere.
        mMesh->UpdateModelBound();

        // An update occurred, so application should update the scene graph.
        return true;
    }

    if (mDeforming)
    {
        // Force restoration of body to its initial state on a transition
        // from deforming to nondeforming.
        mDeforming = false;
        Update(0.0f);
        if (mDoAffine)
        {
            mMesh->localTransform.SetRotation(Matrix4x4<float>::Identity());
        }
        mMesh->UpdateModelBound();
        return true;
    }

    mDeforming = false;
    return false;
}

bool DeformableBall::VertexInfluenced(unsigned int, float time, Vector3<float> const& position) const
{
    float rSqr = Dot(position, position);
    return rSqr < 1.0f && mMinActive < time && time < mMaxActive;
}

float DeformableBall::GetTangentWeight(unsigned int, float, Vector3<float> const&) const
{
    return 0.5f;
}

float DeformableBall::GetNormalWeight(unsigned int i, float time, Vector3<float> const& position) const
{
    // Find root of F along line origin+s*dir using Newton's method.
    float s = 0.0f;
    for (int iter = 0; iter < mMaxIterations; ++iter)
    {
        // Point of evaluation.
        Vector3<float> evalPosition = position + s * mNormal[i];

        // Get F(pos,time) and Grad(F)(pos,time).
        float function;
        Vector3<float> gradient;
        ComputeFunction(evalPosition, time, function, gradient);
        if (std::abs(function) < mErrorTolerance)
        {
            return s;
        }

        // Get directional derivative Dot(dir,Grad(F)(pos,time)).
        float derFunction = Dot(mNormal[i], gradient);
        if (std::abs(derFunction) < mErrorTolerance)
        {
            // Derivative too close to zero, no change.
            return 0.0f;
        }

        s -= function / derFunction;
    }

    // Method failed to converge within iteration budget, no change.
    return 0.0f;
}

void DeformableBall::ComputeFunction(Vector3<float> const& position, float time,
    float& function, Vector3<float>& gradient) const
{
    // Level function is L(X,t) = F(X) + D(X,t) where F(X) = 0 defines the
    // initial body.

    // Compute F(X) = x^2 + y^2 + z^2 - 2*z and Grad(F)(X).
    float rSqr = Dot(position, position);
    float F = rSqr - 2.0f * position[2];
    Vector3<float> FGrad = 2.0f * (position - Vector3<float>::Unit(2));

    // Compute D(X,t) = A(t)*G(X).  The duration is d and the period is p.
    // The amplitude is
    //   A(t) = 0, t in [0,p/2-d]
    //          [t-(p/2-d)][(p/2+d)-t]/d^2, t in [p/2-d,p/2+d]
    //          0, t in [p/2+d,p]
    // The spatial component is G(X) = 1 - (x^2 + y^2 + z^2)
    float D;
    Vector3<float> DGrad;
    if (rSqr < 1.0f && mMinActive < time && time < mMaxActive)
    {
        float amp = GetAmplitude(time);
        D = amp * (1.0f - rSqr);
        DGrad = -2.0f * amp * position;
    }
    else
    {
        D = 0.0f;
        DGrad = Vector3<float>::Zero();
    }

    function = F + D;
    gradient = FGrad + DGrad;
}
