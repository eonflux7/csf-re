#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0 (0x01) of the flags word at +0x14 and returns the updated word.
static uint32_t __fastcall C_00862598_SetBit0Flag(C_00862598* self, void* /*edx*/, char enable) {
    uint32_t flags = self->field_0014;
    if (enable) {
        flags = flags | 0x1u;
    } else {
        flags = flags & 0xFFFFFFFEu;
    }
    self->field_0014 = flags;
    return flags;
}

HOOK(0x6CA080, C_00862598_SetBit0Flag, "auto");
