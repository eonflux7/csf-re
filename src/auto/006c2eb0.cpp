#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, _rwSqrt, 0x00622EE0, (float a1));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);

CSF_GLOBAL(float, g_SineCoefficient5, 0x00845DC0);
CSF_GLOBAL(float, g_SineCoefficient7, 0x00845DC4);
CSF_GLOBAL(float, g_SineCoefficient9, 0x00845DC8);
CSF_GLOBAL(float, g_SineCoefficient11, 0x00845DCC);
CSF_GLOBAL(float, g_SineCoefficient13, 0x00845DD0);
CSF_GLOBAL(float, g_AcosPiHigh, 0x00845DD4);
CSF_GLOBAL(float, g_AcosHalfPiHigh, 0x00845DD8);
CSF_GLOBAL(float, g_AcosCorrection, 0x00845DDC);
CSF_GLOBAL(float, g_AsinDenominator1, 0x00845DE0);
CSF_GLOBAL(float, g_AsinDenominator2, 0x00845DE4);
CSF_GLOBAL(float, g_AsinDenominator3, 0x00845DE8);
CSF_GLOBAL(float, g_AsinDenominator4, 0x00845DEC);
CSF_GLOBAL(float, g_AsinNumerator0, 0x00845DF0);
CSF_GLOBAL(float, g_AsinNumerator1, 0x00845DF4);
CSF_GLOBAL(float, g_AsinNumerator2, 0x00845DF8);
CSF_GLOBAL(float, g_AsinNumerator3, 0x00845DFC);
CSF_GLOBAL(float, g_AsinNumerator4, 0x00845E00);
CSF_GLOBAL(float, g_AsinNumerator5, 0x00845E04);
CSF_GLOBAL(float, g_AcosPi, 0x00845E08);
CSF_GLOBAL(float, g_SlerpLinearThreshold, 0x00845E0C);

static double GfxAnimCompress_AsinCorrection(double squared)
{
    const double numerator =
        (((((squared * g_AsinNumerator5 + g_AsinNumerator4)
            * squared - g_AsinNumerator3)
           * squared + g_AsinNumerator2)
          * squared - g_AsinNumerator1)
         * squared + g_AsinNumerator0) * squared;
    const double denominator =
        (((squared * g_AsinDenominator4 - g_AsinDenominator3)
           * squared + g_AsinDenominator2)
          * squared - g_AsinDenominator1)
         * squared + g_TransitionOne;
    return numerator / denominator;
}

static double GfxAnimCompress_ApproximateSine(double angle)
{
    const double squared = angle * angle;
    return (((((g_SineCoefficient13 * squared - g_SineCoefficient11)
                * squared + g_SineCoefficient9)
               * squared - g_SineCoefficient7)
              * squared + g_SineCoefficient5)
             * squared - g_AsinNumerator0)
            * (squared * angle) + angle;
}

static float* __cdecl GfxAnimCompress_InterpolateKeyframes(
    float* output, float* first, float* second, float fraction)
{
    float dot = second[3] * first[3] + second[2] * first[2]
              + first[4] * second[4] + second[5] * first[5];

    if (dot < g_fZero)
    {
        dot = -dot;
        second[2] = -second[2];
        second[3] = -second[3];
        second[4] = -second[4];
        second[5] = -second[5];
    }

    float firstWeight = g_TransitionOne - fraction;
    // An unordered comparison also enters the spherical path.
    if (!(dot >= g_SlerpLinearThreshold))
    {
        std::int32_t dotBits;
        std::memcpy(&dotBits, &dot, sizeof(dotBits));
        const std::uint32_t magnitudeBits =
            static_cast<std::uint32_t>(dotBits) & 0x7FFFFFFFu;

        double angle;
        if (magnitudeBits < 0x3F800000)
        {
            if (magnitudeBits >= 0x3F000000)
            {
                if (dotBits >= 0)
                {
                    const float squared =
                        (g_TransitionOne - dot) * g_HalfDurationScale;
                    const double root = _rwSqrt(squared);

                    // Split the root into a high part for the correction.
                    float rootHigh = static_cast<float>(root);
                    std::uint32_t rootBits;
                    std::memcpy(&rootBits, &rootHigh, sizeof(rootBits));
                    rootBits &= 0xFFFFF000;
                    std::memcpy(&rootHigh, &rootBits, sizeof(rootHigh));

                    const double halfAngle =
                        GfxAnimCompress_AsinCorrection(squared) * root
                        + (squared - static_cast<double>(rootHigh) * rootHigh)
                            / (rootHigh + root)
                        + rootHigh;
                    angle = halfAngle + halfAngle;
                }
                else
                {
                    const float squared =
                        (dot + g_TransitionOne) * g_HalfDurationScale;
                    const double root = _rwSqrt(squared);
                    const double correction =
                        GfxAnimCompress_AsinCorrection(squared) * root
                        - g_AcosCorrection;
                    angle = g_AcosPiHigh
                          - (root + correction + root + correction);
                }
            }
            else if (magnitudeBits > 0x23000000)
            {
                const double cosine = dot;
                angle = g_AcosHalfPiHigh
                      - (cosine - (g_AcosCorrection
                          - GfxAnimCompress_AsinCorrection(cosine * cosine)
                            * cosine));
            }
            else
            {
                angle = g_TransitionHalfPi;
            }
        }
        else if (dotBits <= 0)
        {
            angle = g_AcosPi;
        }
        else
        {
            angle = g_fZero;
        }

        const double inverseSine =
            g_TransitionOne / GfxAnimCompress_ApproximateSine(angle);
        firstWeight = static_cast<float>(
            GfxAnimCompress_ApproximateSine(angle * firstWeight) * inverseSine);
        fraction = static_cast<float>(
            GfxAnimCompress_ApproximateSine(angle * fraction) * inverseSine);
    }

    output[2] = fraction * second[2] + firstWeight * first[2];
    output[3] = fraction * second[3] + firstWeight * first[3];
    output[4] = firstWeight * first[4] + fraction * second[4];
    output[5] = fraction * second[5] + firstWeight * first[5];
    output[6] = (second[6] - first[6]) * fraction + first[6];
    output[7] = (second[7] - first[7]) * fraction + first[7];
    output[8] = (second[8] - first[8]) * fraction + first[8];
    return output;
}

HOOK(0x006C2EB0, GfxAnimCompress_InterpolateKeyframes, "auto");
