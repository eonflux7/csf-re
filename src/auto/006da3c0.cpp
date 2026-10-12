#include <cstdint>
#include "globals.h"
#include "hook.h"

// Zeroes the entry's runtime state fields and returns 0.
static int __fastcall GfxSkin_CompleteEntrySetup(void* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 0;
    return 0;
}

HOOK(0x006DA3C0, GfxSkin_CompleteEntrySetup, "auto");
