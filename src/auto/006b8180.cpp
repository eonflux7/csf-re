#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns bit 3 of the dword at offset +0x4 (a flag/predicate test on the
// object's state word). No stack arguments: ret 0.
static int __fastcall IsFlagBit3Set(void* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 0x4) >> 3) & 1;
}

HOOK(0x006B8180, IsFlagBit3Set, "auto");
