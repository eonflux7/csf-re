#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the dword at offset 0x184 of the object (0x184 is inside a base-class
// region not covered by the declared C_00865328 fields, so access it by offset).
static int __fastcall C_00865328_GetField0184(C_00865328* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x184));
}

HOOK(0x724F00, C_00865328_GetField0184, "auto");
