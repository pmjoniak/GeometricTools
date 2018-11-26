// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2018
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// File Version: 3.0.0 (2016/06/19)

#include <GTEngine.h>
using namespace gte;

int PrimalQuery2ToLine(bool isFloat, bool forBSNumber)
{
    // ToLine (no order parameter):
    //Real x0 = test[0] - vec0[0];
    //Real y0 = test[1] - vec0[1];
    //Real x1 = vec1[0] - vec0[0];
    //Real y1 = vec1[1] - vec0[1];
    //Real x0y1 = x0*y1;
    //Real x1y0 = x1*y0;
    //Real det = x0y1 - x1y0;
    //
    // ToLine (with order parameter), worst-case path:
    //Real x0 = test[0] - vec0[0];
    //Real y0 = test[1] - vec0[1];
    //Real x1 = vec1[0] - vec0[0];
    //Real y1 = vec1[1] - vec0[1];
    //Real x0y1 = x0*y1;
    //Real x1y0 = x1*y0;
    //Real det = x0y1 - x1y0;
    //Real x0x1 = x0*x1;
    //Real y0y1 = y0*y1;
    //Real dot = x0x1 + y0y1;
    //Real x0x0 = x0*x0;
    //Real y0y0 = y0*y0;
    //Real sqrlen = x0x0 + y0y0;
    //
    // ToLineExtended, worst-case path:
    //Real x0 = Q1[0] - Q0[0];
    //Real y0 = Q1[1] - Q0[1];
    //Real x1 = P[0] - Q0[0];
    //Real y1 = P[1] - Q0[1];
    //Real x2 = P[0] - Q1[0];
    //Real y2 = P[1] - Q1[1];
    //Real x0y1 = x0 * y1;
    //Real x1y0 = x1 * y0;
    //Real det = x0y1 - x1y0;
    //Real x0x1 = x0 * x1;
    //Real y0y1 = y0 * y1;
    //Real dot = x0x1 + y0y1;
    //Real x0x0 = x0 * x0;
    //Real y0y0 = y0 * y0;
    //Real sqrLength = x0x0 + y0y0;

    // test[.], vec0[.], vec1[.]
    BSPrecision u(isFloat, forBSNumber);
    // x0, y0, x1, y1
    BSPrecision add0 = u + u;
    // x0y1, x1y0, x0x1, y0y1, x0x0, y0y0
    BSPrecision mul = add0 * add0;
    // det, dot, sqrLength
    BSPrecision add1 = mul + mul;
    return add1.GetNumWords();
}

int PrimalQuery2ToCircumcircle(bool isFloat, bool forBSNumber)
{
    //Real x0 = vec0[0] - test[0];
    //Real y0 = vec0[1] - test[1];
    //Real s00 = vec0[0] + test[0];
    //Real s01 = vec0[1] + test[1];
    //Real t00 = s00*x0;
    //Real t01 = s01*y0;
    //Real z0 = t00 + t01;

    //Real x1 = vec1[0] - test[0];
    //Real y1 = vec1[1] - test[1];
    //Real s10 = vec1[0] + test[0];
    //Real s11 = vec1[1] + test[1];
    //Real t10 = s10*x1;
    //Real t11 = s11*y1;
    //Real z1 = t10 + t11;

    //Real x2 = vec2[0] - test[0];
    //Real y2 = vec2[1] - test[1];
    //Real s20 = vec2[0] + test[0];
    //Real s21 = vec2[1] + test[1];
    //Real t20 = s20*x2;
    //Real t21 = s21*y2;
    //Real z2 = t20 + t21;

    //Real y0z1 = y0*z1;
    //Real y0z2 = y0*z2;
    //Real y1z0 = y1*z0;
    //Real y1z2 = y1*z2;
    //Real y2z0 = y2*z0;
    //Real y2z1 = y2*z1;
    //Real c0 = y1z2 - y2z1;
    //Real c1 = y2z0 - y0z2;
    //Real c2 = y0z1 - y1z0;
    //Real x0c0 = x0*c0;
    //Real x1c1 = x1*c1;
    //Real x2c2 = x2*c2;
    //Real term = x0c0 + x1c1;
    //Real det = term + x2c2;

    // test[.], vec0[.], vec1[.], vec2[.]
    BSPrecision u(isFloat, forBSNumber);
    // x0, y0, s00, s01, x1, y1, s10, s11, x2, y2, s20, s21
    BSPrecision add0 = u + u;
    // t00, t01, t10, t11, t20, t21
    BSPrecision mul0 = add0 * add0;
    // z0, z1, z2
    BSPrecision add1 = mul0 + mul0;
    // y0z1, y0z2, y1z0, y1z2, y2z0, y2z1
    BSPrecision mul1 = add0 * add1;
    // c0, c1, c2
    BSPrecision add2 = mul1 + mul1;
    // x0c0, x1c1, x2c2
    BSPrecision mul2 = add0 * add2;
    // term
    BSPrecision add3 = mul2 + mul2;
    // det
    BSPrecision add4 = add3 + mul2;
    return add4.GetNumWords();
}

int PrimalQuery3ToPlane(bool isFloat, bool forBSNumber)
{
    //Real x0 = test[0] - vec0[0];
    //Real y0 = test[1] - vec0[1];
    //Real z0 = test[2] - vec0[2];
    //Real x1 = vec1[0] - vec0[0];
    //Real y1 = vec1[1] - vec0[1];
    //Real z1 = vec1[2] - vec0[2];
    //Real x2 = vec2[0] - vec0[0];
    //Real y2 = vec2[1] - vec0[1];
    //Real z2 = vec2[2] - vec0[2];
    //Real y1z2 = y1*z2;
    //Real y2z1 = y2*z1;
    //Real y2z0 = y2*z0;
    //Real y0z2 = y0*z2;
    //Real y0z1 = y0*z1;
    //Real y1z0 = y1*z0;
    //Real c0 = y1z2 - y2z1;
    //Real c1 = y2z0 - y0z2;
    //Real c2 = y0z1 - y1z0;
    //Real x0c0 = x0*c0;
    //Real x1c1 = x1*c1;
    //Real x2c2 = x2*c2;
    //Real term = x0c0 + x1c1;
    //Real det = term + x2c2;

    // test[.], vec0[.], vec1[.], vec2[.]
    BSPrecision u(isFloat, forBSNumber);
    // x0, y0, z0, x1, y1, z1, x2, y2, z2
    BSPrecision add0 = u + u;
    // y1z2, y2z1, y2z0, y0z2, y0z1, y1z0
    BSPrecision mul0 = add0 * add0;
    // c0, c1, c2
    BSPrecision add1 = mul0 + mul0;
    // x0c0, x1c1, x2c2
    BSPrecision mul1 = add0 * add1;
    // term
    BSPrecision add2 = mul1 + mul1;
    // det
    BSPrecision add3 = add2 + mul1;
    return add3.GetNumWords();
}

int PrimalQuery3ToCircumsphere(bool isFloat, bool forBSNumber)
{
    //Real x0 = vec0[0] - test[0];
    //Real y0 = vec0[1] - test[1];
    //Real z0 = vec0[2] - test[2];
    //Real s00 = vec0[0] + test[0];
    //Real s01 = vec0[1] + test[1];
    //Real s02 = vec0[2] + test[2];
    //Real t00 = s00*x0;
    //Real t01 = s01*y0;
    //Real t02 = s02*z0;
    //Real t00pt01 = t00 + t01;
    //Real w0 = t00pt01 + t02;

    //Real x1 = vec1[0] - test[0];
    //Real y1 = vec1[1] - test[1];
    //Real z1 = vec1[2] - test[2];
    //Real s10 = vec1[0] + test[0];
    //Real s11 = vec1[1] + test[1];
    //Real s12 = vec1[2] + test[2];
    //Real t10 = s10*x1;
    //Real t11 = s11*y1;
    //Real t12 = s12*z1;
    //Real t10pt11 = t10 + t11;
    //Real w1 = t10pt11 + t12;

    //Real x2 = vec2[0] - test[0];
    //Real y2 = vec2[1] - test[1];
    //Real z2 = vec2[2] - test[2];
    //Real s20 = vec2[0] + test[0];
    //Real s21 = vec2[1] + test[1];
    //Real s22 = vec2[2] + test[2];
    //Real t20 = s20*x2;
    //Real t21 = s21*y2;
    //Real t22 = s22*z2;
    //Real t20pt21 = t20 + t21;
    //Real w2 = t20pt21 + t22;

    //Real x3 = vec3[0] - test[0];
    //Real y3 = vec3[1] - test[1];
    //Real z3 = vec3[2] - test[2];
    //Real s30 = vec3[0] + test[0];
    //Real s31 = vec3[1] + test[1];
    //Real s32 = vec3[2] + test[2];
    //Real t30 = s30*x3;
    //Real t31 = s31*y3;
    //Real t32 = s32*z3;
    //Real t30pt31 = t30 + t31;
    //Real w3 = t30pt31 + t32;

    //Real x0y1 = x0*y1;
    //Real x0y2 = x0*y2;
    //Real x0y3 = x0*y3;
    //Real x1y0 = x1*y0;
    //Real x1y2 = x1*y2;
    //Real x1y3 = x1*y3;
    //Real x2y0 = x2*y0;
    //Real x2y1 = x2*y1;
    //Real x2y3 = x2*y3;
    //Real x3y0 = x3*y0;
    //Real x3y1 = x3*y1;
    //Real x3y2 = x3*y2;
    //Real a0 = x0y1 - x1y0;
    //Real a1 = x0y2 - x2y0;
    //Real a2 = x0y3 - x3y0;
    //Real a3 = x1y2 - x2y1;
    //Real a4 = x1y3 - x3y1;
    //Real a5 = x2y3 - x3y2;

    //Real z0w1 = z0*w1;
    //Real z0w2 = z0*w2;
    //Real z0w3 = z0*w3;
    //Real z1w0 = z1*w0;
    //Real z1w2 = z1*w2;
    //Real z1w3 = z1*w3;
    //Real z2w0 = z2*w0;
    //Real z2w1 = z2*w1;
    //Real z2w3 = z2*w3;
    //Real z3w0 = z3*w0;
    //Real z3w1 = z3*w1;
    //Real z3w2 = z3*w2;
    //Real b0 = z0w1 - z1w0;
    //Real b1 = z0w2 - z2w0;
    //Real b2 = z0w3 - z3w0;
    //Real b3 = z1w2 - z2w1;
    //Real b4 = z1w3 - z3w1;
    //Real b5 = z2w3 - z3w2;
    //Real a0b5 = a0*b5;
    //Real a1b4 = a1*b4;
    //Real a2b3 = a2*b3;
    //Real a3b2 = a3*b2;
    //Real a4b1 = a4*b1;
    //Real a5b0 = a5*b0;
    //Real term0 = a0b5 - a1b4;
    //Real term1 = term0 + a2b3;
    //Real term2 = term1 + a3b2;
    //Real term3 = term2 - a4b1;
    //Real det = term3 + a5b0;

    // test[.], vec0[.], vec1[.], vec2[.], vec3[.]
    BSPrecision u(isFloat, forBSNumber);
    // x0, y0, z0, s00, s01, s02
    // x1, y1, z1, s10, s11, s12
    // x2, y2, z2, s20, s21, s22
    // x3, y3, z3, s30, s31, s32
    BSPrecision add0 = u + u;
    // t00, t01, t02, t10, t11, t12, t20, t21, t22, t30, t31, t32
    BSPrecision mul0 = add0 * add0;
    // t00pt01, t10pt11, t20pt21, t30pt31
    BSPrecision add1 = mul0 + mul0;
    // w0, w1, w2, w3
    BSPrecision add2 = add1 + mul0;
    // x0y1, x0y2, x0y3, x1y0, x1y2, x1y3, x2y0, x2y1, x2y3, x3y0, x3y1, x3y2
    BSPrecision mul1 = add0 * add0;
    // a0, a1, a2, a3, a4, a5
    BSPrecision add3 = mul1 + mul1;
    // z0w1, z0w2, z0w3, z1w0, z1w2, z1w3, z2w0, z2w1, z2w3, z3w0, z3w1, z3w2
    BSPrecision mul2 = add0 * add2;
    // b0, b1, b2, b3, b4, b4
    BSPrecision add4 = mul2 + mul2;
    // a0b5, a1b4, a2b3, a3b2, a4b1, a5b0
    BSPrecision mul3 = add3 * add4;
    // term0
    BSPrecision add5 = mul3 + mul3;
    // term1
    BSPrecision add6 = add5 + mul3;
    // term2
    BSPrecision add7 = add6 + mul3;
    // term3
    BSPrecision add8 = add7 + mul3;
    // det
    BSPrecision add9 = add8 + mul3;
    return add9.GetNumWords();
}

int main(int, char const*[])
{
    int32_t bsNumberFloatWords, bsNumberDoubleWords;
    int32_t bsRationalFloatWords, bsRationalDoubleWords;

    bsNumberFloatWords = PrimalQuery2ToLine(true, true);  // 18
    bsNumberDoubleWords = PrimalQuery2ToLine(false, true);  // 132
    bsRationalFloatWords = PrimalQuery2ToLine(true, false);  // 214
    bsRationalDoubleWords = PrimalQuery2ToLine(false, false);  // 1587

    bsNumberFloatWords = PrimalQuery2ToCircumcircle(true, true);  // 35
    bsNumberDoubleWords = PrimalQuery2ToCircumcircle(false, true);  // 263
    bsRationalFloatWords = PrimalQuery2ToCircumcircle(true, false);  // 7008
    bsRationalDoubleWords = PrimalQuery2ToCircumcircle(false, false);  // 52859

    bsNumberFloatWords = PrimalQuery3ToPlane(true, true);  // 27
    bsNumberDoubleWords = PrimalQuery3ToPlane(false, true);  // 197
    bsRationalFloatWords = PrimalQuery3ToPlane(true, false);  // 2882
    bsRationalDoubleWords = PrimalQuery3ToPlane(false, false);  // 21688

    bsNumberFloatWords = PrimalQuery3ToCircumsphere(true, true);  // 44
    bsNumberDoubleWords = PrimalQuery3ToCircumsphere(false, true);  // 329
    bsRationalFloatWords = PrimalQuery3ToCircumsphere(true, false);  // 298037
    bsRationalDoubleWords = PrimalQuery3ToCircumsphere(false, false);  // 2254442
    return 0;
}
