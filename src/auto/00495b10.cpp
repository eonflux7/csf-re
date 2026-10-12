#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Toggles bit 0x4 of the 32-bit flags word at +0x2E4 when the character is
// not blocked (byte at +0x3DE is zero and no bits of mask 0x13000 are set in
// the word at +0x2E0). Returns the low byte of the resulting EAX value.
static char __fastcall C_00833b88_ToggleSpecialActionRequest(C_00833b88* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint8_t, self, 0x3de);

    if (result == 0 && (CSF_AT(uint32_t, self, 0x2e0) & 0x13000) == 0) {
        uint32_t flags = CSF_AT(uint32_t, self, 0x2e4);
        if ((flags & 4) != 0)
            result = flags & 0xFFFFFFFB;
        else
            result = flags | 4;
        CSF_AT(uint32_t, self, 0x2e4) = result;
    }

    return static_cast<char>(result);
}

HOOK(0x495B10, C_00833b88_ToggleSpecialActionRequest, "auto");
