// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#pragma once

#include <LowLevel/GteLogger.h>
#include <array>
#include <fstream>
#include <string>
#include <vector>

// The class implements Hermite cubic interpolation.

template <int N, typename Real>
class CubicInterpolator
{
public:
    // Construction from a data file.
    CubicInterpolator(std::string const& filename);

    // Evaluate the interpolator.  The input is clamped to [min,max], where
    // min = inputs[0] and max = inputs[numSamples-1].
    std::array<Real, N> operator() (Real input) const;

private:
    // Lookup on bounding keys.
    void GetKeyInfo(Real input, Real& normInput, int& key) const;

    // Constructor inputs.  These have 'numSamples' elements.
    int mNumSamples;
    std::vector<Real> mInputs;
    std::vector<std::array<Real, N>> mOutputs;      // mC0
    std::vector<std::array<Real, N>> mTangents0;    // mC1
    std::vector<std::array<Real, N>> mTangents1;

    // Support for key lookup and evaluation.  These have 'numSamples - 1'
    // elements.
    int mNumSamplesM1;
    std::vector<Real> mInvDeltas;
    std::vector<std::array<Real, N>> mC2;
    std::vector<std::array<Real, N>> mC3;

    // For O(1) lookup on bounding keys.
    mutable int mLastIndex;
};

template <int N, typename Real>
CubicInterpolator<N, Real>::CubicInterpolator(std::string const& filename)
    :
    mNumSamples(0),
    mNumSamplesM1(0),
    mLastIndex(0)
{
    std::ifstream inFile(filename.c_str());
    if (!inFile)
    {
        LogError("Failed to open " + filename);
        return;
    }

    inFile >> mNumSamples;
    LogAssert(mNumSamples >= 2, "Interpolation requires two or more samples.");

    mNumSamplesM1 = mNumSamples - 1;

    mInputs.resize(mNumSamples);
    mOutputs.resize(mNumSamples);
    mTangents0.resize(mNumSamples);
    mTangents1.resize(mNumSamples);

    for (int i = 0; i < mNumSamples; ++i)
    {
        inFile >> mInputs[i];
    }

    for (int i = 0; i < mNumSamples; ++i)
    {
        std::array<Real, N>& output = mOutputs[i];
        for (int j = 0; j < N; ++j)
        {
            inFile >> output[j];
        }
    }

    for (int i = 0; i < mNumSamples; ++i)
    {
        std::array<Real, N>& tangent0 = mTangents0[i];
        for (int j = 0; j < N; ++j)
        {
            inFile >> tangent0[j];
        }
    }

    for (int i = 0; i < mNumSamples; ++i)
    {
        std::array<Real, N>& tangent1 = mTangents1[i];
        for (int j = 0; j < N; ++j)
        {
            inFile >> tangent1[j];
        }
    }

    mInvDeltas.resize(mNumSamplesM1);
    mC2.resize(mNumSamplesM1);
    mC3.resize(mNumSamplesM1);

    for (int i0 = 0, i1 = 1; i1 < mNumSamples; ++i0, ++i1)
    {
        Real delta = mInputs[i1] - mInputs[i0];
        LogAssert(delta >(Real)0, "The inputs must be strictly increasing.");

        mInvDeltas[i0] = ((Real)1) / delta;
        for (int j = 0; j < N; ++j)
        {
            Real diff0 = mOutputs[i1][j] - mOutputs[i0][j];
            Real diff1 = mTangents1[i0][j] - mTangents0[i0][j];
            Real tmp0 = diff0 - delta * mTangents0[i0][j];
            Real tmp1 = delta*diff1;
            mC2[i0][j] = (((Real)3) * tmp0 - tmp1);
            mC3[i0][j] = (tmp1 - ((Real)2) * tmp0);
        }
    }
}

template <int N, typename Real>
std::array<Real, N> CubicInterpolator<N, Real>::operator() (Real input) const
{
    Real u;
    int k;
    GetKeyInfo(input, u, k);

    std::array<Real, N> output;
    for (int j = 0; j < N; ++j)
    {
        output[j] = mOutputs[k][j] + u * (mTangents0[k][j] + u * (mC2[k][j] + u * mC3[k][j]));
    }
    return output;
}

template <int N, typename Real>
void CubicInterpolator<N, Real>::GetKeyInfo(Real input, Real& normInput, int& key) const
{
    if (input <= mInputs[0])
    {
        normInput = (Real)0;
        mLastIndex = 0;
        key = 0;
        return;
    }

    if (input >= mInputs[mNumSamplesM1])
    {
        normInput = (Real)1;
        mLastIndex = mNumSamplesM1 - 1;
        key = mLastIndex;
        return;
    }

    int nextIndex;
    if (input > mInputs[mLastIndex])
    {
        nextIndex = mLastIndex + 1;
        while (input >= mInputs[nextIndex])
        {
            mLastIndex = nextIndex;
            ++nextIndex;
        }

        key = mLastIndex;
        normInput = (input - mInputs[key]) * mInvDeltas[key];
    }
    else if (input < mInputs[mLastIndex])
    {
        nextIndex = mLastIndex - 1;
        while (input <= mInputs[nextIndex])
        {
            mLastIndex = nextIndex;
            --nextIndex;
        }

        key = nextIndex;
        normInput = (input - mInputs[key]) * mInvDeltas[key];
    }
    else
    {
        normInput = (Real)0;
        key = mLastIndex;
    }
}
