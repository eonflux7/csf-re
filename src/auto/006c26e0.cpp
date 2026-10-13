#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

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
CSF_GLOBAL(float, g_AcosHalfPiTail, 0x00845DDC);
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
CSF_GLOBAL(float, g_QuaternionLinearThreshold, 0x00845E0C);

static void AnimCompress_DecodeQuaternion(const void* keyframe, float* quaternion)
{
    hook::Regs registers{};
    registers.eax = reinterpret_cast<uint32_t>(keyframe);
    registers.esi = reinterpret_cast<uint32_t>(quaternion);
    hook::call_regs(CSF_ADDR_FN(0x006C24B0), &registers, nullptr, 0);
}

static uint32_t AnimCompress_GetFloatBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static float AnimCompress_AsinCorrection(float squaredValue)
{
    const float numerator =
        (((((squaredValue * g_AsinNumerator5 + g_AsinNumerator4)
             * squaredValue - g_AsinNumerator3)
            * squaredValue + g_AsinNumerator2)
           * squaredValue - g_AsinNumerator1)
          * squaredValue + g_AsinNumerator0)
        * squaredValue;

    const float denominator =
        (((squaredValue * g_AsinDenominator4 - g_AsinDenominator3)
          * squaredValue + g_AsinDenominator2)
         * squaredValue - g_AsinDenominator1)
        * squaredValue + g_TransitionOne;

    return numerator / denominator;
}

static float AnimCompress_ApproximateSine(float angle)
{
    const float squaredAngle = angle * angle;
    return ((((((g_SineCoefficient13 * squaredAngle - g_SineCoefficient11)
                * squaredAngle + g_SineCoefficient9)
               * squaredAngle - g_SineCoefficient7)
              * squaredAngle + g_SineCoefficient5)
             * squaredAngle - g_AsinNumerator0)
            * (squaredAngle * angle))
           + angle;
}

static int __cdecl AnimCompress_InterpolateQuaternion(
    float* output, int firstKeyframe, int secondKeyframe, float time)
{
    alignas(8) uint8_t firstQuaternionBytes[0x24];
    alignas(8) uint8_t secondQuaternionBytes[0x24];
    float* firstQuaternion = reinterpret_cast<float*>(firstQuaternionBytes);
    float* secondQuaternion = reinterpret_cast<float*>(secondQuaternionBytes);

    const void* first = reinterpret_cast<const void*>(firstKeyframe);
    const void* second = reinterpret_cast<const void*>(secondKeyframe);
    AnimCompress_DecodeQuaternion(first, firstQuaternion);
    AnimCompress_DecodeQuaternion(second, secondQuaternion);

    float dot =
        secondQuaternion[1] * firstQuaternion[1]
        + secondQuaternion[0] * firstQuaternion[0]
        + secondQuaternion[2] * firstQuaternion[2]
        + secondQuaternion[3] * firstQuaternion[3];

    float secondWeight =
        (time - CSF_AT(float, first, 0x4))
        / (CSF_AT(float, second, 0x4) - CSF_AT(float, first, 0x4));

    if (dot < g_fZero)
    {
        dot = -dot;
        secondQuaternion[0] = -secondQuaternion[0];
        secondQuaternion[1] = -secondQuaternion[1];
        secondQuaternion[2] = -secondQuaternion[2];
        secondQuaternion[3] = -secondQuaternion[3];
    }

    float firstWeight = g_TransitionOne - secondWeight;

    // The original also takes this branch for an unordered comparison.
    if (!(dot >= g_QuaternionLinearThreshold))
    {
        const uint32_t dotBits = AnimCompress_GetFloatBits(dot);
        const uint32_t magnitudeBits = dotBits & 0x7FFFFFFFu;
        float angle;

        if (magnitudeBits < 0x3F800000)
        {
            if (magnitudeBits >= 0x3F000000)
            {
                if ((dotBits & 0x80000000u) == 0)
                {
                    const float squaredRoot =
                        (g_TransitionOne - dot) * g_HalfDurationScale;
                    const double root = _rwSqrt(squaredRoot);

                    // Deliberate mantissa truncation in the acos approximation.
                    float truncatedRoot = static_cast<float>(root);
                    uint32_t rootBits =
                        AnimCompress_GetFloatBits(truncatedRoot) & 0xFFFFF000u;
                    std::memcpy(&truncatedRoot, &rootBits, sizeof(truncatedRoot));

                    const double halfAngle =
                        AnimCompress_AsinCorrection(squaredRoot) * root
                        + (squaredRoot - truncatedRoot * truncatedRoot)
                            / (truncatedRoot + root)
                        + truncatedRoot;
                    angle = static_cast<float>(halfAngle + halfAngle);
                }
                else
                {
                    const float squaredRoot =
                        (dot + g_TransitionOne) * g_HalfDurationScale;
                    const double root = _rwSqrt(squaredRoot);
                    const double correction =
                        AnimCompress_AsinCorrection(squaredRoot) * root
                        - g_AcosHalfPiTail;
                    angle = static_cast<float>(
                        g_AcosPiHigh - (root + correction + root + correction));
                }
            }
            else if (magnitudeBits > 0x23000000)
            {
                angle = g_AcosHalfPiHigh
                    - (dot - (g_AcosHalfPiTail
                              - AnimCompress_AsinCorrection(dot * dot) * dot));
            }
            else
            {
                angle = g_TransitionHalfPi;
            }
        }
        else if ((dotBits & 0x80000000u) != 0 || dotBits == 0)
        {
            angle = g_AcosPi;
        }
        else
        {
            angle = g_fZero;
        }

        const float inverseSine =
            g_TransitionOne / AnimCompress_ApproximateSine(angle);
        firstWeight =
            AnimCompress_ApproximateSine(angle * firstWeight) * inverseSine;
        secondWeight =
            AnimCompress_ApproximateSine(angle * secondWeight) * inverseSine;
    }

    output[2] = firstWeight * firstQuaternion[0]
        + secondWeight * secondQuaternion[0];
    output[3] = secondWeight * secondQuaternion[1]
        + firstWeight * firstQuaternion[1];
    output[4] = secondWeight * secondQuaternion[2]
        + firstWeight * firstQuaternion[2];
    output[5] = secondWeight * secondQuaternion[3]
        + firstWeight * firstQuaternion[3];

    return reinterpret_cast<int>(output);
}

HOOK(0x006C26E0, AnimCompress_InterpolateQuaternion, "auto");
