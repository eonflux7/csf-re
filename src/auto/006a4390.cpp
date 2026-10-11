#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Sums 7 fixed 16-bit slots (0x8C..0xA4 step 4), 121 16-bit values at 0xA8 (step 4),
// and 128 16-bit values at 0x28C (step 4); compares a quarter of the middle sum
// against the outer total and stores the result as a byte at +0x1C.
static bool __fastcall UpdateBufferPressureFlag(void* self, void* /*edx*/) {
    uint32_t sampledSum = 0;
    uint32_t totalSum = 0;

    totalSum += CSF_AT(uint16_t, self, 0xA0);
    totalSum += CSF_AT(uint16_t, self, 0xA4);
    totalSum += CSF_AT(uint16_t, self, 0x9C);
    totalSum += CSF_AT(uint16_t, self, 0x98);
    totalSum += CSF_AT(uint16_t, self, 0x94);
    totalSum += CSF_AT(uint16_t, self, 0x90);
    totalSum += CSF_AT(uint16_t, self, 0x8C);

    for (int i = 0; i < 0x79; ++i) {
        sampledSum += CSF_AT(uint16_t, self, 0xA8 + i * 4);
    }

    for (int i = 0; i < 0x80; ++i) {
        totalSum += CSF_AT(uint16_t, self, 0x28C + i * 4);
    }

    const bool result = (sampledSum >> 2) >= totalSum;
    CSF_AT(uint8_t, self, 0x1C) = result ? 1 : 0;
    return result;
}

HOOK(0x6A4390, UpdateBufferPressureFlag, "auto");
