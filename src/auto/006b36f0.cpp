#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0x40 in the 32-bit flags word at +0x64 and returns the updated word.
static unsigned int __fastcall C_00860288_SetFlagBit40(C_00860288* self, void* /*edx*/, char enable) {
    uint32_t flags = self->field_0064;
    if (enable) {
        flags = flags | 0x40u;
    } else {
        flags = flags & 0xFFFFFFBFu;
    }
    self->field_0064 = flags;
    return flags;
}

HOOK(0x6B36F0, C_00860288_SetFlagBit40, "auto");
