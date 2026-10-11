#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
// Referenced by the original's unsigned-integer-to-floating-point conversion.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_WheelResponseScale, 0x0082E258);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static void C_0082e1e8_AccumulateWheelPair(
    C_0082e1e8* self, void* destination, uint32_t offset, float normalization)
{
    auto* left = reinterpret_cast<uint8_t*>(self->field_0034) + offset;
    auto* right = reinterpret_cast<uint8_t*>(self->field_0044) + offset;

    const float leftResponse =
        CSF_AT(float, left, 0x08) == g_fZero
            ? g_fZero
            : (CSF_AT(float, left, 0x14)
               - self->field_0080 * CSF_AT(float, left, 0x08))
                  * (normalization / CSF_AT(float, left, 0x08));

    const float rightResponse =
        CSF_AT(float, right, 0x08) == g_fZero
            ? g_fZero
            : (CSF_AT(float, right, 0x14)
               - self->field_0080 * CSF_AT(float, right, 0x08))
                  * (normalization / CSF_AT(float, right, 0x08));

    CSF_AT(float, destination, 0x44) =
        (rightResponse * CSF_AT(float, right, 0x18)
         + leftResponse * CSF_AT(float, left, 0x18))
            * g_WheelResponseScale
        + CSF_AT(float, destination, 0x44);

    CSF_AT(float, destination, 0x48) =
        (rightResponse * CSF_AT(float, right, 0x1C)
         + leftResponse * CSF_AT(float, left, 0x1C))
            * g_WheelResponseScale
        + CSF_AT(float, destination, 0x48);
}

static int __fastcall C_0082e1e8_AccumulateWheelResponse(
    C_0082e1e8* self, void* /*edx*/, int destinationAddress)
{
    void* destination = reinterpret_cast<void*>(destinationAddress);
    const uint32_t count = self->field_003c;
    const float normalization = static_cast<float>(
        g_flt_0082E25C
        / (static_cast<double>(count) + static_cast<double>(count)));

    int result = static_cast<int32_t>(count);
    uint32_t processed = 0;

    if (static_cast<int32_t>(count) >= 4)
    {
        const uint32_t groups = ((count - 4) >> 2) + 1;
        uint32_t offset = 0;

        for (uint32_t group = 0; group < groups; ++group)
        {
            for (uint32_t wheel = 0; wheel < 4; ++wheel)
            {
                C_0082e1e8_AccumulateWheelPair(
                    self, destination, offset, normalization);
                offset += 32;
            }
        }
        processed = 4 * groups;
    }

    if (processed < count)
    {
        uint32_t offset = 32 * processed;
        uint32_t remaining = count - processed;
        do
        {
            C_0082e1e8_AccumulateWheelPair(
                self, destination, offset, normalization);
            offset += 32;
            result = static_cast<int32_t>(--remaining);
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x00419160, C_0082e1e8_AccumulateWheelResponse, "auto");
