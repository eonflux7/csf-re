#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_0069afe0_AllocateSlot(S_0069afe0* self, void* /*edx*/)
{
    if (!self->field_0000 || self->field_001c == self->field_0024)
        return 0;

    const uint32_t startByte = self->field_0018;
    uint32_t byteIndex = startByte;
    uint8_t* bitmap = nullptr;
    bool found = false;

    if (byteIndex < self->field_0014)
    {
        bitmap = reinterpret_cast<uint8_t*>(self->field_0004);
        for (;;)
        {
            if (bitmap[byteIndex] != 0xFF)
            {
                found = true;
                break;
            }

            ++byteIndex;
            if (byteIndex >= self->field_0014)
                break;
        }
    }

    if (!found)
    {
        byteIndex = 0;
        if (!startByte)
            return 0;

        bitmap = reinterpret_cast<uint8_t*>(self->field_0004);
        while (bitmap[byteIndex] == 0xFF)
        {
            if (++byteIndex >= startByte)
                return 0;
        }
    }

    const uint8_t freeBits = static_cast<uint8_t>(~bitmap[byteIndex]);
    uint32_t bitIndex = 0;
    while ((freeBits & (1u << bitIndex)) == 0)
        ++bitIndex;

    bitmap[byteIndex] |= static_cast<uint8_t>(1u << bitIndex);

    const uint32_t allocatedCount = self->field_001c + 1;
    const uint32_t peakCount = CSF_AT(uint32_t, self, 0x20);
    self->field_0018 = byteIndex;
    self->field_001c = allocatedCount;
    if (allocatedCount > peakCount)
        CSF_AT(uint32_t, self, 0x20) = allocatedCount;

    const uint32_t baseAddress = CSF_AT(uint32_t, self, 0x08);
    return static_cast<int>(
        baseAddress + self->field_0010 * (bitIndex + 8 * byteIndex));
}

HOOK(0x0069AFE0, S_0069afe0_AllocateSlot, "auto");
