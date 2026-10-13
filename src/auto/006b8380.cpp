#include <cstdint>
#include "hook.h"

// Replaces the high 16 bits of the stored dword with the low 16 bits of a2,
// keeping the low 16 bits of the existing value.
static int __fastcall GfxObjectProps_SetSecondaryRecordValue(int* self, void* /*edx*/, int a2) {
    uint32_t currentLow = static_cast<uint16_t>(*self);
    uint32_t newHigh = static_cast<uint32_t>(a2) << 16;
    int result = static_cast<int>(newHigh | currentLow);
    *self = result;
    return result;
}

HOOK(0x6B8380, GfxObjectProps_SetSecondaryRecordValue, "auto");
