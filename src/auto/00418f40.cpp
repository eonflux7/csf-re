#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_0082e1e8_ComputeWheelStatistics(
    C_0082e1e8* self, void* /*edx*/, uint32_t* output)
{
    output[1] = 0;
    output[2] = -1;
    output[3] = -1;
    output[4] = -1;
    output[5] = -1;
    output[6] = -1;
    output[7] = -1;
    output[8] = -1;
    output[9] = -1;
    output[10] = 0;
    output[11] = 0;
    output[12] = 0;
    output[13] = 0;

    const int32_t wheelCount = static_cast<int32_t>(self->field_003c);
    int32_t negativeCount = 0;
    int32_t nonnegativeCount = 0;
    uint32_t byteOffset = 0;

    for (int32_t index = 0; index < wheelCount; ++index)
    {
        auto* left = reinterpret_cast<uint8_t*>(self->field_0034) + byteOffset;
        auto* right = reinterpret_cast<uint8_t*>(self->field_0044) + byteOffset;

        CSF_AT(int32_t, output, 0x04) =
            CSF_AT(int32_t, output, 0x04) < CSF_AT(int32_t, left, 0x0C)
                ? CSF_AT(int32_t, left, 0x0C)
                : CSF_AT(int32_t, output, 0x04);

        CSF_AT(int32_t, output, 0x04) =
            CSF_AT(int32_t, output, 0x04) < CSF_AT(int32_t, right, 0x0C)
                ? CSF_AT(int32_t, right, 0x0C)
                : CSF_AT(int32_t, output, 0x04);

        const int32_t leftState = CSF_AT(int32_t, left, 0x0C);
        if (leftState > 1)
        {
            if (output[2] == static_cast<uint32_t>(-1))
                output[2] = index;
            output[4] = index;
        }
        else if (leftState == 1)
        {
            if (output[6] == static_cast<uint32_t>(-1))
                output[6] = index;
            output[8] = index;
        }

        const int32_t rightState = CSF_AT(int32_t, right, 0x0C);
        if (rightState > 1)
        {
            if (output[3] == static_cast<uint32_t>(-1))
                output[3] = index;
            output[5] = index;
        }
        else if (rightState == 1)
        {
            if (output[7] == static_cast<uint32_t>(-1))
                output[7] = index;
            output[9] = index;
        }

        if (CSF_AT(float, left, 0x08) > g_fZero)
        {
            const float ratio =
                CSF_AT(float, left, 0x14) / CSF_AT(float, left, 0x08);
            CSF_AT(float, output, 0x30) =
                ratio / static_cast<float>(wheelCount) +
                CSF_AT(float, output, 0x30);

            // The original treats an unordered comparison as nonnegative.
            if (CSF_AT(float, left, 0x18) < g_fZero)
            {
                ++negativeCount;
                CSF_AT(float, output, 0x28) =
                    ratio + CSF_AT(float, output, 0x28);
            }
            else
            {
                ++nonnegativeCount;
                CSF_AT(float, output, 0x2C) =
                    ratio + CSF_AT(float, output, 0x2C);
            }
        }

        if (CSF_AT(float, right, 0x08) > g_fZero)
        {
            CSF_AT(float, output, 0x34) =
                CSF_AT(float, right, 0x14) / CSF_AT(float, right, 0x08) /
                    static_cast<float>(wheelCount) +
                CSF_AT(float, output, 0x34);
        }

        byteOffset += 32;
    }

    CSF_AT(float, output, 0x28) =
        CSF_AT(float, output, 0x28) / static_cast<float>(negativeCount);
    CSF_AT(float, output, 0x2C) =
        CSF_AT(float, output, 0x2C) / static_cast<float>(nonnegativeCount);

    return static_cast<int32_t>(byteOffset);
}

HOOK(0x00418F40, C_0082e1e8_ComputeWheelStatistics, "auto");
