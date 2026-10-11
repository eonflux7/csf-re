#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0x400 in the 32-bit flags word at +0x04 and returns the updated word.
static uint32_t __fastcall SetObjectFlagBit400(void* self, void* /*edx*/, char enable) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x4);
    if (enable) {
        flags = flags | 0x400u;
    } else {
        flags = flags & 0xFFFFFBFFu;
    }
    CSF_AT(uint32_t, self, 0x4) = flags;
    return flags;
}

HOOK(0x6B8BB0, SetObjectFlagBit400, "auto");
