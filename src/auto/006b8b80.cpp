#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0 (0x01) in the 32-bit flags word at +0x04 and returns the updated word.
static uint32_t __fastcall GfxObjectProps_SetFlagBit01(void* self, void* /*edx*/, char enable) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x04);
    if (enable) {
        flags = flags | 0x01u;
    } else {
        flags = flags & 0xFFFFFFFEu;
    }
    CSF_AT(uint32_t, self, 0x04) = flags;
    return flags;
}

HOOK(0x6B8B80, GfxObjectProps_SetFlagBit01, "auto");
