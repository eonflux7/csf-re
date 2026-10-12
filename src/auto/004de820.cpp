#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets the subsystem at +0x20 (element stride 0x40, count at +0x28):
// clears the two counters at +0x08/+0x0C, then for each element zeroes the
// dword at +0x3C and clears bits 0 and 1 of the dword at +0x04.
static void __fastcall Scene_ResetSubsystem12C(void* self, void* /*edx*/)
{
    uint32_t remaining = CSF_AT(uint32_t, self, 0x28);
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;

    if (remaining != 0) {
        uint32_t offset = 0;
        do {
            CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x20), offset + 0x3C) = 0;
            CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x20), offset + 0x04) &= 0xFFFFFFFEu;
            CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x20), offset + 0x04) &= 0xFFFFFFFDu;
            offset += 0x40;
            --remaining;
        } while (remaining != 0);
    }
}

HOOK(0x4DE820, Scene_ResetSubsystem12C, "auto");
