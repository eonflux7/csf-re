#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, GetPositionById, 0x004DE860, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static float* __fastcall S_004e0de0_GetAveragePosition(
    S_004e0de0* self, void* /*edx*/, float* output)
{
    alignas(8) uint8_t sumBytes[0x10];
    float* sum = reinterpret_cast<float*>(sumBytes);
    sum[0] = 0.0f;
    sum[1] = 0.0f;
    sum[2] = 0.0f;

    const uint32_t count = self->field_0008;
    CSF_AT(uint32_t, sumBytes, 0x0C) = count;

    uint32_t remaining = count;
    uint32_t offset = 0;
    while (remaining != 0)
    {
        const int id = CSF_AT(
            int, reinterpret_cast<void*>(self->field_000c), offset);

        const uint32_t positionContainer = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        const float* position = reinterpret_cast<float*>(GetPositionById(
            reinterpret_cast<void*>(positionContainer), nullptr, id));

        offset += 0x10;
        --remaining;
        sum[0] += position[0];
        sum[1] += position[1];
        sum[2] += position[2];
    }

    CSF_AT(uint32_t, sumBytes, 0x0C) = count;
    const int32_t signedCount = static_cast<int32_t>(count);
    float divisor = static_cast<float>(signedCount);
    if (signedCount < 0)
        divisor += g_UnsignedIntegerBias;

    Vec3_NormalizeHelper(sumBytes, nullptr, divisor);

    output[0] = sum[0];
    output[1] = sum[1];
    output[2] = sum[2];
    return output;
}

HOOK(0x004E0DE0, S_004e0de0_GetAveragePosition, "auto");
