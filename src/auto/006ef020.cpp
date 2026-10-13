#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0x04 in the 32-bit flags word at +0x30 and returns the updated word.
static unsigned int __fastcall C_00862110_SetFlagBit04(C_00862110* self, void* /*edx*/, char enable) {
    uint32_t flags = self->field_0030;
    if (enable) {
        flags = flags | 0x4u;
    } else {
        flags = flags & 0xFFFFFFFBu;
    }
    self->field_0030 = flags;
    return flags;
}

HOOK(0x6EF020, C_00862110_SetFlagBit04, "auto");
