#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x7F7FFFFF is the IEEE-754 bit pattern of FLT_MAX (the largest finite float).
static constexpr uint32_t kFloatMaxBits = 0x7F7FFFFFu;

// Resets a 3D bounds record to its "empty" state: the minimum corner is set to FLT_MAX,
// the maximum corner to 0 (layout inferred from the writes), and the flag bytes cleared.
static int __fastcall BoundsRecord_ResetToEmpty(void* self, void* /*edx*/) {
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x0C) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x10) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x14) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x20) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x24) = kFloatMaxBits;
    CSF_AT(uint32_t, self, 0x28) = 0;
    return static_cast<int>(kFloatMaxBits);
}

HOOK(0x004068B0, BoundsRecord_ResetToEmpty, "auto");
