#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, AiFncStatic_EvaluateScanAngle, 0x004B0860, (void* self, void* edx, int arg0, float a4, float a5));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);
CSF_GLOBAL(float, g_StaticScanSamples, 0x0083A218);

static void __fastcall AiFncStatic_SelectScanAngle(
    void* self, void* /*edx*/, int linkedObject)
{
    alignas(8) uint8_t positionBytes[0x20];

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(linkedObject), nullptr);
    void* linkedOwner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* resolvedObject = CSF_AT(void*, linkedOwner, 0x2C);

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, resolvedObject, 0)[0xA4 / 4])(
            resolvedObject, nullptr, positionBytes);

    linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(linkedObject), nullptr);
    linkedOwner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    const double orientation = C_008342e8_ComputeResolvedObjectScalar(
        CSF_AT(C_008342e8*, linkedOwner, 0x2C), nullptr);
    const bool findStrongest = CSF_AT(int, self, 0x1C) == 2139095039;
    const float heading = static_cast<float>(orientation * g_DegreesToRadians);
    const float angleRange =
        CSF_AT(float, self, 0x18) - CSF_AT(float, self, 0x14);

    if (findStrongest)
    {
        float bestMagnitude = 0.0f;
        float bestAngle = 3.4028235e38f;
        float sample = 0.0f;
        do
        {
            const float angle = sample * angleRange * g_flt_0082DC84
                + CSF_AT(float, self, 0x14) + heading;
            const double magnitude = std::fabs(
                AiFncStatic_EvaluateScanAngle(
                    self, nullptr, linkedObject, angle, 3000.0f));

            if (magnitude > bestMagnitude)
            {
                bestMagnitude = static_cast<float>(magnitude);
                bestAngle = angle;
            }

            sample += g_TransitionOne;
        } while (sample <= g_MessageBoxAnimationScale);

        if (bestMagnitude != g_fZero)
        {
            CSF_AT(float, self, 0x20) = bestAngle;
            CSF_AT(int, self, 0x04) = 3;
        }
    }
    else
    {
        const float* sample = &g_StaticScanSamples;
        for (;;)
        {
            const float angle = angleRange * *sample * g_flt_0082DC84
                + CSF_AT(float, self, 0x14) + heading;
            const double score = AiFncStatic_EvaluateScanAngle(
                self, nullptr, linkedObject, angle,
                CSF_AT(float, self, 0x1C));

            if (score > g_fZero)
            {
                CSF_AT(float, self, 0x20) = angle;
                CSF_AT(int, self, 0x04) = 3;
                break;
            }

            const int nextSampleBits = CSF_AT(int, sample, 0x04);
            ++sample;
            if (nextSampleBits == -1082130432)
                break;
        }
    }

    if (CSF_AT(int, self, 0x04) != 3)
        CSF_AT(int, self, 0x04) = 2;
}

HOOK(0x004B09C0, AiFncStatic_SelectScanAngle, "auto");
