#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets or clears bit 0x02 in the 32-bit flags word at +0x14 and returns the updated word.
static uint32_t __fastcall C_00862598_SetBit2Flag(C_00862598* self, void* /*edx*/, char enable) {
    uint32_t flags = self->field_0014;
    if (enable) {
        flags = flags | 0x2u;
    } else {
        flags = flags & 0xFFFFFFFDu;
    }
    self->field_0014 = flags;
    return flags;
}

HOOK(0x6CA0B0, C_00862598_SetBit2Flag, "auto");
