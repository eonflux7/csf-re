#include <cstdint>
#include "hook.h"

// __thiscall with no stack arguments: ecx = this, edx unused.
// Returns 1 when bit 1 of the flags word is clear and bit 2 is set, else 0.
static int __fastcall HudStatusIcons_IsFlagTwoSetFlagOneClear(uint32_t* self, void* /*edx*/)
{
    uint32_t flags = *self;
    if ((flags & 1u) == 0 && (flags & 2u) != 0)
        return 1;
    return 0;
}

HOOK(0x45A960, HudStatusIcons_IsFlagTwoSetFlagOneClear, "auto");
