#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0x10 in the 32-bit flags word at +0x14 and returns the updated word.
// The second stack argument (a2) is unused by the original but is part of its ret 8 frame.
static uint32_t __fastcall C_00863430_SetFlagBit10(C_00863430* self, void* /*edx*/, int /*unusedA2*/, char enable) {
    uint32_t flags = self->field_0014;
    if (enable) {
        flags = flags | 0x10u;
    } else {
        flags = flags & 0xFFFFFFEFu;
    }
    self->field_0014 = flags;
    return flags;
}

HOOK(0x708310, C_00863430_SetFlagBit10, "auto");
