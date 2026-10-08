#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0x80 in the 32-bit flags word at +0x1F0 and returns the updated word.
static uint32_t __fastcall SetFlagBit80(void* self, void* /*edx*/, char enable) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x1F0);
    if (enable) {
        flags = flags | 0x80u;
    } else {
        flags = flags & 0xFFFFFF7Fu;
    }
    CSF_AT(uint32_t, self, 0x1F0) = flags;
    return flags;
}

HOOK(0x45BCC0, SetFlagBit80, "auto");
