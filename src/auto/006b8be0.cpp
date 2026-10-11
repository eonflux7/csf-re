#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0x10 (flagLightmap) in the 32-bit flags word at +0x04 and returns the updated word.
static uint32_t __fastcall GfxObjectProps_SetFlagLightmapBit(void* self, void* /*edx*/, char enable) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x4);
    if (enable) {
        flags = flags | 0x10u;
    } else {
        flags = flags & 0xFFFFFFEFu;
    }
    CSF_AT(uint32_t, self, 0x4) = flags;
    return flags;
}

HOOK(0x6B8BE0, GfxObjectProps_SetFlagLightmapBit, "auto");
